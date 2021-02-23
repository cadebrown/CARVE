/* ext/template.h - template extension
 * 
 * This is an example of an extension implementation.
 * 
 * Here are the important terms and concepts to understand
 * in order to implement your instructions:
 * 
 * RISC-V TERMS
 * rd:  Destination Register (as number)
 * rs1: Register Source 1    (as number)
 * rs2: Register Source 2    (as number)
 * imm: Immediate value      (as value)
 * 
 * CARVE IMPLEMENTATION TERMS
 * REGU(r):   Accesses to register number r as a modifiable unsigned value
 * REGS(r):   Accesses to register number r as a modifiable signed value
 * PC:        Program Counter as a modifiable value
 * HALT(msg): Halts the execution of the program and prints msg to the console
 *
 * CARVE is implemented through WebASM, so if you want to print messages to
 * the console, you can use console i/o ops (printf, scanf, etc.) as usual
 * and the user can participate through the js console on the webpage.
 * 
 * @author: Gregory Croisdale <greg@kscript.org>
 */

#ifndef EXT_EXTENSIONNAME_H__
#define EXT_EXTENSIONNAME_H__

/*
This is where you implement the instructions. Instructions are
implemented as macros that will be used in the execution cycle's
switch case loop. Because of this, make sure to include a backslash
at the end of every newline. Depending on the type of instruction
you are implementing, the prototype will look different. The default
instruction types areas follows:

R TYPE: Register type
CARVE_instname(rd, rs1, rs2) { \
   implementation \
}

I TYPE: Immediate type (immediate only bits 0 to 11)
CARVE_instname(rd, rs1, imm) { \
   implementation \
}

S TYPE: Store type (immediate only bits 0 to 11)
CARVE_instname(rs1, rs2, imm) { \
   implementation \
}

B TYPE: Branch type (immediate only bits 1 to 12 -- generally labels)
CARVE_instname(rs1, rs2, imm) { \
   implementation \
}

U TYPE: Upper Immediate type (immediate only bits 12 to 32)
CARVE_instname(rd, imm) { \
    implementation \
}

J TYPE: Jump type (immediate only bits 12 to 32 -- generally labels)
CARVE_instname(rd, imm) { \
    implementation \
}

The last type is unique to the CARVE implementation

y TYPE: 0 arg syscall (relies on register values)
CARVE_instname() { \
    implementation \
}
*/

#endif /* EXT_EXTENSIONNAME_H__ */
