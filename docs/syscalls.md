---
layout: doc
title: CARVE
desc: Syscall Referece
permalink: /syscalls
---

# CARVE Syscall Reference

To perform a syscall, here is the basic format:

```

# syscall number goes into a7
li a7, 0

# syscall arguments go in a0, a1, ...
li a0, 1
li a1, 43

# Actually performs syscall
ecall

```

Each syscall does something different, and this page has a list of them:

## exit (`0`)

The exit syscall (number `0`) causes the program to cease executing. It uses an exit code of `0` (see `exit_code`, down below)

Example:

```
# syscall: exit (0)
li a7, 0

# Do syscall
ecall

```
## exit_code (`1`)

The exit syscall (number `1`) causes the program to cease executing. It uses an exit code of whatever is in `a0`

Example:

```
# syscall: exit_code (1)
li a7, 1

# exit code goes in 
li a0, 5

# Do syscall
ecall

```

## readchar (`10`)

This syscall reads a single character from standard input, and stores in `a0`


```

# sycall: readchar (10)
li a7, 10

# Do syscall
ecall

# Now, the character code is in `a0`

```

NOTE: Like other I/O, this is somewhat buggy with Emscripten. For best results, enter the text before you run the program


## readint (`11`)

This syscall reads a base-10 integer from standard input, and stores in `a0`


```

# sycall: readint (11)
li a7, 11

# Do syscall
ecall

# Now, the integer value is in `a0`

```

NOTE: Like other I/O, this is somewhat buggy with Emscripten. For best results, enter the text before you run the program

## readflt (`12`)

This syscall reads a base-10 floating point number from standard input, and stores in `fa0`

```

# sycall: readflt (12)
li a7, 12

# Do syscall
ecall

# Now, the integer value is in `fa0`

```

NOTE: Like other I/O, this is somewhat buggy with Emscripten. For best results, enter the text before you run the program

## readlong (`13`)

This syscall reads a base-10 integer number from standard input, and stores in `a0`

```

# sycall: readlong (13)
li a7, 13

# Do syscall
ecall

# Now, the integer value is in `a0`

```

NOTE: Like other I/O, this is somewhat buggy with Emscripten. For best results, enter the text before you run the program


## readstr (`14`)

Reads a string from stdin until either a newline or `a1` characters are read, storing the characters at the address in `a0` (null terminated)

```

# sycall: readstr (14)
li a7, 14

# somehow, set `a0` to address of memory

# maximum size
li a1, 25

# Do syscall
ecall

```

NOTE: Like other I/O, this is somewhat buggy with Emscripten. For best results, enter the text before you run the program



## writechar (`20`)

Writes a single character (value from `a0`) to standard out

```

# sycall: writechar (20)
li a7, 20

# somehow, set `a0` to address of memory

# maximum size
li a1, 25

# Do syscall
ecall

```






