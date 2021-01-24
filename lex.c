#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CMP_INST(inst, str) strncmp(inst, str, sizeof(inst)-1)

#define MATCH(str, row, col) do {printf("Matched %s at %d, %d\n", str, row, col);} while(0)

enum {
    CARVE_TOK_EOF = 0,
    CARVE_TOK_IDENT,

    CARVE_TOK_STR,
    CARVE_TOK_CHR,

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
    int kind, len;
    long pos;
};
typedef struct Token carve_tok;

int lex(char* src, carve_tok** toks_p) {
    #define toks (*toks_p)
    #define ADD_TOK(k, l) do {\
        toks = realloc(toks, (++n_tok) * sizeof(*toks));\
        toks[n_tok-1].kind = k; toks[n_tok-1].len = l; toks[n_tok-1].pos = cur - src;\
        } while(0)
    #define MATCH_CHAR(k) ADD_TOK(k, 1); col++;
    #define ADV(cur) do {col++; cur++;} while (0)
    #define CRASH(msg) do {fprintf(stderr, msg " @ (%d, %d)\n", col, line); return -1;} while (0)
    
    char *cur = src, *t_cur, cur_c;
    int line = 0, col = 0, n_tok = 0, len;
    int i;

    while ((cur_c = *cur) != '\0') {
        switch (cur_c) {
            case ':':
                MATCH_CHAR(CARVE_TOK_COL);
                break;

            case '.':
                MATCH_CHAR(CARVE_TOK_DOT);
                break;

            case ',':
                MATCH_CHAR(CARVE_TOK_COM);
                break;

            case '\n':
                MATCH_CHAR(CARVE_TOK_NEWLINE);
                break;

            case '-':
                MATCH_CHAR(CARVE_TOK_MIN);
                break;

            case '+':
                MATCH_CHAR(CARVE_TOK_PLS);
                break;

            case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case'9':
                t_cur = cur;
                int is_float = 0, is_hex = 0;

                /* handle hex */
                if ((cur_c == '0') && (*(cur + 1) == 'x' || *(cur + 1) == 'X')) {
                    ADV(t_cur); ADV(t_cur); is_hex = 1;
                }
                /* accept numerics and a single period */
                while (((cur_c = *t_cur) >= '0' && cur_c <= '9') || cur_c == '.') {
                    if (cur_c == '.') {
                        if (is_float || is_hex) {
                            CRASH("Invalid float");
                        }
                        else {
                            is_float = 1;
                        }
                    }
                    ADV(t_cur);
                }
                if (is_float) {
                    ADD_TOK(CARVE_TOK_FLOAT, t_cur - cur);
                }
                else {
                    ADD_TOK(CARVE_TOK_INT, t_cur - cur);
                }

                cur = t_cur - 1;
                break;

            case ' ': case '\t': case '\r': case '\f':
                col++;
                break;

            case '\"':
                ADV(cur);
                t_cur = cur;
                while ((cur_c = *t_cur) != '\0') {
                    if (cur_c == '\"') {
                        ADD_TOK(CARVE_TOK_IDENT, t_cur - cur);
                        break;
                    }
                    ADV(t_cur);
                }
                if (cur_c == '\0') CRASH("Unended \"");

                cur = t_cur;
                ADV(cur);
                break;
            
            case '\'':
                switch(cur[1]) {
                    case '\'': CRASH("Empty character literal"); break;
                    case '\0': CRASH("Uncaught '"); break;
                    case '\\':
                        switch(cur[2]) {
                            case '\'':case 'a':case 'b':case 't':case 'n':case 'v':case 'f': case 'r':
                                if (cur[3] == '\'') {
                                    ADD_TOK(CARVE_TOK_CHR, 4);
                                }
                                else {
                                    CRASH ("Uncaught '");
                                }
                                break;
                            default:
                                CRASH("Invalid escape code");
                        }
                    default:
                        if (cur[2] == '\'') {
                            ADD_TOK(CARVE_TOK_CHR, 3);
                        }
                }
                if (cur[1] == '\0') CRASH("Unended '");

                for (i = 0; i < toks[n_tok - 1].len; i++) {
                    ADV(cur);
                }

                break;
            
            default:
                t_cur = cur;
                while ((cur_c = *t_cur) != '\0') {
                    if (cur_c=='\n'||cur_c==' '||cur_c=='\t'||cur_c=='\r'||cur_c=='\f'||cur_c==':') {
                        ADD_TOK(CARVE_TOK_IDENT, t_cur - cur);
                        break;
                    }

                    ADV(t_cur);
                }
                cur = t_cur - 1;
                break;
        }
        cur++;
    }
    MATCH_CHAR(CARVE_TOK_EOF);

    printf("Found %d toks\n", n_tok);
    for (i = 0; i < n_tok; i++) {
        printf("@%d: \"%.*s\": %d\n", toks[i].pos, toks[i].len, src + toks[i].pos, toks[i].kind);
    }

    #undef toks
    #undef ADD_TOK
    #undef ADV
    #undef CRASH
}

void parse(char* src) {
    carve_tok* toks = malloc(0);
    carve_tok** toks_p = &toks;
    lex(src, toks_p);
    free(toks);
    return;
    char *cur = src, *tmp, *left;
    char cur_c;
    int line = 0, col = 0;

    while ((cur_c = *cur) != '\0') {
        switch (cur_c) {
            case '\n':
                col = 0;
                line++;
                cur++;
                continue;

            case ' ': case '\t': case '\r': case '\f':
                break;

            case ';':
                while (*cur != '\n') cur++;
                continue;
            default:
                if (!CMP_INST("test", cur)) {
                    cur += sizeof("test ");
                    col += sizeof("test ");
                    MATCH("test ", line, col);
                }
                else {
                    /* Try to match label 
                    */
                    left = cur;
                    while ((cur_c = *cur) != '\n') {
                        if (cur_c == ' ' || cur_c == ':') {
                            tmp = malloc(cur - left);
                            strncpy(tmp, left, (cur - 1) - left);
                            tmp[cur - left - 1] = '\0';
                            
                            while ((cur_c = *cur) == ' ') {cur++; col++;}
                            if (cur_c == ':') MATCH(tmp, line, col);
                            else {
                                free(tmp);
                                break;
                            }
                        }
                        cur++; col++;
                    }
                    printf("Uncaught error @ %d %d\n", line, col);
                }
                
                break;
        }
        col++;
        cur++;
    }

    printf("Done!\n");
}



int main() {
    char    *buffer;
    size_t   n = 1024;
    buffer = malloc(n);
    char     src[1024];
    char*    cur = src;

    while (getline(&buffer, &n, stdin) > 0) {
        cur += sprintf(cur, "%s", buffer);
    }
    printf("Initiating parsing...\n");
    parse(src);

    free(buffer);
    return 0;
}