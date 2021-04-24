/* carve.hh - header for libcarve, the C/C++ library powering carve
 *
 * Meant as an assembler, executor, and debugging engine for RISC-V code
 * 
 * @author: Cade Brown <cade@utk.edu>
 */

#ifndef CARVE_HH
#define CARVE_HH

#ifdef __EMSCRIPTEN__
 #include <emscripten.h>
 #define CARVE_API EMSCRIPTEN_KEEPALIVE extern "C"
#else
 #define CARVE_API extern "C"
#endif

// C/C++ standard library
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <functional>

// C++ STL
#include <vector>
#include <map>
#include <unordered_map>

// Use C++ standard
using namespace std;

namespace std {
  template<typename T1, typename T2>
  struct hash<std::pair<T1, T2>> {
    std::size_t operator()(std::pair<T1, T2> const &p) const {
        return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
  };
}
namespace carve {

// Instruction type, capable of holding any RISC-V instruction, in its
//   encoded format
typedef uint32_t inst;

// Specific sized integers
typedef uint64_t u64;
typedef  int64_t s64;
typedef uint32_t u32;
typedef  int32_t s32;
typedef uint16_t u16;
typedef  int16_t s16;
typedef  uint8_t u8;
typedef   int8_t s8;

// Specifically sized floating point numbesr
typedef float f32;
typedef double f64;

// Default size of the extra memory for heap/stack
const u64 DEFA_SZ_EXTRA = 0x100000;

// Number of integer registers in the VM
const u64 NUM_RX = 32;

// Number of floating point registers in the VM
const u64 NUM_RF = 32;

// Instruction description
struct instdesc {

    // Name of the instruction
    string name;

    // Kind of the instruction
    string kind;

    // Opcode, funct3, and funct6 fields (if applicable)
    // Zero otherwise
    u64 op, f3, f7;

    // Docstring
    string doc;

    // Constructor from all components
    instdesc(const string& name_, const string& kind_, u64 op_, u64 f3_, u64 f7_) 
        : name(name_), kind(kind_), op(op_), f3(f3_), f7(f7_) {}

};

// Source code token
struct Token {

    enum Kind {

        // Valid identifier
        IDENT,
        
        // Integer literal
        // Allows for hex, binary, or no prefix
        INT,

        // Floating point literal
        // TODO
        FLOAT,

        // String literal
        STR,

        // Character literal
        CHR,

        // Linebreak
        NEWLINE,

        // Dot: .
        DOT,

        // Colon: :
        COL,

        // Comma: ,
        COM,
        
        // Add: +
        ADD,

        // Sub: -
        SUB,
        
        // Left/right parenthesis: (, )
        LPAR,
        RPAR,
    } kind;


    // Line and column in source code
    int line, col;

    // Position and length, in bytes
    int pos, len;

    // Default constructor
    Token(Kind kind_, int line_, int col_, int pos_, int len_) : kind(kind_), line(line_), col(col_), pos(pos_), len(len_) {}

    // Get the token from the source code
    string get(const string& src) const {
        return src.substr(pos, len);
    }

    // Get string representing context in the source code
    string context(const string& fname, const string& src) const {

        // Find the start of the line
        int sl = pos;
        while (sl > 0 && src[sl - 1] != '\n') sl--;

        // Find the end of the line
        int el = pos + 1;
        while (el < src.size() && src[el] != '\n') el++;

        // Populate result
        string r;
        for (int i = sl; i < el; ++i) {
            r += src[i];
        }
        r += '\n';
        
        for (int i = 0; i < col; ++i) r += ' ';
        r += '^';
        for (int i = 0; i < len - 1; ++i) r += '~';
        r += '\n';
        r += "@ " + fname + " (L" + to_string(line + 1) + ", C" + to_string(col + 1) + ")\n";

        return r;

    }


};


// Map of all instructions, keyed by name 
extern unordered_map<string, instdesc> insts;

// Debug table entry
struct debug_entry {
    u64 pos;  // position in program memory
    u64 ln;   // line number
};

// RISC-V program
struct Program {

    // File name and source string
    string fname, src;

