/* parse.cc - Parser implementation
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.hh"

namespace carve {


// Is 'c' whitespace
static bool my_isspace(int c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// Is 'c' a valid digit in base 'b'
static bool my_isdigit(int c, int b) {
    if (b == 10) {
        return '0' <= c && c <= '9';
    } else if (b == 16) {
        return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
    } else if (b == 2) {
        return (c == '0' || c == '1');
    }

    return false;
}

// Is 'c' a valid start of an identifier
static bool my_isident_s(int c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

// Is 'c' a valid middle of an identifier
static bool my_isident_m(int c) {
    return my_isdigit(c, 10) || my_isident_s(c);
}

vector<Token> lex(const string& fname, const string& src) {
    vector<Token> res;

    // C-style string buffer, and length
    const char* s = src.c_str();
    int sl = src.size();

    // Positions in the text
    int pos = 0, line = 0, col = 0;

    // Current starting positions for a token
    int spos, sline, scol;

    /* Advance one character */
    #define ADV() do { \
        char _c = src[pos++]; \
        if (_c == '\n') { \
            line++; \
            col = 0; \
        } else { \
            col++; \
        } \
    } while (0)

    /* Add token */
    #define ADD(_kind) do { \
        res.push_back(Token(_kind, sline, scol, spos, pos - spos)); \
    } while (0)

    while (pos < sl) {
        char c = s[pos];
        spos = pos;
        sline = line;
        scol = col;
        if (c == '#') {
            // Comments, ignore until a newline
            do {
                ADV();
            } while (pos < sl && s[pos] != '\n');
        } else if (c == '\n') {
            // Newline should be handled differently than space
            ADV();
            ADD(Token::Kind::NEWLINE);
        } else if (c == '.') {
            ADV();
            ADD(Token::Kind::DOT); 
        } else if (c == ',') {
            ADV();
            ADD(Token::Kind::COM); 
        } else if (c == ':') {
            ADV();
            ADD(Token::Kind::COL); 
        } else if (c == '+') {
            ADV();
            ADD(Token::Kind::ADD); 
        } else if (c == '-') {
            ADV();
            ADD(Token::Kind::SUB); 
        } else if (c == '(') {
            ADV();
            ADD(Token::Kind::LPAR); 
        } else if (c == ')') {
            ADV();
            ADD(Token::Kind::RPAR); 
        } else if (my_isident_s(c)) {
            // Identifier
            do {
                ADV();
            } while (pos < sl && my_isident_m(s[pos]));
            ADD(Token::Kind::IDENT); 
        } else if (my_isdigit(c, 10)) {
            // Integer/float constant

            int base = 10;

            /* 0[xX] prefix for base 16 */
            if (pos+1 < sl && s[pos+1] == 'x' || s[pos+1] == 'X') {
                ADV();
                ADV();
                base = 16;
            }
            else if (pos+1 < sl && s[pos+1] == 'b' || s[pos+1] == 'B') {
                ADV();
                ADV();
                base = 2;
            }

            do {
                ADV();
            } while (pos < sl && (my_isdigit(s[pos], base) || (s[pos] == '_')));

            bool is_float = s[pos] == '.';
            if (is_float) {
                ADV();
                do {
                    ADV();
                } while (pos < sl && (my_isdigit(s[pos], base) || (s[pos] == '_')));
            }

            ADD(is_float ? Token::Kind::FLOAT : Token::Kind::INT);

        } else if (my_isspace(c)) {
            // Skip space
            ADV();

        } else if (c == '"') {
            char sc = c;

            do {
                if (s[pos] == '\\') {
                    ADV();
                }
                ADV();

            } while (pos < sl && s[pos] != sc);

            if (s[pos] != sc) {
                cerr << "Expected an end to the string literal" << endl;
                ADD(Token::Kind::IDENT);
                cerr << res[res.size() - 1].context(fname, src);
                res.clear();
                return res;
            }
            ADV();

            ADD(Token::Kind::STR);

        } else {
            // Unknown character
            cerr << "Unexpected character: '" << c << "'" << endl;
            ADD(Token::Kind::IDENT);
            cerr << res[res.size() - 1].context(fname, src);
            res.clear();
            return res;
        }
    }

    if (res.size() == 0) {
        // Don't return error, add newline
        res.push_back(Token(Token::Kind::NEWLINE, 0, 0, 0, 0));
    }
    return res;
}


