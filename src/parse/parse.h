// all possible tokens
enum {
    CARVE_TOK_EOF = 0,
    CARVE_TOK_IDENT,

    CARVE_TOK_STR,
    CARVE_TOK_CHR,

    CARVE_TOK_LPAR,
    CARVE_TOK_RPAR,

    CARVE_TOK_COL,
    CARVE_TOK_DOT,
    CARVE_TOK_COM,
    CARVE_TOK_PLS,
    CARVE_TOK_MIN,

    CARVE_TOK_INT,
    CARVE_TOK_FLOAT,
    CARVE_TOK_NEWLINE
};

struct Token {
    int kind, len, line, col;
    long pos;
};
typedef struct Token carve_tok;

struct Inst {
    char* inst;
    int type;
};
typedef struct Inst carve_inst;

void parse(char* src);

int lex(char* src, carve_tok** toks_p);

int check_inst(carve_tok* tok);