    // Virtual memory segments:
    //   0: text
    //   1: rodata
    //   2: data
    //   3: bss
    vector<u8> vmem[4];

    // exe table for ui
    vector<debug_entry> debug;
};

// Virtual RISC-V machine state, capable of holding all relevant parts of
//   registers, memory, and states
struct State {

    // Virtual memory sector of the program, which contains all sections of data:
    //   * text
    //   * rodata
    //   * data
    //   * bss
    //   * extra (heap, stack, ...)
    vector<u8> vmem;

    // x (integer) registers for the VM
    u64 rx[NUM_RX];

    // f (floating point) registers for the VM
    f64 rf[NUM_RF];

    // Program counter, current index into 'vmem' pointing to the current instruction
    u64 pc;

    // Whether or not the program is currently halted. If it is halted, it
    //   must be unhalted
    bool is_halted;

    // Whether the program has hard exited
    bool is_exited;

    // The exit code, only valid if 'is_exited'
    int exit_code;

    // Template for loading a given typename from an index (bytewise) into 'vmem'
    template<typename T>
    T& load(u64 idx) {
        return *(T*)&vmem[idx];
    }

    // Initialize the state from a given program
    void init(const Program& prog, u64 extra=DEFA_SZ_EXTRA);

    // Executes a single instruction, but doesn't do much else
    // This does not update PC !!
    void exec(inst v);

    // Return index into 'vmem' from an address
    s64 real(s64 idx) {
        return idx;
        // return idx - 0x1000;
    }

    // Return fake address from 'vmem' index
    s64 fake(s64 idx) {
        return idx;
       // return idx + 0x1000;
    }

    // Signed register
    s64& rxs(int idx) {
        return *(s64*)(&rx[idx]);
    }

