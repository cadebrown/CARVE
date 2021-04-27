/* impl/RV64I.cc - Implementations of the RV64I functionality
 *
 * 
 * @author: Cade Brown <cade@utk.edu>
 */

#include <carve.hh>

namespace carve {
namespace impl {
    
void _addiw  (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rxs(rd) = sext((s32)s.rxs(rs1) + sext(imm, 11), 31);
}

void _slliw  (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = sext((s64)(s.rx[rs1]) << imm, 31);
}
void _srliw  (State& s, int rd, int rs1, u64 imm) {
    if (rd == 0) return;
    s.rx[rd] = sext((s64)s.rx[rs1] >> imm, 31);
}

void _addw   (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = sext(s32(s.rxs(rs1) + s.rxs(rs2)), 31);
}

void _subw   (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = sext(s.rxs(rs1) - s.rxs(rs2), 31);
}

void _sllw   (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = sext(s.rxs(rs1) << s.rxs(rs2), 31);
}

void _srlw   (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = sext(s.rxs(rs1) >> s.rxs(rs2), 31);
}

void _sraw  (State& s, int rd, int rs1, int rs2) {
    if (rd == 0) return;
    s.rxs(rd) = sext(s32(s.rxs(rs1)) / ((s32)1 << s.rxs(rs2)), 31);
}

}
}
