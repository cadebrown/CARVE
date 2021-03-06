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

# RISC-V extensions
RISCV        ?= RV32I

# Defines
DEFS         += $(foreach x,$(RISCV),-D$(x))


# -*- Files -*-

src_C        := $(filter-out src/execinst.c src/getinst.c,$(wildcard src/*.c)) src/execinst.c src/getinst.c
src_H        := $(wildcard src/*.h) $(wildcard src/ext/*.h) 

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

src/execinst.c: tools/genexecinst.py tools/riscvdata.py
	$< > $@

src/getinst.c: tools/gengetinst.py tools/riscvdata.py
	$< > $@


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
	$(EMCC) $(EMCC_CFLAGS) $(DEFS) -Isrc $< -c -fPIC -o $@