    // 32 bit register
    f32& rf32(int idx) {
        return *(f32*)(&rf[idx]);
    }

};

// Disassemble an instruction
string disassemble(inst v);

// Sign-extend a value, where 'bit' is the index of the bit
s64 sext(u64 val, int bit);

// Tokenize a source string, and return a vector of tokens
// If return value is empty, there was a problem
vector<Token> lex(const string& fname, const string& src);

// Parse a program from source code, and return a pointer to it
Program* parse(const string& fname, const string& src, const vector<Token>& toks);

// Add bytes to a bytes array
template<typename T>
void addbytes(vector<u8>& res, T val) {
    u8* ptr = (u8*)&val;
    for (int i = 0; i < sizeof(T); ++i) {
        res.push_back(ptr[i]);
    }
}



/* Encode an instruction, with arguments. Each '_<kind>' function corresponds to a different
 *   instruction kind
 * 
 * All arguments are assumed to be their real value (i.e. should not be shifted, or
 *   extended or anything). These routines figure out all the packing
 */
inst enc_R(u64 op, u64 f3, u64 f7, int rd, int rs1, int rs2);
inst enc_I(u64 op, u64 f3, int rd, int rs1, u64 imm);
inst enc_U(u64 op, int rd, u64 imm);
inst enc_J(u64 op, int rd, u64 imm);
inst enc_S(u64 op, u64 f3, int rs1, int rs2, u64 imm);
inst enc_B(u64 op, u64 f3, int rs1, int rs2, u64 imm);
inst enc_y(u64 op, u64 f3, int rd, int rs1, u64 imm);

/* Decode an instruction, setting each reference argument to that component
 *
 * Sets all reference arguments to their real values (i.e. not shifted or sign extended)
 */
void dec_R(inst v, u64& op, u64& f3, u64& f7, int& rd, int& rs1, int& rs2);
void dec_I(inst v, u64& op, u64& f3, int& rd, int& rs1, u64& imm);
void dec_U(inst v, u64& op, int& rd, u64& imm);
void dec_J(inst v, u64& op, int& rd, u64& imm);
void dec_S(inst v, u64& op, u64& f3, int& rs1, int& rs2, u64& imm);
void dec_B(inst v, u64& op, u64& f3, int& rs1, int& rs2, u64& imm);
void dec_y(inst v, u64& op, u64& f3, int& rd, int& rs1, u64& imm);


/* Instruction implementation */

namespace impl {


// RV32I, implemented in 'impl/RV32I.cc'
void _lui    (State& s, int rd, u64 imm);
void _auipc  (State& s, int rd, u64 imm);
void _jal    (State& s, int rd, u64 imm);
void _jalr   (State& s, int rd, int rs1, u64 imm);
void _beq    (State& s, int rs1, int rs2, u64 imm);
void _bne    (State& s, int rs1, int rs2, u64 imm);
void _blt    (State& s, int rs1, int rs2, u64 imm);
void _bge    (State& s, int rs1, int rs2, u64 imm);
void _bltu   (State& s, int rs1, int rs2, u64 imm);
void _bgeu   (State& s, int rs1, int rs2, u64 imm);
void _lb     (State& s, int rd, int rs1, u64 imm);
void _lh     (State& s, int rd, int rs1, u64 imm);
void _lw     (State& s, int rd, int rs1, u64 imm);
void _lbu    (State& s, int rd, int rs1, u64 imm);
void _lhu    (State& s, int rd, int rs1, u64 imm);
void _sb     (State& s, int rs1, int rs2, u64 imm);
void _sh     (State& s, int rs1, int rs2, u64 imm);
void _sw     (State& s, int rs1, int rs2, u64 imm);
void _addi   (State& s, int rd, int rs1, u64 imm);
void _slti   (State& s, int rd, int rs1, u64 imm);
void _sltiu  (State& s, int rd, int rs1, u64 imm);
void _xori   (State& s, int rd, int rs1, u64 imm);
void _ori    (State& s, int rd, int rs1, u64 imm);
void _andi   (State& s, int rd, int rs1, u64 imm);
void _slli   (State& s, int rd, int rs1, u64 imm);
void _srli   (State& s, int rd, int rs1, u64 imm);
void _add    (State& s, int rd, int rs1, int rs2);
void _sub    (State& s, int rd, int rs1, int rs2);
void _sll    (State& s, int rd, int rs1, int rs2);
void _srl    (State& s, int rd, int rs1, int rs2);
void _sra    (State& s, int rd, int rs1, int rs2);
void _slt    (State& s, int rd, int rs1, int rs2);
void _sltu   (State& s, int rd, int rs1, int rs2);
void _xor    (State& s, int rd, int rs1, int rs2);
void _or     (State& s, int rd, int rs1, int rs2);
void _and    (State& s, int rd, int rs1, int rs2);
void _ebreak (State& s, int rd, int rs1, u64 imm);

}



/* External C-API interface */

// Initialize the CARVE library
CARVE_API void carve_init();

// Create a new State
// NOTE: Free this with 'carve_state_free()'
CARVE_API State* carve_state_new();

// Free a state allocated with 'carve_state_new()'
CARVE_API void carve_state_free(State* self);

// Initialize a state with a program
CARVE_API void carve_state_init(State* self, Program* program);

// Create a new program, from the given filename and source
// NOTE: Free this with 'carve_program_free()'
CARVE_API Program* carve_program_new(const char* fname, const char* src);

// Free a program allocated with 'carve_program_new()'
CARVE_API void carve_program_free(Program* self);

// Execute a single instruction on 's'
CARVE_API void carve_exec_single(State* s);

// Execute until its done
CARVE_API void carve_exec_all(State* s);

// Get integer register (X-register) and store as a string
CARVE_API void carve_getrx(State* s, int len, char* data, int reg, int base);

// Get a floating point register (F-register) and store as a string
CARVE_API void carve_getrf(State* s, int len, char* data, int reg);

// Get a floating point register (F-register) and store as a hex string
CARVE_API void carve_getrfx(State* s, int len, char* data, int reg);

// Determines if a state is halted
CARVE_API bool carve_is_halted(State* s);

// Returns the data required for debug table
CARVE_API char* carve_get_debug(Program* p);

// Get memory, returns a pointer to it
CARVE_API u8* carve_getmem(State* s);

// Returns the total size of the memory in the state
CARVE_API u64 carve_getmemsize(State* s);

// Returns a single byte from memory
CARVE_API u8 carve_getbyte(State* s, int idx);

}





#endif /* CARVE_H */
