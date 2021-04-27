---
layout: doc
title: CARVE
desc: Developer Guide
permalink: /dev
---

# CARVE Development Guide

This is the developer's guide to CARVE, walking through how to contribute and build CARVE. The source code and development for CARVE are located on the GitHub: [https://github.com/chemicaldevelopment/carve](https://github.com/chemicaldevelopment/carve). Within that repository, the code is commented so you can see how it works and what you need to change (also, check `README.md` in that repository for more information). This is a higher level overview of the development process

## Building

Once you've cloned the repository, you can use `make` to run various targets. For example, running `make update` will recompile the `./carve` binary (which runs on your local machine) as well as the JS/WASM package in `docs/js/lib`. You should re-run this every time the code has changed.

The build process (internally) has a few stages:

  * Code generators (located in `tools/gen*.py`), which generate C++ code, mainly for encoding/decoding instructions (so that new instructions can be added)
    * To change the data used to generate code, edit the file `tools/riscvdata.py`. There are comments explaining what to do to add extensions/instructions
  * Native C++ compiling, which compiles object files for your computer
  * WASM compiling with Emscripten, which compiles JS/WASM for C++ code
    * You may get an error related to `emcc not found`. In which case, you will need to google how to install and use Emscripten on your computer

## Running Binary

You can run the main binary with `./carve` (once it is built). Right now, it just runs the program and dumps the registers afterwards. Eventually, we aim to add a way to control it for automated grading

## Interfacing Through JS/WASM

The CARVE API can be accessed through JS/WASM. See `src/js/carve.js` for examples on how to do this. Basically, the idea is to create a very simple C-style API which just accepts pointers (i.e. no C++ STL containers, or normal objects), so the API is as resiliant as possible. Then, on the JS side, you keep pointers to the state, program, and so forth, and use the API to manipulate them


## Serving With Jekyll

We use Jekyll (a static site generator) to build the web IDE for CARVE (the main usage of the project). Within the `docs/` directory, first run `bundle install` (assuming you have installed Jekyll), and then run `bundle exec jekyll serve`. It should print the local URL that you can access it on your computer.

It is designed to work with GitHub pages, which we use to deploy it at the main URL: [https://carve.chemicaldevelopment.us/](https://carve.chemicaldevelopment.us/)

## Changing Code

In the case that you want to change code, there are a few locations to check:

  * The core library is located in `src/*.cc`
  * The C-API is located in `src/capi.cc`
  * The actual implementations of instructions are located in `src/impl/*.cc`
  * The code generators are located in `src/gen*.py`
  * The website (i.e. the web IDE) is located in `docs/`






