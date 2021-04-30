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

Here's a short table (the names are links):

| Name                       | Number |
|----------------------------|--------|
| [exit](#exit-0)            |      0 |
| [exit_code](#exit_code-1)  |      1 |
| [readchar](#readchar-10)   |     10 |
| [readint](#readint-11)     |     11 |
| [readflt](#readflt-12)     |     12 |
| [readlong](#readlong-13)   |     13 |
| [readstr](#readstr-14)     |     14 |
| [writechar](#writechar-20) |     20 |
| [wrriteint](#wrriteint-21) |     21 |
| [writelong](#writelong-22) |     22 |
| [writeflt](#writeflt-23)   |     23 |
| [writestr](#writestr-24)   |     24 |
| [time](#time-30)           |     30 |
| [randint](#randint-41)     |     41 |
| [randflt](#randflt-43)     |     43 |
| [seed](#seed-45)           |     45 |

# PROC SYSCALLS

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

# READ SYSCALLS
NOTE: These syscalls rely on the emcc buffer which has some known issues with flushing.
Sometimes, you have to give all of the input for the program execution before building for the first time.

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

Reads a string from stdin until either a newline or `a1` characters are read, storing the characters at the address in `a0` (null terminated).

```
# sycall: readstr (14)
# assume text "greg" is in buffer

.section .data
dest: .asciz "cade is cool"

.section .text
li a7, 14

# set `a0` to address of memory
la a0, dest

# set `a1` to maximum size
li a1, 5

# Do syscall
ecall

# replace null with space
li t0, ' '
sb t0, 4(a0)

# Print result
li a7, 24
la a0, test
li a1, 512
ecall
# flush buffer
li a7, 20
li a0, '\n'
ecall

```

NOTE: Like other I/O, this is somewhat buggy with Emscripten. For best results, enter the text before you run the program

# WRITE SYSCALLS

NOTE: Output syscalls are more stable than input syscalls, but you may have to manually flush the buffer by printing the newline character.

## writechar (`20`)

Writes a single character (value from `a0`) to standard out

```

# sycall: writechar (20)
li a7, 20

# char to be printed
li a0, '!'

# Do syscall
ecall

# flush buffer
li a7, 20
li a0, '\n'
ecall

```

## writeint (`21`)

Writes a single integer (value from `a0`) to standard out

```

# sycall: writeint (21)
li a7, 21

# int to be printed
li a0, 420

# Do syscall
ecall

# flush buffer
li a7, 20
li a0, '\n'
ecall

```

## writelong (`22`)

Writes a single long (value from `a0`) to standard out

```

# sycall: writelong (22)
li a7, 22

# long to be printed
li a0, 142434245

# Do syscall
ecall

# flush buffer
li a7, 20
li a0, '\n'
ecall

```

## writeflt (`23`)

Writes a single float (value from `fa0`) to standard out

```

# sycall: writeflt (23)
li a7, 23

# get float into fa0

# Do syscall
ecall

# flush buffer
li a7, 20
li a0, '\n'
ecall

```

## writestr (`24`)

Writes a string (address from `a0`, max chars from `a1`) to standard out -- can be null-terminated or terminated by `a1`

```
# sycall: writestr (24)

.section .rodata
test: .asciz "Marz, not Mars."

.section .text
li a7, 24

# load address
la a0, test

# load max characters
li a1, 512

# Do syscall
ecall

# flush buffer
li a7, 20
li a0, '\n'
ecall

```

# TIME SYSCALLS

## time (`30`)

Sets `a0` to the current time since epoch in seconds

```
# sycall: time (30)

li a7, 30
ecall

# print result
li a7, 21
ecall

# flush buffer
li a7, 20
li a0, '\n'
ecall
```

# RANDOM SYSCALLS

## randint (`41`)

Generates a random integer between 0 and C's RAND_MAX and stores to `a0`

```
# sycall: randint (41)

li a7, 41
ecall

# print result
li a7, 21
ecall

# flush buffer
li a7, 20
li a0, '\n'
ecall
```

## randflt (`43`)

Generates a random float between 0 and 1 and stores to `fa0`

```
# sycall: randflt (43)

li a7, 43
ecall

# print result
li a7, 23
ecall

# flush buffer
li a7, 20
li a0, '\n'
ecall
```


## seed (`45`)

Seeds the random number generator using the value in `a0`

```
# sycall: seed (45)

# set loop params
li t0, 0
li t1, 3

loop:
# set seed
li a0, 0xDEAD_CADE
li a7, 45
ecall

# print random int
li a7, 41
ecall
li a7, 21
ecall
li a7, 20
li a0, '\n'
ecall
addi t0, t0, 1
blt t0, t1, loop
```