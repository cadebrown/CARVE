/* impl/RV32I.cc - Implementations of the RV32I functionality
 *
 * 
 * @author: Cade Brown <cade@utk.edu>
 */

#include <carve.hh>

namespace carve {
namespace impl {
    
void _lui    (State& s, int rd, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = imm;
}

void _auipc  (State& s, int rd, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = s.pc + imm;
}

void _jal    (State& s, int rd, u64 imm) {
    if (rd != 0) s.rx[rd] = s.pc;
    s.pc += sext(imm, 20);
}
void _jalr   (State& s, int rd, int rs1, u64 imm) {
    u64 rr = s.pc;
    s.pc = s.rx[rs1] + sext(imm, 19);
    if (rd != 0) s.rx[rd] = rr;

}
void _beq    (State& s, int rs1, int rs2, u64 imm) {
    if (s.rx[rs1] == s.rx[rs2]) {
        s.pc += sext(imm, 12);
    }
}
void _bne    (State& s, int rs1, int rs2, u64 imm) {
    if (s.rx[rs1] != s.rx[rs2]) {
        s.pc += sext(imm, 12);
    }
}
void _blt    (State& s, int rs1, int rs2, u64 imm) {
    if (s.rxs(rs1) < s.rxs(rs2)) {
        s.pc += sext(imm, 12);
    }
}
void _bge    (State& s, int rs1, int rs2, u64 imm) {
    if (s.rxs(rs1) >= s.rxs(rs2)) {
        s.pc += sext(imm, 12);
    }
}
void _bltu   (State& s, int rs1, int rs2, u64 imm) {
    if (s.rx[rs1] < s.rx[rs2]) {
        s.pc += sext(imm, 12);
    }
}
void _bgeu   (State& s, int rs1, int rs2, u64 imm) {
    if (s.rx[rs1] >= s.rx[rs2]) {
        s.pc += sext(imm, 12);
    }
}
void _lb     (State& s, int rd, int rs1, u64 imm) {
    s64 real = s.real(s.rx[rs1] + sext(imm, 11));
    s.rxs(rd) = sext(s.load<u8>(real), 7);
}
void _lh     (State& s, int rd, int rs1, u64 imm) {
    s64 real = s.real(s.rx[rs1] + sext(imm, 11));
    s.rxs(rd) = sext(s.load<u16>(real), 15);
}
void _lw     (State& s, int rd, int rs1, u64 imm) {
    s64 real = s.real(s.rx[rs1] + sext(imm, 11));
    s.rxs(rd) = sext(s.load<u32>(real), 31);
}
void _lbu    (State& s, int rd, int rs1, u64 imm) {
    s64 real = s.real(s.rx[rs1] + sext(imm, 11));
    s.rx[rd] = s.load<u8>(real);
}
void _lhu    (State& s, int rd, int rs1, u64 imm) {
    s64 real = s.real(s.rx[rs1] + sext(imm, 11));
    s.rx[rd] = s.load<u16>(real);
}
void _sb     (State& s, int rs1, int rs2, u64 imm) {
    s64 real = s.real(s.rx[rs1] + sext(imm, 11));
    s.load<u8>(real) = s.rx[rs2];
}
void _sh     (State& s, int rs1, int rs2, u64 imm) {
    s64 real = s.real(s.rx[rs1] + sext(imm, 11));
    s.load<u16>(real) = s.rx[rs2];
}
void _sw     (State& s, int rs1, int rs2, u64 imm) {
    s64 real = s.real(s.rx[rs1] + sext(imm, 11));
    s.load<u32>(real) = s.rx[rs2];
}
void _addi   (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rxs(rd) = s.rxs(rs1) + sext(imm, 11);
}
void _slti   (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rxs(rd) = s.rxs(rs1) < sext(imm, 11);
}
void _sltiu  (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] < (s64)sext(imm, 11);
}
void _xori   (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] ^ imm;
}
void _ori    (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] | imm;
}
void _andi   (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] & imm;
}
void _slli   (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] << imm;
}
void _srli   (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] >> imm;
}
void _add    (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = s.rxs(rs1) + s.rxs(rs2);
}
void _sub    (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = s.rxs(rs1) - s.rxs(rs2);
}
void _sll    (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = s.rxs(rs1) << s.rxs(rs2);
}
void _srl    (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = s.rxs(rs1) >> s.rxs(rs2);
}
void _sra    (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = s.rxs(rs1) / ((s64)1 << s.rxs(rs2));
}
void _slt    (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = s.rxs(rs1) < s.rxs(rs2);
}
void _sltu   (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] < s.rx[rs2];
}
void _xor    (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] ^ s.rx[rs2];
}
void _or     (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] | s.rx[rs2];
}
void _and    (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rx[rd] = s.rx[rs1] & s.rx[rs2];
}

// Syscalls
void _ebreak (State& s, int rd, int rs1, u64 imm) {
    switch (imm)
    {
    case 0:
        // ecall
        break;
    case 1:
        // ebreak
        s.is_halted = true;
        break;
    default:
        break;
    }
}

}
}
