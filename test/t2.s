# t2.s - Floating point numbers
#
# @author: Cade Brown <cade@utk.edu>

la x4, msg 
lw x5, 0(x4)


.section rodata

msg:
.float 1.0
