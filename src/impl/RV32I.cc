/* impl/RV32I.cc - Implementations of the RV32I functionality
 *
 * 
 * @author: Cade Brown <cade@utk.edu>
 * @author: Gregory Croisdale <gcroisda@vols.utk.edu>
 */

#include <carve.hh>
#include <time.h>
#include <stdlib.h>

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
    s.pc += sext(imm, 20) - 4;
}
void _jalr   (State& s, int rd, int rs1, u64 imm) {
    u64 rr = s.pc;
    s.pc = s.rx[rs1] + sext(imm, 19) - 4;
    if (rd != 0) s.rx[rd] = rr;

}
void _beq    (State& s, int rs1, int rs2, u64 imm) {
    if (s.rx[rs1] == s.rx[rs2]) {
        s.pc += sext(imm, 12) - 4;
    }
}
void _bne    (State& s, int rs1, int rs2, u64 imm) {
    if (s.rx[rs1] != s.rx[rs2]) {
        s.pc += sext(imm, 12) - 4;
    }
}
void _blt    (State& s, int rs1, int rs2, u64 imm) {
    if (s.rxs(rs1) < s.rxs(rs2)) {
        s.pc += sext(imm, 12) - 4;
    }
}
void _bge    (State& s, int rs1, int rs2, u64 imm) {
    if (s.rxs(rs1) >= s.rxs(rs2)) {
        s.pc += sext(imm, 12) - 4;
    }
}
void _bltu   (State& s, int rs1, int rs2, u64 imm) {
    if (s.rx[rs1] < s.rx[rs2]) {
        s.pc += sext(imm, 12) - 4;
    }
}
void _bgeu   (State& s, int rs1, int rs2, u64 imm) {
    if (s.rx[rs1] >= s.rx[rs2]) {
        s.pc += sext(imm, 12) - 4;
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
    #define a(n)  s.rx[10 + n]
    #define fa(n) s.rf[10 + n]
    switch (imm)
    {
    case 0: {
        // ecall (syscall)
        // Which syscall?
        switch (a(7))
        {
            // basic proc calls
            case 0: {
                // exit(0)
                s.is_exited = true;
                s.exit_code = 0;
                break;
            }
            case 1: {
                // exit(a0)
                s.is_exited = true;
                s.exit_code = a(0);
                break;
            }

            // I/O
            // Input
            case 10: {
                // readchar
                char tmp;

                if ((scanf("%c", (char*) &tmp)) != 1) {
                    fprintf(stderr, "(KNOWN ISSUE WITH EMCC) Syscall failed -- got errno %d\n", errno);
                }

                a(0) = tmp;
                break;
            }
            case 11: {
                // readint
                uint32_t tmp;

                if ((scanf("%d", (int*) &tmp)) != 1) {
                    fprintf(stderr, "(KNOWN ISSUE WITH EMCC) Syscall failed -- got errno %d\n", errno);
                }

                a(0) = tmp;
                break;
            }
            case 12: {
                // readflt
                double tmp;

                if ((scanf("%lf", (double*) &tmp)) != 1) {
                    fprintf(stderr, "(KNOWN ISSUE WITH EMCC) Syscall failed -- got errno %d\n", errno);
                }

                fa(0) = tmp;
                break;
            }
            case 13: {
                // readlong
                uint64_t tmp;

                if ((scanf("%ld", (long*) &tmp)) != 1) {
                    fprintf(stderr, "(KNOWN ISSUE WITH EMCC) Syscall failed -- got errno %d\n", errno);
                }

                a(0) = tmp;
                break;
            }
            case 14: {
                // readstr
                char buff[a(1)];

                if (fgets(buff, a(1), stdin) == nullptr) {
                    fprintf(stderr, "(KNOWN ISSUE WITH EMCC) Syscall failed -- got errno %d\n", errno);
                }

                memcpy(((char*) s.vmem.data()) + a(0), buff, a(1));
                break;
            }
            // Output
            case 20: {
                // writechar
                printf("%c", (char) a(0));
                fflush(stdout);
                break;
            }
            case 21: {
                // writeint
                printf("%d", (int) a(0));
                fflush(stdout);
                break;
            }
            case 22: {
                // writelong
                printf("%ld", (long) a(0));
                fflush(stdout);
                break;
            }
            case 23: {
                // writeflt
                printf("%lf", (double) fa(0));
                fflush(stdout);
                break;
            }
            case 24: {
                // writestr
                printf("%.*s", (int) a(1), ((char*) s.vmem.data()) + a(0));
                fflush(stdout);
                break;
            }

            // Time operations
            case 30: {
                // time
                a(0) = (u64) time(NULL);
                break;
            }

            // Random
            case 41: {
                // randint
                a(0) = (u64) rand();
                break;
            }
            case 43: {
                // randflt
                fa(0) = ((double) rand()) / RAND_MAX;
                break;
            }
            case 45: {
                // seed random
                srand((int) a(0));
                break;
            }
            default: {
                fprintf(stderr, "Unknown syscall number %d\n", (int) a(7));
            }
        }

        break;
    } 
    case 1: {
        // ebreak
        s.is_halted = true;
        break;
    }


    default:
        break;
    }
}

}
}
