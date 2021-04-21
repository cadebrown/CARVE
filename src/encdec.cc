/* encdec.cc - Encoding and decoding functions for instruction
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.hh"

namespace carve {


/* Utilities */


// Create a mask from bit '_i' (inclusive) to bit '_j' (exclusive)
#define MASK(_i, _j) (((1ULL << ((_j) - (_i))) - 1) << (_i))

// Treat '_val' as a field, and shift it and mask it to start at bit '_i' (inclusive) 
//   and end at bit '_j' (exclusive)
#define FIELD(_val, _i, _j) (((_val) & MASK(0, (_j) - (_i))) << (_i))

// Get field from bit '_i' (inclusive) to bit '_j' (exclusive)
#define UNFIELD(_val, _i, _j) (((_val) & MASK(_i, _j)) >> (_i))


inst enc_R(u64 op, u64 f3, u64 f7, int rd, int rs1, int rs2) {
    return 0
        | FIELD(op, 0, 7)
        | FIELD(rd, 7, 12)
        | FIELD(f3, 12, 15)
        | FIELD(rs1, 15, 20)
        | FIELD(rs2, 20, 25)
        | FIELD(f7, 25, 32)
    ;
}
inst enc_I(u64 op, u64 f3, int rd, int rs1, u64 imm) {
    return 0
        | FIELD(op, 0, 7)
        | FIELD(rd, 7, 12)
        | FIELD(f3, 12, 15)
        | FIELD(rs1, 15, 20)
        | FIELD(imm, 20, 32)
    ;
}
inst enc_U(u64 op, int rd, u64 imm) {
    return 0
        | FIELD(op, 0, 7)
        | FIELD(rd, 7, 12)
        | FIELD(imm >> 12, 12, 32)
    ;
}
inst enc_J(u64 op, int rd, u64 imm) {
    return 0
        | FIELD(op, 0, 7)
        | FIELD(rd, 7, 12)
        | FIELD((MASK(12, 20) & imm) >> 12, 12, 20)
        | FIELD((MASK(11, 12) & imm) >> 11, 20, 21)
        | FIELD((MASK(1, 11) & imm) >> 1, 21, 31)
        | FIELD((MASK(20, 21) & imm) >> 20, 31, 32)
    ;
}
inst enc_S(u64 op, u64 f3, int rs1, int rs2, u64 imm) {
    return 0
        | FIELD(op, 0, 7)
        | FIELD((MASK(0, 5) & imm), 7, 12)
        | FIELD(f3, 12, 15)
        | FIELD(rs1, 15, 20)
        | FIELD(rs2, 20, 25)
        | FIELD((MASK(5, 12) & imm) >> 5, 25, 32)
    ;
}
inst enc_B(u64 op, u64 f3, int rs1, int rs2, u64 imm) {
    return 0
        | FIELD(op, 0, 7)
        | FIELD((MASK(11, 12) & imm) >> 11, 7, 8)
        | FIELD((MASK(1, 5) & imm) >> 1, 8, 12)
        | FIELD(f3, 12, 15)
        | FIELD(rs1, 15, 20)
        | FIELD(rs2, 20, 25)
        | FIELD((MASK(5, 11) & imm) >> 5, 25, 31)
        | FIELD((MASK(12, 13) & imm) >> 12, 31, 32)
    ;
}
inst enc_y(u64 op, u64 f3, int rd, int rs1, u64 imm) {
    return 0
        | FIELD(op, 0, 7)
        | FIELD(rd, 7, 12)
        | FIELD(f3, 12, 15)
        | FIELD(rs1, 15, 20)
        | FIELD(imm, 20, 32)
    ;
}

void dec_R(inst v, u64& op, u64& f3, u64& f7, int& rd, int& rs1, int& rs2) {
    op = UNFIELD(v, 0, 7);
    f3 = UNFIELD(v, 12, 15);
    f7 = UNFIELD(v, 25, 32);
    rd = UNFIELD(v, 7, 12);
    rs1 = UNFIELD(v, 15, 20);
    rs2 = UNFIELD(v, 20, 25);
}
void dec_I(inst v, u64& op, u64& f3, int& rd, int& rs1, u64& imm) {
    op = UNFIELD(v, 0, 7);
    f3 = UNFIELD(v, 12, 15);
    rd = UNFIELD(v, 7, 12);
    rs1 = UNFIELD(v, 15, 20);
    imm = UNFIELD(v, 20, 32);
}
void dec_U(inst v, u64& op, int& rd, u64& imm) {
    op = UNFIELD(v, 0, 7);
    rd = UNFIELD(v, 7, 12);
    imm = UNFIELD(v, 12, 32) << 12;
}
void dec_J(inst v, u64& op, int& rd, u64& imm) {
    op = UNFIELD(v, 0, 7);
    rd = UNFIELD(v, 7, 12);

    // Decode parts of the immediate
    u64 t0 = UNFIELD(v, 21, 31);
    u64 t1 = UNFIELD(v, 20, 21);
    u64 t2 = UNFIELD(v, 12, 20);
    u64 t3 = UNFIELD(v, 31, 32);

    // Combine parts
    imm = (t0 << 1) | (t1 << 11) | (t2 << 12) | (t3 << 20);

}
void dec_S(inst v, u64& op, u64& f3, int& rs1, int& rs2, u64& imm) {
    op = UNFIELD(v, 0, 7);
    f3 = UNFIELD(v, 12, 15);
    rs1 = UNFIELD(v, 15, 20);
    rs2 = UNFIELD(v, 20, 25);

    // Decode parts of the immediate
    u64 t0 = UNFIELD(v, 7, 12);
    u64 t1 = UNFIELD(v, 25, 32);

    // Combine parts
    imm = (t0) | (t1 << 5);
}
void dec_B(inst v, u64& op, u64& f3, int& rs1, int& rs2, u64& imm) {
    op = UNFIELD(v, 0, 7);
    f3 = UNFIELD(v, 12, 15);
    rs1 = UNFIELD(v, 15, 20);
    rs2 = UNFIELD(v, 20, 25);

    // Decode parts of the immediate
    u64 t0 = UNFIELD(v, 8, 12);
    u64 t1 = UNFIELD(v, 25, 31);
    u64 t2 = UNFIELD(v, 7, 8);
    u64 t3 = UNFIELD(v, 31, 32);

    // Combine parts
    imm = (t0 << 1) | (t1 << 5) | (t2 << 11) | (t3 << 12);
}
void dec_y(inst v, u64& op, u64& f3, int& rd, int& rs1, u64& imm) {
    op = UNFIELD(v, 0, 7);
    f3 = UNFIELD(v, 12, 15);
    rd = UNFIELD(v, 7, 12);
    rs1 = UNFIELD(v, 15, 20);
    imm = UNFIELD(v, 20, 32);
}

}
