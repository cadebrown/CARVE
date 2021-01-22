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

.PHONY: default all clean

default: $(carve_SHARED)

all: $(carve_SHARED)

clean:
	rm -rf $(wildcard $(carve_SHARED) $(src_O))

$(carve_SHARED): $(src_O)
	@mkdir -p $(dir $@)
	$(EMCC) \
		$^ \
		--pre-js src/pre.js \
		-sWASM=1 \
		-sMODULARIZE=1 -sEXPORT_NAME='loadlibcarve' \
		-sEXTRA_EXPORTED_RUNTIME_METHODS=[\"cwrap\",\"stringToUTF8\",\"UTF8ToString\"] \
		-sEXPORTED_FUNCTIONS=[\"_malloc\",\"_free\"] \
		-o $@

build/%.o: %.c $(src_H)
	@mkdir -p $(dir $@)
	$(EMCC) $(EMCC_CFLAGS) $< -c -fPIC -o $@
