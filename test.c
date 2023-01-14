/*
Função principal para realização da análise sintática.

Este arquivo será posterioremente substituído, não acrescente nada.
*/
#include <stdio.h>
#define _(s) #s // https://gcc.gnu.org/onlinedocs/gcc-12.2.0/cpp/Stringizing.html
#include "parser.tab.h" //arquivo gerado com bison -d parser.y

#define bool int
#define true 1
#define false 0

typedef struct yy_buffer_state * YY_BUFFER_STATE;

extern int yylex(void);
extern YY_BUFFER_STATE yy_scan_string(char * string);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yylex_destroy(void);
extern int yylineno;

extern char *yytext;
extern int get_line_number (void);
extern int error_message_enabled;

/* protótipos deste módulo - as implementações estão após a main */
int print_token (int token);
int test_token(char* string, int expected_token, bool assertion);
int test_line_numbers(char* string, int expected_line_number);
int test_parse (char* string, int assertion);

int main (int argc, char **argv)
{
	int test_result = true;
	error_message_enabled = 0;

    // EMPTY PROGRAM
	test_result = test_result && test_parse("", true);

    // DEC VAR GLOBAL
	test_result = test_result && test_parse("int aa;", true);
	test_result = test_result && test_parse("int az[13^5];", true);
	test_result = test_result && test_parse("int aa, ab, ac;", true);
	test_result = test_result && test_parse("float bb;", true);
	test_result = test_result && test_parse("float bz[13^5];", true);
	test_result = test_result && test_parse("float bb, bc, bd;", true);
	test_result = test_result && test_parse("char cc;", true);
	test_result = test_result && test_parse("char cz[13^5];", true);
	test_result = test_result && test_parse("char cc, cd, ce;", true);
	test_result = test_result && test_parse("bool dd;", true);
	test_result = test_result && test_parse("bool cz[13^5];", true);
	test_result = test_result && test_parse("bool dd, de, df;", true);
	test_result = test_result && test_parse("int a = 1;", false);

    // FUNCOES
	test_result = test_result && test_parse(""
        "int fA (){\n"
        "}"
        , true);
	test_result = test_result && test_parse(""
        "float fB (){\n"
        "}"
        , true);
	test_result = test_result && test_parse(""
        "char fC (){\n"
        "}"
        , true);
	test_result = test_result && test_parse(""
        "bool fD (){\n"
        "}"
        , true);
    
    if (!test_result)
        printf("\n\033[0;31mSOME TESTS FAILED!\033[0m\n\n");
    else
        printf("\n\033[0;32mALL TESTS PASSED!\033[0m\n\n");

	return 0;
}


int test_line_numbers(char* string, int expected_line_number) {
    YY_BUFFER_STATE buffer = yy_scan_string(string);
    while (yylex()) { }
    yy_delete_buffer(buffer);
    int line_number = get_line_number();
    yylineno = 0;

    printf("\nLine number is %d, expected is %d. ", line_number, expected_line_number);
    if (expected_line_number != line_number){	 
        printf("\033[0;31mFAILED!\033[0m");
        return false;
    }
    printf("\033[0;32mPASSED!\033[0m");
    return true;
}


int test_token(char* string, int expected_token, bool assertion) {
    YY_BUFFER_STATE buffer = yy_scan_string(string);
    int tk = yylex();

    if (yylex() != 0){
        printf("\nTesting %s. More than one token present. ", string);
        if (!assertion) {
            printf("Failure expected. \033[0;32mPASSED!\033[0m");
            return true;
        } else {
            printf("\033[0;31mFAILED!\033[0m");
            return false;
        }
    }

    printf("\nTesting %s. Token is %d, expected is %d. ", string, tk, expected_token);

    yy_delete_buffer(buffer);

    if (expected_token == tk) {
        if (assertion) {
            printf("\033[0;32mPASSED!\033[0m");
            return true;
        } else {
            printf("Success not expexted. \033[0;31mFAILED!\033[0m");
            return false;
        }
    } else {
        if (!assertion) {
            printf("Failure expected. \033[0;32mPASSED!\033[0m");
            return true;
        } else {
            printf("\033[0;31mFAILED!\033[0m");
            return false;
        }
    }
}


int test_parse (char* string, int assertion) {
    YY_BUFFER_STATE buffer = yy_scan_string(string);
    int ret = yyparse();
    yy_delete_buffer(buffer);
    yylex_destroy();
    printf("\n-----------input start-----------\n%s\n------------input end------------\nResult: ", string);
    if (ret==1) { // Failure
        if(assertion) { // Success expected
            printf("\033[0;31mFAILED!\033[0m\n");
            return false;
        }
        else { // Failure expected
            printf("Failure expected. \033[0;32mPASSED!\033[0m\n");
            return true;
        }
    }
    else { // Success
        if(assertion) { // Success expected
            printf("\033[0;32mPASSED!\033[0m\n");
            return true;
        }
        else { // Failure expected
            printf("Success not expexted. \033[0;31mFAILED!\033[0m\n");
            return false;
        }
    }
}
