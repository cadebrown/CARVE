# t0.s - Basic example showing RISC-V code
#
# @author: Cade Brown <cade@utk.edu>


.section text

la x5, msg

ecall


.section rodata

msg:
.asciz "XXXXXXXXXXXXXXXXXXXXXXXX"

