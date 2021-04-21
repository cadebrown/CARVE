/* capi.cc - Implementation of the CARVE C API
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.hh"


namespace carve {

void carve_init() {
    cout << "CARVE initialized" << endl;
}


State* carve_state_new() {
    return new State();
}

void carve_state_free(State* self) {
    delete self;
}

Program* carve_program_new(const char* fname, const char* src) {
    string fname_s = fname;
    string src_s = src;
   
    // Lex source code
    vector<Token> toks = lex(fname_s, src_s);
    if (toks.size() == 0) {
        return NULL;
    }

    // Parse source code
    Program* res = parse(fname_s, src_s, toks);
    if (!res) {
        return NULL;
    }

    // Otherwise, everything worked
    return res;
}

void carve_program_free(Program* self) {
    delete self;
}

void carve_exec_single(State* s) {
    // TODO: How to handle halted program
    if (s->is_halted) {
        return;
    }

    // Now, first, we need to get the current instruction
    inst v = s->load<inst>(s->pc);

    // And execute in the state
    s->exec(v);

    // Increment to next instruction
    s->pc += 4;
}

void carve_exec_all(State* s) {
    while (!s->is_halted) {
        carve_exec_single(s);
    }
}


void carve_getrx(State* s, int len, char* data, int reg, int base) {
    if (base == 10) {
        snprintf(data, len, "%lli", (long long int)s->rx[reg]);
    } else {
        snprintf(data, len, "%llx", (long long int)s->rx[reg]);
    }
}

void carve_getrf(State* s, int len, char* data, int reg) {
    snprintf(data, len, "%lf", (double)s->rf[reg]);
}

void carve_state_init(State* self, Program* program) {
    self->init(*program);
}


}
