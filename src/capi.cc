/* capi.cc - Implementation of the CARVE C API
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.hh"
#include <sstream>

namespace carve {

void carve_init() {
}


State* carve_state_new() {
    return new State();
}

void carve_state_free(State* self) {
    delete self;
}

Program* carve_program_new(const char* fname, const char* src, bool addexit) {
    string fname_s = fname;
    string src_s = src;
   
    // Lex source code
    vector<Token> toks = lex(fname_s, src_s);
    if (toks.size() == 0) {
        return NULL;
    }

    // Parse source code
    Program* res = parse(fname_s, src_s, toks, addexit);
    if (!res) {
        return NULL;
    }

    // Otherwise, everything worked
    return res;
}

void carve_program_free(Program* self) {
    delete self;
}

u64 carve_exec_single(State* s) {
    // TODO: How to handle halted program
    if (s->is_exited) {
        fprintf(stderr, "INERNAL ERROR: CANNOT EXECUTE -- STATE IS EXITED\n");
        return -1;
    }

    // Now, first, we need to get the current instruction
    inst v = s->load<inst>(s->pc);

    // And execute in the state
    s->exec(v);

    // Increment to next instruction
    return s->pc += sizeof(inst);
}

void carve_exec_all(State* s) {
    while (!s->is_halted && !s->is_exited) {
        carve_exec_single(s);
    }
}


void carve_getrx(State* s, int len, char* data, int reg, int base) {
    if (base == 10) {
        snprintf(data, len, "%lli", (long long int)s->rx[reg]);
    } else {
        snprintf(data, len, "%016llx", (long long int)s->rx[reg]);
    }
}

void carve_getrf(State* s, int len, char* data, int reg) {
    snprintf(data, len, "%lf", (double)s->rf[reg]);
}

void carve_getrfx(State* s, int len, char* data, int reg) {
    snprintf(data, len, "%016lli", *(long long int*)&s->rf[reg]);
}


void carve_state_init(State* self, Program* program) {
    self->init(*program);
}

bool carve_is_halted(State* s) {
    return s->is_halted;
}

void carve_unhalt(State* s) {
    s->is_halted = false;
}

bool carve_is_exited(State* s) {
    return s->is_exited;
}

int carve_exit_status(State* s) {
    return s->exit_code;
}

char* carve_get_debug(Program* p) {
    ostringstream out;
    for (int i = 0; i < p->debug.size(); i++) {
        if (i > 0) out << "!";
        char tmp[256];
        inst code = *((inst*) &p->vmem[0][p->debug[i].pos]);
        snprintf(tmp, sizeof(tmp) - 2, "%08llx", (long long int)code);
        out << tmp << ";" << disassemble(code) << ";" << p->debug[i].ln + 1;
    }
    return strdup(out.str().c_str());
}

u8 carve_getbyte(State* s, int idx) {
    return s->vmem[idx];
}


u8* carve_getmem(State* s) {
    return &s->vmem[0];
}

u64 carve_getmemsize(State* s) {
    return s->vmem.size();
}

}
