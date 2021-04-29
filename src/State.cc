/* State.cc - implementation of 'carve::State', which is the virtual machine implementation
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.hh"

namespace carve {


void State::init(const Program& prog, u64 extra) {
    vmem.clear();
    // Insert segments

    // Start program counter at the text section
    pc = vmem.size();
    for (int i = 0; i < 4; ++i) {
        vmem.insert(vmem.end(), prog.vmem[i].begin(), prog.vmem[i].end());
    }

    // Now, append 'extra' bytes, initialized to zero
    vmem.resize(vmem.size() + extra);

    // Zero out registers
    for (int i = 0; i < NUM_RX; ++i) {
        rx[i] = 0;
    }
    for (int i = 0; i < NUM_RF; ++i) {
        rf[i] = 0.0;
    }

    // Set the zero register (zero) to zero
    rx[0] = 0;

    // Set stack pointer register (sp) to the top of the vmem space
    rx[2] = fake(vmem.size());

    // Unhalt / unexit program
    is_halted = false;
    is_exited = false;
    exit_code = -1;
}


}
