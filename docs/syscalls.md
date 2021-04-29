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

The exit syscall (number `0`) causes the program to cease executing. The exit code is provided in `a0` (`0` means success)

Example:

```
# syscall: exit (0)
li a7, 0

# 0: success
li a0, 0

# Do syscall
ecall

```