// Parse and skip a token kind
static bool parse_skip(Program* res, const vector<Token>& toks, int& toki, Token::Kind kind) {
    if (toki >= toks.size()) {

        cerr << "Unexpected EOF" << endl;
        cerr << toks[toki - 1].context(res->fname, res->src);
        return false;
    }
    Token t = toks[toki];
    if (t.kind == kind) {
        toki++;
        return true;
    } else {

        cerr << "Unexpected token" << endl;
        cerr << t.context(res->fname, res->src);
        return false;
    }
}

// Parse integer register
static bool parse_rx(Program* res, const vector<Token>& toks, int& toki, int& out) {
    Token t = toks[toki];
    if (t.kind == Token::Kind::IDENT) {
#define C(_str, _rv) else if (t.len == sizeof(_str) - 1 && strncmp(&res->src[t.pos], _str, t.len) == 0) { \
        toki++; \
        out = _rv; \
        return true; \
    }

        if (false) {}

        C("x0", 0)
        C("x1", 1)
        C("x2", 2)
        C("x3", 3)
        C("x4", 4)
        C("x5", 5)
        C("x6", 6)
        C("x7", 7)
        C("x8", 8)
        C("x9", 9)
        C("x10", 10)
        C("x11", 11)
        C("x12", 12)
        C("x13", 13)
        C("x14", 14)
        C("x15", 15)
        C("x16", 16)
        C("x17", 17)
        C("x18", 18)
        C("x19", 19)
        C("x20", 20)
        C("x21", 21)
        C("x22", 22)
        C("x23", 23)
        C("x24", 24)
        C("x25", 25)
        C("x26", 26)
        C("x27", 27)
        C("x28", 28)
        C("x29", 29)
        C("x30", 30)
        C("x31", 31)

        C("zero", 0)
        C("ra", 1)
        C("sp", 2)
        C("gp", 3)
        C("tp", 4)
        C("t0", 5)
        C("t1", 6)
        C("t2", 7)
        C("s0", 8)
        C("fp", 8)
        C("s1", 9)
        C("a0", 10)
        C("a1", 11)
        C("a2", 12)
        C("a3", 13)
        C("a4", 14)
        C("a5", 15)
        C("a6", 16)
        C("a7", 17)
        C("s2", 18)
        C("s3", 19)
        C("s4", 20)
        C("s5", 21)
        C("s6", 22)
        C("s7", 23)
        C("s8", 24)
        C("s9", 25)
        C("s10", 26)
        C("s11", 27)
        C("t3", 28)
        C("t4", 29)
        C("t5", 30)
        C("t6", 31)

        #undef C

        cerr << "Unknown register name" << endl;
        cerr << t.context(res->fname, res->src);
        return false;
    } else {
        cerr << "Expected a register name" << endl;
        cerr << t.context(res->fname, res->src);
        return false;
    }
}

