# makefile - build system for CARVE
#
#
# @author: Cade Brown <cade@utk.edu>



# -*- Programs -*-

# Emscripten compiler
EMCC         := emcc

# -*- Options -*-

# Emscripten compiler options
EMCC_CFLAGS  := -sASSERTIONS=1 -sSAFE_HEAP=1 -sDEMANGLE_SUPPORT=1


# -*- Files -*-

src_C        := $(wildcard src/*.c)
src_H        := $(wildcard src/*.h) 

# -*- Outputs -*-

src_O        := $(patsubst %.c,build/%.o,$(src_C))

carve_SHARED := build/libcarve.js


# -*- Rules -*-

.PHONY: default all clean update

default: $(carve_SHARED)

all: $(carve_SHARED)

clean:
	rm -rf $(wildcard $(carve_SHARED) $(src_O))

update: build/libcarve.js build/libcarve.wasm
	cp build/libcarve.js docs/js/lib
	cp build/libcarve.wasm docs/js/lib

$(carve_SHARED): $(src_O) src/pre.js
	@mkdir -p $(dir $@)
	$(EMCC) \
		$(src_O) \
		--pre-js src/pre.js \
		-sWASM=1 \
		-sMODULARIZE=1 -sEXPORT_NAME='loadlibcarve' \
		-sFORCE_FILESYSTEM=1 \
		-sEXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\",\"stringToUTF8\",\"UTF8ToString\"] \
		-sEXPORTED_FUNCTIONS=[\"_malloc\",\"_free\"] \
		-o $@

build/%.o: %.c $(src_H)
	@mkdir -p $(dir $@)
	$(EMCC) $(EMCC_CFLAGS) $< -c -fPIC -o $@
