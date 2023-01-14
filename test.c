/*
Função principal para realização da análise sintática.

Este arquivo será posterioremente substituído, não acrescente nada.
*/
#include <stdio.h>
#include <string.h>
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
char com_bloc_output[200];
char* com_bloc(char* input);

int main (int argc, char **argv)
{
	bool test_result = true;
	error_message_enabled = 1;

    test_result = test_result

    // EMPTY PROGRAM
	&& test_parse("", true)

    // DEC VAR GLOBAL

        //ints
        && test_parse("int aa;", true)
        && test_parse("int aa, ab, ac;", true)
        && test_parse("int aa[]", false)
        && test_parse("int az[1];", true)
        && test_parse("int az[13^5];", true)
        && test_parse("int az[13^5^8];", true)
        && test_parse("int aa[1], ab[2^3], ac[2^3^4];", true)

        //floats
        && test_parse("float aa;", true)
        && test_parse("float aa, ab, ac;", true)
        && test_parse("float aa[]", false)
        && test_parse("float az[1];", true)
        && test_parse("float az[13^5];", true)
        && test_parse("float az[13^5^8];", true)
        && test_parse("float aa[1], ab[2^3], ac[2^3^4];", true)

        //char
        && test_parse("char aa;", true)
        && test_parse("char aa, ab, ac;", true)
        && test_parse("char aa[]", false)
        && test_parse("char az[1];", true)
        && test_parse("char az[13^5];", true)
        && test_parse("char az[13^5^8];", true)
        && test_parse("char aa[1], ab[2^3], ac[2^3^4];", true)

        //bool
        && test_parse("int aa;", true)
        && test_parse("int aa, ab, ac;", true)
        && test_parse("int aa[]", false)
        && test_parse("int az[1];", true)
        && test_parse("int az[13^5];", true)
        && test_parse("int az[13^5^8];", true)
        && test_parse("int aa[1], ab[2^3], ac[2^3^4];", true)

    // DEC VAR LOCAL

        //ints
        && test_parse(com_bloc("int a;"), true)
        && test_parse(com_bloc("int a, b, c;"), true)
        && test_parse(com_bloc("int a <= 2;"), true)
        && test_parse(com_bloc("int a, b, c <= 2;"), true)
        && test_parse(com_bloc("int a <= 2, b <= 2, c <= 2;"), true)

        //floats
        && test_parse(com_bloc("float a;"), true)
        && test_parse(com_bloc("float a, b, c;"), true)
        && test_parse(com_bloc("float a <= 2;"), true)
        && test_parse(com_bloc("float a, b, c <= 2;"), true)
        && test_parse(com_bloc("float a <= 2, b <= 2, c <= 2;"), true)

        //chars
        && test_parse(com_bloc("char a;"), true)
        && test_parse(com_bloc("char a, b, c;"), true)
        && test_parse(com_bloc("char a <= 2;"), true)
        && test_parse(com_bloc("char a, b, c <= 2;"), true)
        && test_parse(com_bloc("char a <= 2, b <= 2, c <= 2;"), true)

        //bools
        && test_parse(com_bloc("bool a;"), true)
        && test_parse(com_bloc("bool a, b, c;"), true)
        && test_parse(com_bloc("bool a <= 2;"), true)
        && test_parse(com_bloc("bool a, b, c <= 2;"), true)
        && test_parse(com_bloc("bool a <= 2, b <= 2, c <= 2;"), true)

    // FUNCOES
    
        //ints
        && test_parse("int a () {};", false)
        && test_parse("int a (int a) {}", true)
        && test_parse("int a (int a, float b) {}", true)
        && test_parse("int a (int a, float b, char c) {}", true)

        //floats
        && test_parse("float a () {};", false)
        && test_parse("float a (int a) {}", true)
        && test_parse("float a (int a, float b) {}", true)
        && test_parse("float a (int a, float b, char c) {}", true)

        //chars
        && test_parse("char a () {};", false)
        && test_parse("char a (int a) {}", true)
        && test_parse("char a (int a, float b) {}", true)
        && test_parse("char a (int a, float b, char c) {}", true)

        //bools
        && test_parse("bool a () {};", false)
        && test_parse("bool a (int a) {}", true)
        && test_parse("bool a (int a, float b) {}", true)
        && test_parse("bool a (int a, float b, char c) {}", true)

    // BLOCO DE COMANDOS

        && test_parse(com_bloc("{}"), true)
        && test_parse(com_bloc("{{{{{}}}}}"), true)
        && test_parse(com_bloc("foo = bar; func(); if(a) then {foo = bar; while(1){doNothing();}}"), true)

    // ATRIBUICOES

        && test_parse(com_bloc("foo = bar;"), true)
        && test_parse(com_bloc("foo = bar"), false)
        && test_parse(com_bloc("foo = 123545;"), true)
        && test_parse(com_bloc("foo = !(1234*abcd+47);"), true)
        && test_parse(com_bloc("int vet[1];"), false)
        && test_parse(com_bloc("vet[a^2^-(1234*abcd+47)] = 3;"), true)
        && test_parse(com_bloc("var[var[10^20^30*2^var[20^30/2^40]]] = 5;"), true)

    // CHAMADA DE FUNCAO

        && test_parse(com_bloc("func();"), true)
        && test_parse(com_bloc("func()"), false)
        && test_parse(com_bloc("func(1, a, -(1234*abcd+47));"), true)

    // EXPRESSOES

        && test_parse(com_bloc("a = -(1234*!abcd+47);"), true)
        && test_parse(com_bloc("a = !(1234*-abcd+47);"), true)
        && test_parse(com_bloc("a = vet[a^2^-(1234*abcd+47)];"), true)
        && test_parse(com_bloc("a = b;"), true)
        && test_parse(com_bloc("a = func();"), true)
        && test_parse(com_bloc("a = (1234*-abcd+47/9) \% (1234*-abcd+47/9) == var*5 != 10\%2;"), true)
        && test_parse(com_bloc("a = (1234*-abcd+47/9) <= (1234*-abcd+47/9) >= var*5 != 10\%2;"), true)
        && test_parse(com_bloc("a = (1234*-abcd+47/9) && (1234*-abcd+47/9) || var*5 != 10\%2;"), true)

    // CONTROLE DE FLUXO

        //fluxos no global
        && test_parse("if ()", false)
        && test_parse("if () then", false)
        && test_parse("if () then {}", false)
        && test_parse("if () then {};", false)        
        && test_parse("if (a)", false)
        && test_parse("if (a) then", false)
        && test_parse("if (a) then {}", false)
        && test_parse("if (a) then {};", false)        
        && test_parse("if (!a)", false)
        && test_parse("if (!a) then", false)
        && test_parse("if (!a) then {}", false)
        && test_parse("if (!a) then {};", false)
        && test_parse("if (a) then {} else", false)
        && test_parse("if (a) then {} else {}", false)
        && test_parse("if (a) then {} else {};", false)        
        && test_parse("if (!a) then {} else", false)
        && test_parse("if (!a) then {} else {}", false)
        && test_parse("if (!a) then {} else {};", false)
        && test_parse("while", false)
        && test_parse("while ()", false)
        && test_parse("while () {}", false)
        && test_parse("while () {};", false)
        && test_parse("while (a)", false)
        && test_parse("while (a) {}", false)
        && test_parse("while (a) {};", false)
        && test_parse("while (!a)", false)
        && test_parse("while (!a) {}", false)
        && test_parse("while (!a) {};", false)
        
        //if-then
        && test_parse(com_bloc("if ()"), false)
        && test_parse(com_bloc("if () then"), false)
        && test_parse(com_bloc("if () then {}"), false)
        && test_parse(com_bloc("if () then {};"), false)        
        && test_parse(com_bloc("if (a)"), false)
        && test_parse(com_bloc("if (a) then"), false)
        && test_parse(com_bloc("if (a) then {}"), true)
        && test_parse(com_bloc("if (a) then {};"), false)        
        && test_parse(com_bloc("if (!a)"), false)
        && test_parse(com_bloc("if (!a) then"), false)
        && test_parse(com_bloc("if (!a) then {}"), true)
        && test_parse(com_bloc("if (!a) then {};"), false)
        
        //if-then-else      
        && test_parse(com_bloc("if (a) then {} else"), false)
        && test_parse(com_bloc("if (a) then {} else {}"), true)
        && test_parse(com_bloc("if (a) then {} else {};"), false)        
        && test_parse(com_bloc("if (!a) then {} else"), false)
        && test_parse(com_bloc("if (!a) then {} else {}"), true)
        && test_parse(com_bloc("if (!a) then {} else {};"), false)

        //while
        && test_parse(com_bloc("while"), false)
        && test_parse(com_bloc("while ()"), false)
        && test_parse(com_bloc("while () {}"), false)
        && test_parse(com_bloc("while () {};"), false)
        && test_parse(com_bloc("while (a)"), false)
        && test_parse(com_bloc("while (a) {}"), true)
        && test_parse(com_bloc("while (a) {};"), false)
        && test_parse(com_bloc("while (!a)"), false)
        && test_parse(com_bloc("while (!a) {}"), true)
        && test_parse(com_bloc("while (!a) {};"), false)
    
    // RETURN

        && test_parse(com_bloc("return;"), false)
        && test_parse(com_bloc("return 1;"), true)
        && test_parse(com_bloc("return a;"), true)
        && test_parse(com_bloc("return -(1234*abcd+47);"), true)

    ;
	
    if (!test_result)
        printf("\n\033[0;31mSOME TESTS FAILED!\033[0m\n\n");
    else
        printf("\n\033[0;32mALL TESTS PASSED!\033[0m\n\n");

	return 0;
}

char* com_bloc (char* input)
{
    char output[200] = "int commBlock () { \n";

    strcat(output, (const char*)input);
    strcat(output,"\n }");

    strcpy(com_bloc_output,output);

    return com_bloc_output;
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
    printf("\n-----------input start-----------\n%s\n------------input end------------\nResult: ", string);
    int ret = yyparse();
    yy_delete_buffer(buffer);
    yylex_destroy();
    
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