// Parse immediate
static bool parse_imm(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg, int& out) {
    bool is_neg = false;
    if (toks[toki].kind == Token::Kind::SUB) {
        toki++;
        is_neg = true;
    }
    if (toks[toki].kind == Token::Kind::INT) {
        Token t = toks[toki++];
        int64_t rv = 0;
        
        int base = 10;
        for (int i = 0; i < t.len; i++) {
            char d = res->src[t.pos + i];
            
            if (d == '_') continue;

            /* handle nondigits */
            if (!my_isdigit(d, base)) {
                /* if base change is valid, check for base char */
                if (i == 1 && res->src[t.pos] == '0') {
                    if (d == 'x' || d == 'X') {
                        base = 16;
                        continue;
                    } else if (d == 'b' || d == 'B') {
                        base = 2;
                        continue;
                    } else {
                        cerr << "Character doesn't match expected integer base" << endl;
                        cerr << t.context(res->fname, res->src);
                        return false;
                    }
                }
            }

            if (d >= 'a') d -= ('a' - 10);
            else if (d >= 'A') d -= ('A' - 10);
            else d -= '0';

            rv = base * rv + d;
        }

        out = is_neg ? -rv : rv;
        return true;
    } else if (toks[toki].kind == Token::Kind::IDENT) {
        // Label reference
        Token t = toks[toki++];

        // Add to the backpatch array
        back[{seg, res->vmem[seg].size()}] = { id.kind, t.get(res->src) };

        // Return 0 for now, so add nothing
        out = 0;
        return true;
    } else {
        cerr << "Unexpected token, expected immediate value" << endl;
        cerr << toks[toki].context(res->fname, res->src);
        return false;
    }
}


// Parse integer
static bool parse_int(Program* res, const vector<Token>& toks, int& toki, s64& out) {
    bool is_neg = false;
    if (toks[toki].kind == Token::Kind::SUB) {
        toki++;
        is_neg = true;
    }
    if (toks[toki].kind == Token::Kind::INT) {
        Token t = toks[toki++];
        s64 rv = 0;
        
        int base = 10;
        for (int i = 0; i < t.len; i++) {
            char d = res->src[t.pos + i];
            
            if (d == '_') continue;

            /* handle nondigits */
            if (!my_isdigit(d, base)) {
                /* if base change is valid, check for base char */
                if (i == 1 && res->src[t.pos] == '0') {
                    if (d == 'x' || d == 'X') {
                        base = 16;
                        continue;
                    } else if (d == 'b' || d == 'B') {
                        base = 2;
                        continue;
                    } else {
                        cerr << "Character doesn't match expected integer base" << endl;
                        cerr << t.context(res->fname, res->src);
                        return false;
                    }
                }
            }

            if (d >= 'a') d -= ('a' - 10);
            else if (d >= 'A') d -= ('A' - 10);
            else d -= '0';

            rv = base * rv + d;
        }

        out = is_neg ? -rv : rv;
        return true;
    } else {
        cerr << "Unexpected token, expected integer" << endl;
        cerr << toks[toki].context(res->fname, res->src);
        return false;
    }
}



// Parse float
static bool parse_float(Program* res, const vector<Token>& toks, int& toki, double& out) {
    int i = 0;

    bool is_neg = false;
    if (toks[toki].kind == Token::Kind::SUB) {
        toki++;
        is_neg = true;
    }
    if (toks[toki].kind == Token::Kind::INT || toks[toki].kind == Token::Kind::FLOAT) {
        Token t = toks[toki++];
        double rv = strtod(t.get(res->src).c_str(), NULL);


        out = is_neg ? -rv : rv;
        return true;
    } else {
        cerr << "Unexpected token, expected floating point number" << endl;
        cerr << toks[toki].context(res->fname, res->src);
        return false;
    }
}
// Parse a string literal
static bool parse_str(Program* res, const string& dir, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, string& val) {
    if (toks[toki].kind == Token::Kind::STR) {
        Token t = toks[toki++];
        string st = t.get(res->src);
        val = "";

        for (size_t i = 1; i < st.size() - 1; ++i) {
            char c = st[i];
            if (c == '\\') {
                // Escape sequence
                i++;
                c = st[i];
                if (c == 'n') {
                    val.push_back('\n');
                } else if (c == 't') {
                    val.push_back('\t');
                } else if (c == 'a') {
                    val.push_back('\a');
                } else if (c == 'b') {
                    val.push_back('\b');
                } else {
                    val.push_back('\\');
                }
            } else {
                val.push_back(c);
            }
        }

        return true;
    } else {
        cerr << "Expected string literal" << endl;
        cerr << toks[toki].context(res->fname, res->src);
        return false;
    }
}

