# t0.s - Basic example showing RISC-V code
#
# @author: Cade Brown <cade@utk.edu>


# t0 = 0
addi t0, zero, 0

# t1 = 5
addi t1, zero, 5

# do
cade:

    # t0++
    addi t0, t0, 1

    # while t0 < t1
    blt t0, t1, cade
