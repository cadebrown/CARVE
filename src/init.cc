/* init.cc - libcarve initialization and general routines
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.hh"

namespace carve {

s64 sext(u64 val, int bit) {
    u64 bm = 1ULL << bit;
    if (val & bm) {
        // Has a sign bit, so set all upper bits to 1
        return val | ~((1ULL << bit) - 1);
    } else {
        // No sign bit
        return val;
    }
}


}