// Parse R-kind instruction
static bool parse_R(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg) {
    int rd, rs1, rs2;
    if (!parse_rx(res, toks, toki, rd)) return false;

    if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

    if (!parse_rx(res, toks, toki, rs1)) return false;

    if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

    if (!parse_rx(res, toks, toki, rs2)) return false;

    // Add assembled instruction
    inst v = enc_R(id.op, id.f3, id.f7, rd, rs1, rs2);
    addbytes<inst>(res->vmem[seg], v);

    return true;
}

// Parse I-kind instruction
static bool parse_I(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg) {
    int rd, rs1;
    int imm;

    if (!parse_rx(res, toks, toki, rd)) return false;

    if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

    if (toks[toki+1].kind == Token::Kind::LPAR) {
        // inst a, c(b)
        if (!parse_imm(res, back, toks, toki, id, seg, imm)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::LPAR)) return false;
        
        if (!parse_rx(res, toks, toki, rs1)) return false;
            
        if (!parse_skip(res, toks, toki, Token::Kind::RPAR)) return false;

    } else {
        // inst a, b, c
        if (!parse_rx(res, toks, toki, rs1)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_imm(res, back, toks, toki, id, seg, imm)) return false;
    }

    // Add assembled instruction
    inst v = enc_I(id.op, id.f3, rd, rs1, imm);
    addbytes<inst>(res->vmem[seg], v);

    return true;
}

// Parse S-kind instruction
static bool parse_S(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg) {
    int rs1, rs2;
    int imm;
    if (!parse_rx(res, toks, toki, rs2)) return false;

    if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

    if (toks[toki+1].kind == Token::Kind::LPAR) {
        // inst a, c(b)
        if (!parse_imm(res, back, toks, toki, id, seg, imm)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::LPAR)) return false;
        
        if (!parse_rx(res, toks, toki, rs1)) return false;
            
        if (!parse_skip(res, toks, toki, Token::Kind::RPAR)) return false;

    } else {
        // inst a, b, c
        if (!parse_rx(res, toks, toki, rs1)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_imm(res, back, toks, toki, id, seg, imm)) return false;
    }

    // Add assembled instruction
    inst v = enc_S(id.op, id.f3, rs1, rs2, imm);
    addbytes<inst>(res->vmem[seg], v);

    return true;
}

// Parse B-kind instruction
static bool parse_B(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg) {
    int rs1, rs2;
    int imm;
    if (!parse_rx(res, toks, toki, rs1)) return false;

    if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

    if (!parse_rx(res, toks, toki, rs2)) return false;

    if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

    if (!parse_imm(res, back, toks, toki, id, seg, imm)) return false;

    // Add assembled instruction
    inst v = enc_B(id.op, id.f3, rs1, rs2, imm);
    addbytes<inst>(res->vmem[seg], v);

    return true;
}

// Parse U-kind instruction
static bool parse_U(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg) {
    int rd;
    int imm;
    if (!parse_rx(res, toks, toki, rd)) return false;

    if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

    if (!parse_imm(res, back, toks, toki, id, seg, imm)) return false;

    // Add assembled instruction
    inst v = enc_U(id.op, rd, imm);
    addbytes<inst>(res->vmem[seg], v);

    return true;
}

// Parse J-kind instruction
static bool parse_J(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg) {
    int rd;
    int imm;
    if (!parse_rx(res, toks, toki, rd)) return false;

    if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

    if (!parse_imm(res, back, toks, toki, id, seg, imm)) return false;

    // Add assembled instruction
    inst v = enc_J(id.op, rd, imm);
    addbytes<inst>(res->vmem[seg], v);

    return true;
}

