
# CARVE

CARVE (Cade Andgreg's Risc-V Emulator) is a project developed under [Dr. Marz](https://www.eecs.utk.edu/people/stephen-marz/). CARVE is deployable as a web application, and is hosted at (WIP -- it isn't hosted yet. Set the link here). It has been tested with many platforms, and relies only on a modern web browser with a modern JavaScript engine and [Wasm](https://webassembly.org/) support.

## Building

This section is not relevant for most people -- it doesn't need to be built by end users. But, if you want to build a custom version, or you're writing the project, this is for you.

You should install the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html). Before you run any commands below, you'll probably have to activate it:

```shell
$ source "/path/to/emsdk/emsdk_env.sh"
```

(replacing `/path/to` with the path to your Emscripten SDK)

Now, to build `libcarve`, which is the C bindings and emulator, you can run:

```shell
$ make
```

## Running

First, [install jekyll](https://jekyllrb.com/docs/installation/) to run locally

To run the application, you'll need to use the Jekyll website in `docs/`. If you're updating the `libcarve`, make sure to copy `libcarve.js` and `libcarve.wasm` from `build` to `docs/lib` to update  (`cp build/libcarve.{js,wasm} docs/lib`)

Then, you can run `bundle exec jekyll serve` to host it locally

