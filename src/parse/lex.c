#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef CARVE_PARSE_H
#include "parse.h"
#define CARVE_PARSE_H
#endif

#define CMP_INST(inst, str) strncmp(inst, str, sizeof(inst)-1)

#define MATCH(str, row, col) do {printf("Matched %s at %d, %d\n", str, row, col);} while(0)

#define VALID_NUMERIC(c) (c >= '0' && c <= '9')
#define VALID_HEX(c) (VALID_NUMERIC(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
#define VALID_ALPHA(c) ((c>='a'&&c<='z')||(c>='A'&&c<='Z'))

int lex(char* src, carve_tok** toks_p) {
    #define toks (*toks_p)
    #define ADD_TOK(k, len) do {\
        toks = realloc(toks, (++n_tok) * sizeof(*toks));\
        toks[n_tok-1] = (carve_tok) {k, len, line, col - len, cur - src};\
        } while(0)
    #define MATCH_CHAR(k) ADD_TOK(k, 1); col++;
    #define ADV(cur) do {col++; cur++;} while (0)
    #define CRASH(msg) do {fprintf(stderr, msg " @ (%d, %d)\n", col, line); return -1;} while (0)
    
    char *cur = src, *t_cur, cur_c;
    int line = 0, col = 0, n_tok = 0, len;
    int i;

    /* loop through all characters */
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

            case '(':
                MATCH_CHAR(CARVE_TOK_LPAR);
                break;

            case ')':
                MATCH_CHAR(CARVE_TOK_RPAR);
                break;

            case '+':
                MATCH_CHAR(CARVE_TOK_PLS);
                break;

            /* all numeric tokens */
            case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case'9':
                t_cur = cur;
                int is_float = 0, is_hex = 0;

                /* handle hex */
                if ((cur_c == '0') && (*(cur + 1) == 'x' || *(cur + 1) == 'X')) {
                    ADV(t_cur); ADV(t_cur); is_hex = 1;
                }
                /* accept numerics (or a-f if hex) and a single period */
                while (((cur_c = *t_cur), VALID_NUMERIC(cur_c) || cur_c == '.' || (is_hex && VALID_HEX(cur_c)))) {
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
                    ADD_TOK(CARVE_TOK_FLOAT, ((int) (t_cur - cur)));
                }
                else {
                    ADD_TOK(CARVE_TOK_INT, ((int) (t_cur - cur)));
                }

                cur = t_cur - 1;
                break;

            /* ignore whitespace */
            case ' ': case '\t': case '\r': case '\f':
                col++;
                break;

            /* string literals */
            case '\"':
                ADV(cur);
                t_cur = cur;
                while ((cur_c = *t_cur) != '\0') {
                    if (cur_c == '\"' && t_cur[-1] != '\\') {
                        ADD_TOK(CARVE_TOK_IDENT, ((int) (t_cur - cur)));
                        break;
                    }
                    ADV(t_cur);
                }
                if (cur_c == '\0') CRASH("Unended \"");

                cur = t_cur;
                ADV(cur);
                break;
            
            /* char literals */
            case '\'':
                switch(cur[1]) {
                    case '\'': CRASH("Empty character literal"); break;
                    case '\0': CRASH("Uncaught '"); break;
                    /* catch escape codes */
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
                    /* otherwise, accept single char */
                    default:
                        if (cur[2] == '\'') {
                            ADD_TOK(CARVE_TOK_CHR, 3);
                        }
                }
                if (cur[1] == '\0') CRASH("Unended '");

                /* advance cursor by amount matched */
                for (i = 0; i < toks[n_tok - 1].len; i++) {
                    ADV(cur);
                }

                break;
            
            /* otherwise, interpret as an identifier */
            default:
                t_cur = cur;
                /* adv while a valid identifier digit */
                while ((cur_c = *t_cur), VALID_ALPHA(cur_c)||VALID_NUMERIC(cur_c)||(cur_c=='_')||(cur_c=='.')) {
                    ADV(t_cur);
                }
                if (t_cur == cur) CRASH("Uncaught token");
                ADD_TOK(CARVE_TOK_IDENT, ((int) (t_cur - cur)));
                cur = t_cur - 1;
                break;
        }
        cur++;
    }
    /* match EOF and return */
    MATCH_CHAR(CARVE_TOK_EOF);

    printf("Found %d toks\n", n_tok);
    for (i = 0; i < n_tok; i++) {
        printf("@(%d, %d): \"%.*s\": %d\n", toks[i].line, toks[i].col, toks[i].len, src + toks[i].pos, toks[i].kind);
    }

    return n_tok;

    #undef toks
    #undef ADD_TOK
    #undef ADV
    #undef CRASH
}

void parse(char* src) {
    #define CRASH(msg) do {fprintf(stderr, msg " @(%d, %d)\n", tok->line, tok->col); free(toks); return;} while(0)

    carve_tok* toks = NULL;
    carve_tok** toks_p = &toks;
    int n_tok = lex(src, toks_p);

    carve_tok* tok = *toks_p;

    while (tok->kind != CARVE_TOK_EOF) {
        switch (tok->kind) {
            case CARVE_TOK_IDENT:
                /* label */
                if (tok[1].kind == CARVE_TOK_COL) {
                }
                /* instruction */
                else {
                    //check_inst(tok);
                }

                /* otherwise, throw error */
                CRASH("Uncaught identifier");
                break;
            
            case CARVE_TOK_DOT:
                break;

            case CARVE_TOK_NEWLINE:
                break;

            default:
                CRASH("Uncaught token");
                break;
        }
    }

    free(toks);
    return;
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