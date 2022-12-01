/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include <stdio.h>
#define _(s) #s // https://gcc.gnu.org/onlinedocs/gcc-12.2.0/cpp/Stringizing.html
#include "tokens.h"

#define bool int
#define true 1
#define false 0

typedef struct yy_buffer_state * YY_BUFFER_STATE;

extern int yylex(void);
extern YY_BUFFER_STATE yy_scan_string(char * string);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yylex_destroy(void);

extern char *yytext;
extern int get_line_number (void);

/* protótipos deste módulo - as implementações estão após a main */
int print_token (int token);
int test_token(char* string, int expected_token, bool assertion);

int main (int argc, char **argv) {
    
    // TK_PR_INT
    test_token("int", TK_PR_INT, true);

    // TK_PR_FLOAT
    test_token("float", TK_PR_FLOAT, true);

    // TK_PR_BOOL
    test_token("bool", TK_PR_BOOL, true);

    // TK_PR_CHAR
    test_token("char", TK_PR_CHAR, true);

    // TK_PR_IF
    test_token("if", TK_PR_IF, true);

    // TK_PR_THEN
    test_token("then", TK_PR_THEN, true);

    // TK_PR_ELSE
    test_token("else", TK_PR_ELSE, true);

    // TK_PR_WHILE
    test_token("while", TK_PR_WHILE, true);

    // TK_PR_INPUT
    test_token("input", TK_PR_INPUT, true);

    // TK_PR_OUTPUT
    test_token("output", TK_PR_OUTPUT, true);

    // TK_PR_RETURN
    test_token("return", TK_PR_RETURN, true);

    // TK_PR_FOR
    test_token("for", TK_PR_FOR, true);

    // TK_OC_LE
    test_token("<=", TK_OC_LE, true);

    // TK_OC_GE
    test_token(">=", TK_OC_GE, true);

    // TK_OC_EQ
    test_token("==", TK_OC_EQ, true);

    // TK_OC_NE
    test_token("!=", TK_OC_NE, true);

    // TK_OC_AND
    test_token("&&", TK_OC_AND, true);

    // TK_OC_OR
    test_token("||", TK_OC_OR, true);

    // TK_LIT_INT
    test_token("1", TK_LIT_INT, true);
    test_token("-1", TK_LIT_INT, true);
    test_token("+1", TK_LIT_INT, true);
    test_token("1234567890", TK_LIT_INT, true);
    test_token("+1234567890", TK_LIT_INT, true);
    test_token("-1234567890", TK_LIT_INT, true);

    // TK_LIT_FLOAT
    test_token("1.", TK_LIT_FLOAT, false); // Invalid
    test_token("1.+0", TK_LIT_FLOAT, false); // Invalid
    test_token("1.0", TK_LIT_FLOAT, true);
    test_token("12345.012345", TK_LIT_FLOAT, true);
    test_token("+12345.012345", TK_LIT_FLOAT, true);
    test_token("-12345.012345", TK_LIT_FLOAT, true);

    // TK_LIT_FALSE
    test_token("false", TK_LIT_FALSE, true);

    // TK_LIT_TRUE
    test_token("true", TK_LIT_TRUE, true);

    // TK_LIT_CHAR
    test_token("\'\'", TK_LIT_CHAR, true);
    test_token("\'a\'", TK_LIT_CHAR, true);
    test_token("\'~\'", TK_LIT_CHAR, true);
    test_token("\'aa\'", TK_ERRO, false); // Invalid

    // TK_IDENTIFICADOR
    test_token("abcd", TK_IDENTIFICADOR, true);
    test_token("ABCD", TK_IDENTIFICADOR, true);
    test_token("abcdABCD", TK_IDENTIFICADOR, true);
    test_token("abcd_ABCD", TK_IDENTIFICADOR, false); // Invalid
    
    yylex_destroy();
    printf("\n");
    return 0;
}

int test_token(char* string, int expected_token, bool assertion) {
    YY_BUFFER_STATE buffer = yy_scan_string(string);
    int tk = yylex();

    if (yylex() != 0){
        printf("\nTesting %s. More than one token present. ", string);
        if (!assertion) {
            printf("Failure expected. \033[0;32mSUCCESS!\033[0m");
            return 1;
        } else {
            printf("\033[0;31mFAILURE!\033[0m");
            return -1;
        }
    }

    printf("\nTesting %s. Token is %d, expected is %d. ", string, tk, expected_token);

    yy_delete_buffer(buffer);

    if (expected_token == tk) {
        if (assertion) {
            printf("\033[0;32mSUCCESS!\033[0m");
            return 1;
        } else {
            printf("Success not expexted. \033[0;31mFAILURE!\033[0m");
            return -1;
        }
    } else {
        if (!assertion) {
            printf("Failure expected. \033[0;32mSUCCESS!\033[0m");
            return 1;
        } else {
            printf("\033[0;31mFAILURE!\033[0m");
            return -1;
        }
    }
}
