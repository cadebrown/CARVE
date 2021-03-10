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
 * REGU(r):         Accesses to register number r as a modifiable unsigned value
 * REGS(r):         Accesses to register number r as a modifiable signed value
 * PC:              Program Counter as a modifiable value
 * HALT(msg):       Halts the execution of the program and prints msg to the console
 * CARVE_SEXT(imm): Sign extends an immediate value
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


This is where you implement the pseudoinstructions. Psuedoinstructions
are also implemented as macros, but these are used to parse the instruction's
arguments. Pseudoinstructions must translate directly to already existing
instructions. For more reference, take a look at the pseudoinstructions
implemented near the bottom of RV32I.h.

#define CARVE_PSEUDO_pseudoinstruction() do { \
   ! comment describing translation

   ! parse arguments (examples below:)
   int rd, imm; \
   if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
      return false; \
   } \

   ! make sure to skip expected commas!
   if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
      return false; \
   } \

   if (!parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'I', &imm)) { \
        return false; \
   } \

   ! find and add desired instruction information ()
   struct carve_instdesc* inst = carve_getinst("instruction", -1); \
   carve_prog_add(prog, carve_makeTYPE(inst->opcode, inst->f3, inst->f7, 0, rs, imm)); \
} while (0)

*/

#endif /* EXT_EXTENSIONNAME_H__ */
