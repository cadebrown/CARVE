# makefile - build system for CARVE
#
# Common targets:
#
#   * `make`: Builds 
#
#
#
# @author: Cade Brown <cade@utk.edu>


# -*- Programs -*-

# Emscripten compiler, assumes you have loaded the Emscripten SDK through
#   your dotfiles, or via `source /path/to/emsdk/emsdk_env.sh`
EMCC         := emcc


# -*- Checks -*-

# Check for Emscripten (EMCC)
ifneq ($(shell $(EMCC) --version > /dev/null; echo $$?),0)
  $(error Emscripten is not installed or loaded ($$(EMCC)=$(EMCC)). Try loading the environment variables, or setting `EMCC` to the Emscripten compiler)
endif


# -*- Options -*-

# Emscripten compiler flags
EMCC_CFLAGS  := -sASSERTIONS=1 -sSAFE_HEAP=1 -sDEMANGLE_SUPPORT=1

# Emscripten linker flags (for final compilation)
EMCC_LDFLAGS := -sWASM=1 -sFORCE_FILESYSTEM=1 \
				-sEXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\",\"stringToUTF8\",\"UTF8ToString\"] \
				-sEXPORTED_FUNCTIONS=[\"_malloc\",\"_free\"]

CXXFLAGS += -g

# -*- Files -*-

src_CC       := $(sort $(wildcard src-c/*.cc) $(wildcard src-c/impl/*.cc) src-c/insts.cc src-c/exec.cc)
src_HH       := $(wildcard src-c/*.hh)

src_O        := $(patsubst %.cc,build/%.o,$(src_CC))
src_EMCCO    := $(patsubst %.cc,build/%.emcco,$(src_CC))

out_LIB_JS   := src/assets/js/libcarve.js
out_CLI      := carve


# -*- Rules -*-

.PHONY: default all clean lib bin serve

default: lib bin

all: lib bin

clean:
	rm -rf $(wildcard $(src_O) $(out_LIB_JS) $(out_CLI))

lib: $(out_LIB_JS)

bin: $(out_CLI)

serve:
	npm run serve

# Generated files
src-c/insts.cc: tools/geninsts.py tools/riscvdata.py
	$< > $@
src-c/exec.cc: tools/genexec.py tools/riscvdata.py
	$< > $@

$(out_LIB_JS): $(src_EMCCO) src-c/pre.js
	@mkdir -p $(dir $@)
	$(EMCC) \
		$(EMCC_LDFLAGS) \
		-sMODULARIZE=1 -sEXPORT_NAME='loadlibcarve' \
		$(src_EMCCO) \
		--pre-js src-c/pre.js \
		-o $@

$(out_CLI): $(src_O) build/src-c/cli/main.o
	@mkdir -p $(dir $@)
	$(CXX) \
		$^ \
		-o $@

build/%.o: %.cc $(src_H)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Isrc-c $< -c -fPIC -o $@

build/%.emcco: %.cc $(src_H)
	@mkdir -p $(dir $@)
	$(EMCC) $(EMCC_CFLAGS) -Isrc-c $< -c -fPIC -o $@