// Parse y-kind instruction
static bool parse_y(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg) {
    if (id.name == "ecall") {
        // Add assembled instruction
        inst v = enc_I(id.op, 0, 0, 0, 0 /* = ecall */);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else if (id.name == "ebreak") {
        // Add assembled instruction
        inst v = enc_I(id.op, 0, 0, 0, 1 /* = ebreak */);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else {
        cerr << "Unsupported instruction" << endl;
        cerr << toks[toki - 1].context(res->fname, res->src);
        return false;
    }
}

// Parse p-kind instruction (psuedo-instructions)
static bool parse_p(Program* res, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, instdesc& id, int seg) {
    if (id.name == "j") {
        // Add assembled instruction
        instdesc& oid = insts.find("jal")->second;
        int imm;
        if (!parse_imm(res, back, toks, toki, oid, seg, imm)) return false;

        // Encode psuedo-instruction
        inst v = enc_J(oid.op, 0, imm);
        addbytes<inst>(res->vmem[seg], v);
        return true;
    } else if (id.name == "nop") {
        instdesc& oid = insts.find("addi")->second;
        int imm;
        if (!parse_imm(res, back, toks, toki, oid, seg, imm)) return false;

        inst v = enc_I(oid.op, oid.f3, 0, 0, 0);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else if (id.name == "li") {
        int rd, rs = 0, imm;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        // TODO: REplace id with oid?
        if (!parse_imm(res, back, toks, toki, id, seg, imm)) return false;

        if (imm & ~((1ULL << 12) - 1)) {
            instdesc& oid = insts.find("lui")->second;
            inst v = enc_U(oid.op, rd, imm + (1 << 11));
            addbytes<inst>(res->vmem[seg], v);
            rs = rd;
        }

        instdesc& oid = insts.find("addi")->second;
        inst v = enc_I(oid.op, oid.f3, rd, rs, imm);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else if (id.name == "la") {
        int rd, rs = 0, imm;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;
        instdesc& oid = insts.find("auipc")->second;

        Token t = toks[toki];
        if (!parse_imm(res, back, toks, toki, oid, seg, imm)) return false;

        inst v = enc_U(oid.op, rd, imm);
        bool dou = (imm & (~0xFFF)) != 0;
        if (dou) {
            addbytes<inst>(res->vmem[seg], v);
        }

        oid = insts.find("addi")->second;
        v = enc_I(oid.op, oid.f3, rd, rd, imm);
        if (t.kind == Token::Kind::IDENT) {
            // We need to link both instructions to be backpatched
            if (dou) {
                back.erase({seg, res->vmem[seg].size() - 4});
                back.erase({seg, res->vmem[seg].size() - 8});
            } else {
                back.erase({seg, res->vmem[seg].size()});
            }
            back[{seg, res->vmem[seg].size()}] = { oid.kind, t.get(res->src) };
        }
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else if (id.name == "mv") {
        instdesc& oid = insts.find("addi")->second;
        int rd, rs;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_rx(res, toks, toki, rs)) return false;

        inst v = enc_I(oid.op, oid.f3, rd, rs, 0);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else if (id.name == "not") {
        instdesc& oid = insts.find("xori")->second;

        int rd, rs;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_rx(res, toks, toki, rs)) return false;

        inst v = enc_I(oid.op, oid.f3, rd, rs, -1);
        addbytes<inst>(res->vmem[seg], v);
        return true;
    } else if (id.name == "neg") {
        instdesc& oid = insts.find("sub")->second;

        int rd, rs;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_rx(res, toks, toki, rs)) return false;

        inst v = enc_R(oid.op, oid.f3, oid.f7, rd, rs, 0);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else if (id.name == "seqz") {
        instdesc& oid = insts.find("sltiu")->second;

        int rd, rs;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_rx(res, toks, toki, rs)) return false;

        inst v = enc_I(oid.op, oid.f3, rd, rs, 1);
        addbytes<inst>(res->vmem[seg], v);
        return true;


    } else if (id.name == "snez") {
        instdesc& oid = insts.find("sltu")->second;

        int rd, rs;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_rx(res, toks, toki, rs)) return false;

        inst v = enc_I(oid.op, oid.f3, rd, 0, rs);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else if (id.name == "sltz") {
        instdesc& oid = insts.find("slt")->second;

        int rd, rs;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_rx(res, toks, toki, rs)) return false;

        inst v = enc_R(oid.op, oid.f3, oid.f7, rd, rs, 0);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else if (id.name == "sgtz") {
        instdesc& oid = insts.find("slt")->second;

        int rd, rs;
        if (!parse_rx(res, toks, toki, rd)) return false;

        if (!parse_skip(res, toks, toki, Token::Kind::COM)) return false;

        if (!parse_rx(res, toks, toki, rs)) return false;

        inst v = enc_R(oid.op, oid.f3, oid.f7, rd, 0, rs);
        addbytes<inst>(res->vmem[seg], v);
        return true;

    } else {
        cerr << "Unknown psuedo instruction" << endl;
        cerr << toks[toki - 1].context(res->fname, res->src);
        return false;
    }
}
// Perform a directive
static bool do_dir(Program* res, const string& dir, unordered_map< pair<int, int>, pair<string, string> >& back, const vector<Token>& toks, int& toki, int& seg) {
    if (dir == "ascii") {
        string val;
        if (!parse_str(res, dir, back, toks, toki, val)) {
            return false;
        }

        // Push all data to current segment
        for (size_t i = 0; i < val.size(); ++i) {
            res->vmem[seg].push_back(val[i]);
        }

        return true;

    } else if (dir == "asciz") {
        string val;
        if (!parse_str(res, dir, back, toks, toki, val)) {
            return false;
        }

        // Push all data to current segment
        for (size_t i = 0; i < val.size(); ++i) {
            res->vmem[seg].push_back(val[i]);
        }
        res->vmem[seg].push_back(0);

        return true;
    } else if (dir == "global") {
        // just skip, as we don't use global
        toki++;
        return true;

    } else if (dir == "section") {
        if (toks[toki].kind == Token::Kind::DOT) {
            toki++;
        }
        string sec = toks[toki++].get(res->src);
        int ss = 0;
        if (sec == "text") {
            ss = 0;
        } else if (sec == "rodata") {
            ss = 1;
        } else if (sec == "data") {
            ss = 2;
        } else if (sec == "bss") {
            ss = 3;
        } else {
            cerr << "Unknown section name" << endl;
            cerr << toks[toki - 1].context(res->fname, res->src);
            return false;
        }

        // Set segment
        seg = ss;
        return true;


    } else if (dir == "byte") {
        toki--;
        do {
            toki++;
            s64 val;
            if (!parse_int(res, toks, toki, val)) {
                return false;
            }

            res->vmem[seg].push_back(val & 0xFF);
        } while (toks[toki].kind == Token::Kind::COM);

        return true;

    } else if (dir == "half") {
        toki--;
        do {
            toki++;
            s64 val;
            if (!parse_int(res, toks, toki, val)) {
                return false;
            }

            res->vmem[seg].push_back(val & 0xFF);
            res->vmem[seg].push_back((val >> 8) & 0xFF);
        } while (toks[toki].kind == Token::Kind::COM);

        return true;

    } else if (dir == "word") {

        toki--;
        do {
            toki++;
            s64 val;
            if (!parse_int(res, toks, toki, val)) {
                return false;
            }

            res->vmem[seg].push_back(val & 0xFF);
            res->vmem[seg].push_back((val >> 8) & 0xFF);
            res->vmem[seg].push_back((val >> 16) & 0xFF);
            res->vmem[seg].push_back((val >> 24) & 0xFF);
        } while (toks[toki].kind == Token::Kind::COM);

        return true;

    } else if (dir == "dword") {


        toki--;
        do {
            toki++;
            s64 val;
            if (!parse_int(res, toks, toki, val)) {
                return false;
            }

            res->vmem[seg].push_back(val & 0xFF);
            res->vmem[seg].push_back((val >> 8) & 0xFF);
            res->vmem[seg].push_back((val >> 16) & 0xFF);
            res->vmem[seg].push_back((val >> 24) & 0xFF);
            res->vmem[seg].push_back((val >> 32) & 0xFF);
            res->vmem[seg].push_back((val >> 40) & 0xFF);
            res->vmem[seg].push_back((val >> 48) & 0xFF);
            res->vmem[seg].push_back((val >> 56) & 0xFF);
        } while (toks[toki].kind == Token::Kind::COM);

        return true;

    } else if (dir == "float") {

        toki--;
        do {
            toki++;
            double val;
            if (!parse_float(res, toks, toki, val)) {
                return false;
            }
            addbytes(res->vmem[seg], (float)val);
        } while (toks[toki].kind == Token::Kind::COM);

        return true;

    } else if (dir == "double") {
        toki--;
        do {
            toki++;

            double val;
            if (!parse_float(res, toks, toki, val)) {
                return false;
            }

            addbytes(res->vmem[seg], (double)val);

        } while (toks[toki].kind == Token::Kind::COM);

        return true;

    } else if (dir == "space") {
        s64 num;
        if (!parse_int(res, toks, toki, num)) {
            return false;
        }
        for (int i = 0; i < num; ++i) {
            res->vmem[seg].push_back(0);
        }

        return true;

    } else {
        cerr << "Unknown directive" << endl;
        cerr << toks[toki - 1].context(res->fname, res->src);
        return false;
    }
}

Program* parse(const string& fname, const string& src, const vector<Token>& toks, bool addexit) {
    Program* res = new Program();
    res->fname = fname;
    res->src = src;

    // Label map, mapping the string name to a pair of '(segment, offset)', giving
    //   the segment index and offset into that segment
    unordered_map< string, pair<int, int> > labels;

    // Back patches, from locations (pair of '(segment, offset)'), and a pair of strings,
    //   (kind, label)
    unordered_map< pair<int, int>, pair<string, string> > back;


    // Current segment (default: text)
    int seg = 0;

    int toki = 0;
    while (toki < toks.size()) {
        Token t = toks[toki++];
        if (t.kind == Token::Kind::IDENT || (t.kind == Token::Kind::INT && toks[toki].kind == Token::Kind::COL)) {
            if (toks[toki].kind == Token::Kind::COL) {
                // Label
                toki++;

                // Set label to current position
                labels[t.get(src)] = { seg, res->vmem[seg].size() };

            } else {
                // Instruction

                u64 ln = t.line;
                u64 startpos = res->vmem[0].size();

                // Look up instruction
                unordered_map<string, instdesc>::iterator it = insts.find(t.get(src));
                if (it == insts.end()) {
                    cerr << "Unknown instruction: '" << t.get(src) << "'" << endl;
                    cerr << t.context(fname, src);
                    delete res;
                    return NULL;
                }

                // Now, parse the instruction
                instdesc& id = it->second;
                
                // Parse depending on the specific kind of instruction
                if (id.kind == "R") {
                    if (!parse_R(res, back, toks, toki, id, seg)) {
                        delete res;
                        return NULL;
                    }
                } else if (id.kind == "I") {
                    if (!parse_I(res, back, toks, toki, id, seg)) {
                        delete res;
                        return NULL;
                    }
                } else if (id.kind == "S") {
                    if (!parse_S(res, back, toks, toki, id, seg)) {
                        delete res;
                        return NULL;
                    }
                } else if (id.kind == "B") {
                    if (!parse_B(res, back, toks, toki, id, seg)) {
                        delete res;
                        return NULL;
                    }
                } else if (id.kind == "U") {
                    if (!parse_U(res, back, toks, toki, id, seg)) {
                        delete res;
                        return NULL;
                    }
                } else if (id.kind == "J") {
                    if (!parse_J(res, back, toks, toki, id, seg)) {
                        delete res;
                        return NULL;
                    }
                } else if (id.kind == "y") {
                    if (!parse_y(res, back, toks, toki, id, seg)) {
                        delete res;
                        return NULL;
                    }
                } else if (id.kind == "p") {
                    if (!parse_p(res, back, toks, toki, id, seg)) {
                        delete res;
                        return NULL;
                    }
                } else {
                    cerr << "Internal error: Instruction kind not supported yet (" << id.kind << ")" << endl;
                    cerr << t.context(res->fname, res->src);
                    delete res;
                    return NULL;
                }
                // Special cases
                if (id.name == "srai") {
                    // We need to set the 30th bit
                    // SEE: https://stackoverflow.com/questions/39489318/risc-v-implementing-slli-srli-and-srai
                    *(inst*)&(res->vmem[seg][res->vmem[seg].size() - 4]) |= 1ULL << 30;
                }
                // add debug information
                if (seg == 0) {
                    for (u64 j = startpos; j < res->vmem[0].size(); j += sizeof(inst)) {
                        res->debug.push_back({j, ln});
                    }
                }
            }
        } else if (t.kind == Token::Kind::DOT) {
            // Directive
            // Get directive name
            string dir = toks[toki++].get(src);
            if (!do_dir(res, dir, back, toks, toki, seg)) {
                delete res;
                return NULL;
            }

        } else if (t.kind == Token::Kind::NEWLINE) {
            // Skip newline
        } else {
            cerr << "Unexpected token" << endl;
            cerr << t.context(fname, src);
            delete res;
            return NULL;
        }
    }    
    // Add exit
    if (addexit) {
        seg = 0;
        u64 addi_inst = enc_I(0x13, 0, 17, 0, 0 /* a7 = 0 */);
        u64 ecall_inst = enc_I(0x73, 0, 0, 0, 0  /* ecall  */);
        res->debug.push_back({res->vmem[seg].size(), (u64) -1});
        addbytes<inst>(res->vmem[seg], addi_inst);
        res->debug.push_back({res->vmem[seg].size(), (u64) -1});
        addbytes<inst>(res->vmem[seg], ecall_inst);
    }


    // Now, link up references
    for (unordered_map< pair<int, int>, pair<string, string> >::iterator it = back.begin(); it != back.end(); ++it) {
        string& kind = it->second.first;
        inst& v = *(inst*)&(res->vmem[it->first.first][it->first.second]);
        int rd, rs1, rs2;
        u64 op, f3, f7;
        u64 imm;

        // Label target
        pair<int, int> from = it->first;
        pair<int, int> to = labels[it->second.second];

        // Calculate offset
        s64 newimm = to.second - from.second;

        // Add spaces between segments
        for (int i = from.first; i < to.first; ++i) {
            newimm += res->vmem[i].size();
        }
        for (int i = from.first; i > to.first; --i) {
            newimm -= res->vmem[i].size();
        }

        // Make sure to add back the immediate that was already there!
        if (kind == "I") {
            dec_I(v, op, f3, rd, rs1, imm);
            v = enc_I(op, f3, rd, rs1, newimm);
        } else if (kind == "U") {
            dec_U(v, op, rd, imm);
            v = enc_U(op, rd, newimm);
        } else if (kind == "J") {
            dec_J(v, op, rd, imm);
            v = enc_J(op, rd, newimm);
        } else if (kind == "S") {
            dec_S(v, op, f3, rs1, rs2, imm);
            v = enc_S(op, f3, rs1, rs2, newimm);
        } else if (kind == "B") {
            dec_B(v, op, f3, rs1, rs2, imm);
            v = enc_B(op, f3, rs1, rs2, newimm);
        } else if (kind == "U") {
            dec_U(v, op, rd, imm);
            v = enc_U(op, rd, imm);
        } else {
            assert(false);
        }
    }

    return res;
}


}
