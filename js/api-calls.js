/**
 * Takes a hex string, returns disassembled instruction
 * @param {string} val - hex string with encoded instruction
 * @returns {string} disassembled instruction
 */
function dissassemble_hex(val) {
    let r = libcarve._carve_dissassemble(parseInt(val, 16));
    let out = libcarve.UTF8ToString(r);
    libcarve._free(r);
    return out;
}

/**
 * Takes an int, returns disassembled instruction
 * @param {number} val - integer with encoded instruction
 * @returns {string} disassembled instruction
 */
function dissassemble_int(val) {
    let r = libcarve._carve_dissassemble(val);
    let out = libcarve.UTF8ToString(r);
    libcarve._free(r);
    return out;
}

/**
 * Sets the program counter of the current state
 * @param {number} val - desired pc
 */
function set_pc(val) {
    libcarve._carve_set_pc(state, val);
    pc_prev = pc;
    pc = val;
    update_ui();
}

/**
 * Returns the program counter of current state
 * @returns {number} current pc
 */
function get_pc() {
    return libcarve._carve_get_pc(state);
}

/**
 * Sets the value of an integer register
 * @param {number} reg - destination register (as int)
 * @param {number} val - value to be set
 */
function set_intreg(reg, val) {
    libcarve._carve_set_xreg(state, reg, val);
    update_ui();
}

/**
 * Sets the value of an float register
 * @param {number} reg - destination register (as int)
 * @param {number} val - value to be set
 */
function set_floatreg(reg, val) {
    libcarve._carve_set_freg(state, reg, val);
    update_ui();
}

/**
 * Sets the value of a location in memory (single byte)
 * @param {number} loc - destination byte
 * @param {number} val - value to be set
 */
function set_byte(loc, val) {
    libcarve._carve_set_byte(state, loc, val);
    update_ui();
}