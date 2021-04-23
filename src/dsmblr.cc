/* exec.cc - GENERATED - Execution implementation
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT **
 *
 * @author: Gregory Croisdale <gc@utk.edu>
 */

#include <carve.hh>
#include <sstream>

namespace carve {



string regnames[] = {
    "zero",
    "ra", 
    "sp", 
    "gp", 
    "tp", 
    "t0", 
    "t1", 
    "t2", 
    "s0", 
    "fp", 
    "s1", 
    "a0", 
    "a1", 
    "a2", 
    "a3", 
    "a4", 
    "a5", 
    "a6", 
    "a7", 
    "s2", 
    "s3", 
    "s4", 
    "s5", 
    "s6", 
    "s7", 
    "s8", 
    "s9", 
    "s10",
    "s11",
    "t3", 
    "t4", 
    "t5", 
    "t6",
};

string disassemble(inst v) {
    u64 op, f3, f7, imm;
    int rd, rs1, rs2;
    std::ostringstream out;
    // Decode opcode
    switch (op = v & 0x7F) {
        case 0x37:
            dec_U(v, op, rd, imm);
                            out << "lui" << " " << regnames[rd] << ", " << imm;
            break;
        case 0x17:
            dec_U(v, op, rd, imm);
                            out << "auipc" << " " << regnames[rd] << ", " << imm;
            break;
        case 0x6f:
            dec_J(v, op, rd, imm);
                            out << "jal" << " " << regnames[rd] << ", " << imm;
            break;
        case 0x67:
            dec_I(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                            out << "jalr" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
            }
            break;
        case 0x63:
            dec_B(v, op, f3, rs1, rs2, imm);
            switch (f3) {
                case 0x0:
                            out << "beq" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
                case 0x1:
                            out << "bne" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
                case 0x4:
                            out << "blt" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
                case 0x5:
                            out << "bge" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
                case 0x6:
                            out << "bltu" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
                case 0x7:
                            out << "bgeu" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
            }
            break;
        case 0x3:
            dec_I(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                            out << "lb" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x1:
                            out << "lh" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x2:
                            out << "lw" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x4:
                            out << "lbu" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x5:
                            out << "lhu" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
            }
            break;
        case 0x23:
            dec_S(v, op, f3, rs1, rs2, imm);
            switch (f3) {
                case 0x0:
                            out << "sb" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
                case 0x1:
                            out << "sh" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
                case 0x2:
                            out << "sw" << " " << regnames[rs1] << ", " << regnames[rs2] << ", " << imm;
                    break;
            }
            break;
        case 0x13:
            dec_I(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                            out << "addi" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x2:
                            out << "slti" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x3:
                            out << "sltiu" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x4:
                            out << "xori" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x6:
                            out << "ori" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x7:
                            out << "andi" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x1:
                            out << "slli" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
                case 0x5:
                            out << "srli" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
            }
            break;
        case 0x33:
            dec_R(v, op, f3, f7, rd, rs1, rs2);
            switch (f3) {
                case 0x0:
                    switch (f7) {
                        case 0x0:
                            out << "add" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                        case 0x20:
                            out << "sub" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                    }
                    break;
                case 0x1:
                    switch (f7) {
                        case 0x0:
                            out << "sll" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                    }
                    break;
                case 0x2:
                    switch (f7) {
                        case 0x0:
                            out << "slt" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                    }
                    break;
                case 0x3:
                    switch (f7) {
                        case 0x0:
                            out << "sltu" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                    }
                    break;
                case 0x4:
                    switch (f7) {
                        case 0x0:
                            out << "xor" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                    }
                    break;
                case 0x5:
                    switch (f7) {
                        case 0x0:
                            out << "srl" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                        case 0x10:
                            out << "sra" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                    }
                    break;
                case 0x6:
                    switch (f7) {
                        case 0x0:
                            out << "or" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                    }
                    break;
                case 0x7:
                    switch (f7) {
                        case 0x0:
                            out << "and" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << rs2;
                            break;
                    }
                    break;
            }
            break;
        case 0x73:
            dec_y(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                            out << "ebreak" << " " << regnames[rd] << ", " << regnames[rs1] << ", " << imm;
                    break;
            }
            break;

    }
    return out.str();
}


}

