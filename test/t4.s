# t4.s - Infinite loop
#
# @author: Cade Brown <cade@utk.edu>


# t0 = 0
addi t0, zero, 0

# do
cade:

    # t0++
    addi t0, t0, 1

    # while true
    j cade
