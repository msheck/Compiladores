/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira

Arquivo contendo testes automatizados. Para executá-los utilize "make test".
*/

#include <stdio.h>
#include <string.h>
#define _(s) #s // https://gcc.gnu.org/onlinedocs/gcc-12.2.0/cpp/Stringizing.html
#include "parser.tab.h" //arquivo gerado com bison -d parser.y

#define bool int
#define true 1
#define false 0

typedef struct yy_buffer_state * YY_BUFFER_STATE;

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

extern int yyparse(void);
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
int test_parse (char* string, int assertion, int line_number);
char failed_tests[200];
char com_bloc_output[200];
char* com_bloc(char* input);
int test_number = 0;

int main (int argc, char **argv)
{
	bool test_result = 0;
	error_message_enabled = 1;

    test_result = test_result

    // EMPTY PROGRAM
	    + test_parse("", true, __LINE__)

    // DEC VAR GLOBAL

        //ints
        + test_parse("int;", false, __LINE__)
        + test_parse("int aa;", true, __LINE__)
        + test_parse("int aa, ab, ac;", true, __LINE__)
        + test_parse("int aa[]", false, __LINE__)
        + test_parse("int aa[0.5]", false, __LINE__)
        + test_parse("int aa[ba]", false, __LINE__)
        + test_parse("int aa[1];", true, __LINE__)
        + test_parse("int aa[13^5];", true, __LINE__)
        + test_parse("int aa[13^5^8];", true, __LINE__)
        + test_parse("int aa[1], ab[2^3], ac[2^3^4];", true, __LINE__)
        + test_parse("int aa <= 0;", false, __LINE__)
        + test_parse("int aa = 0;", false, __LINE__)

        //floats
        + test_parse("float;", false, __LINE__)
        + test_parse("float ba;", true, __LINE__)
        + test_parse("float ba, bb, bc;", true, __LINE__)
        + test_parse("float ba[]", false, __LINE__)
        + test_parse("float ba[0.5]", false, __LINE__)
        + test_parse("float ba[bb]", false, __LINE__)
        + test_parse("float ba[1];", true, __LINE__)
        + test_parse("float ba[13^5];", true, __LINE__)
        + test_parse("float ba[13^5^8];", true, __LINE__)
        + test_parse("float ba[1], bb[2^3], bc[2^3^4];", true, __LINE__)
        + test_parse("float ba <= 0;", false, __LINE__)
        + test_parse("float ba = 0;", false, __LINE__)

        //char
        + test_parse("char;", false, __LINE__)
        + test_parse("char ca;", true, __LINE__)
        + test_parse("char ca, cb, cc;", true, __LINE__)
        + test_parse("char ca[]", false, __LINE__)
        + test_parse("char ca[0.5]", false, __LINE__)
        + test_parse("char ca[cb]", false, __LINE__)
        + test_parse("char ca[1];", true, __LINE__)
        + test_parse("char ca[13^5];", true, __LINE__)
        + test_parse("char ca[13^5^8];", true, __LINE__)
        + test_parse("char ca[1], cb[2^3], cc[2^3^4];", true, __LINE__)
        + test_parse("char ca <= 0;", false, __LINE__)
        + test_parse("char ca = 0;", false, __LINE__)

        //bool
        + test_parse("bool;", false, __LINE__)
        + test_parse("bool da;", true, __LINE__)
        + test_parse("bool da, db, dc;", true, __LINE__)
        + test_parse("bool da[]", false, __LINE__)
        + test_parse("bool da[0.5]", false, __LINE__)
        + test_parse("bool da[db]", false, __LINE__)
        + test_parse("bool da[1];", true, __LINE__)
        + test_parse("bool da[13^5];", true, __LINE__)
        + test_parse("bool da[13^5^8];", true, __LINE__)
        + test_parse("bool da[1], db[2^3], dc[2^3^4];", true, __LINE__)
        + test_parse("bool da <= 0;", false, __LINE__)
        + test_parse("bool da = 0;", false, __LINE__)

    // FUNCOES
    
        //ints
        + test_parse("int fA ();", false, __LINE__)
        + test_parse("int fA () {};", false, __LINE__)
        + test_parse("int fA (int a) {}", true, __LINE__)
        + test_parse("int fA (int a, float b) {}", true, __LINE__)
        + test_parse("int fA (int a, float b, char c) {}", true, __LINE__)

        //floats
        + test_parse("float fB ();", false, __LINE__)
        + test_parse("float fB () {};", false, __LINE__)
        + test_parse("float fB (int a) {}", true, __LINE__)
        + test_parse("float fB (int a, float b) {}", true, __LINE__)
        + test_parse("float fB (int a, float b, char c) {}", true, __LINE__)

        //chars
        + test_parse("char fC ();", false, __LINE__)
        + test_parse("char fC () {};", false, __LINE__)
        + test_parse("char fC (int a) {}", true, __LINE__)
        + test_parse("char fC (int a, float b) {}", true, __LINE__)
        + test_parse("char fC (int a, float b, char c) {}", true, __LINE__)

        //bools
        + test_parse("bool fD ();", false, __LINE__)
        + test_parse("bool fD () {};", false, __LINE__)
        + test_parse("bool fD (int a) {}", true, __LINE__)
        + test_parse("bool fD (int a, float b) {}", true, __LINE__)
        + test_parse("bool fD (int a, float b, char c) {}", true, __LINE__)

    // BLOCO DE COMANDOS

        + test_parse(com_bloc("{}"), false, __LINE__)
        + test_parse(com_bloc("{{{{{}}}}}"), false, __LINE__)
        + test_parse(com_bloc("foo = bar; func(); if(a) then {foo = bar; while(1){doNothing();}}"), false, __LINE__)
        + test_parse(com_bloc("{};"), true, __LINE__)
        + test_parse(com_bloc("{{{{{};};};};};"), true, __LINE__)
        + test_parse(com_bloc("foo = bar; func(); if(a) then {foo = bar; while(1){doNothing();};};"), true, __LINE__)

    // DEC VAR LOCAL

        //ints
        + test_parse(com_bloc("int a;"), true, __LINE__)
        + test_parse(com_bloc("int a, b, c;"), true, __LINE__)
        + test_parse(com_bloc("int a <= 2;"), true, __LINE__)
        + test_parse(com_bloc("int a, b, c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("int a <= 2, b <= 2, c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("int a[];"), false, __LINE__)
        + test_parse(com_bloc("int a[1];"), false, __LINE__)
        + test_parse(com_bloc("int a[1^2];"), false, __LINE__)
        + test_parse(com_bloc("int a <= '2', b <= '2', c <= '2';"), true, __LINE__)
        + test_parse(com_bloc("int a <= '2';"), true, __LINE__)
        + test_parse(com_bloc("int a <= 2 + 3;"), false, __LINE__)

        //floats
        + test_parse(com_bloc("float b;"), true, __LINE__)
        + test_parse(com_bloc("float b, b, c;"), true, __LINE__)
        + test_parse(com_bloc("float b <= 2;"), true, __LINE__)
        + test_parse(com_bloc("float b, b, c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("float b <= 2, b <= 2, c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("float b[];"), false, __LINE__)
        + test_parse(com_bloc("float b[1];"), false, __LINE__)
        + test_parse(com_bloc("float b[1^2];"), false, __LINE__)
        + test_parse(com_bloc("float a <= '2', b <= '2', c <= '2';"), true, __LINE__)
        + test_parse(com_bloc("float a <= '2';"), true, __LINE__)
        + test_parse(com_bloc("float a <= 2 + 3;"), false, __LINE__)

        //chars
        + test_parse(com_bloc("char c;"), true, __LINE__)
        + test_parse(com_bloc("char c, b, c;"), true, __LINE__)
        + test_parse(com_bloc("char c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("char c, b, c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("char c <= 2, b <= 2, c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("char c[];"), false, __LINE__)
        + test_parse(com_bloc("char c[1];"), false, __LINE__)
        + test_parse(com_bloc("char c[1^2];"), false, __LINE__)
        + test_parse(com_bloc("char a <= '2', b <= '2', c <= '2';"), true, __LINE__)
        + test_parse(com_bloc("char a <= '2';"), true, __LINE__)
        + test_parse(com_bloc("char a <= 2 + 3;"), false, __LINE__)

        //bools
        + test_parse(com_bloc("bool d;"), true, __LINE__)
        + test_parse(com_bloc("bool d, b, c;"), true, __LINE__)
        + test_parse(com_bloc("bool d <= 2;"), true, __LINE__)
        + test_parse(com_bloc("bool d, b, c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("bool d <= 2, b <= 2, c <= 2;"), true, __LINE__)
        + test_parse(com_bloc("bool d[];"), false, __LINE__)
        + test_parse(com_bloc("bool d[1];"), false, __LINE__)
        + test_parse(com_bloc("bool d[1^2];"), false, __LINE__)
        + test_parse(com_bloc("bool a <= '2', b <= '2', c <= '2';"), true, __LINE__)
        + test_parse(com_bloc("bool a <= '2';"), true, __LINE__)
        + test_parse(com_bloc("bool a <= 2 + 3;"), false, __LINE__)

    // ATRIBUICOES

        + test_parse(com_bloc("foo = bar;"), true, __LINE__)
        + test_parse(com_bloc("foo = 'a';"), true, __LINE__)
        + test_parse(com_bloc("foo = 'aaa';"), false, __LINE__)
        + test_parse(com_bloc("foo = bar"), false, __LINE__)
        + test_parse(com_bloc("foo = 123545;"), true, __LINE__)
        + test_parse(com_bloc("foo = 1235.45;"), true, __LINE__)
        + test_parse(com_bloc("foo = 1235.45e12345;"), true, __LINE__)
        + test_parse(com_bloc("foo = !(1234*abcd+47);"), true, __LINE__)
        + test_parse(com_bloc("int vet[1];"), false, __LINE__)
        + test_parse(com_bloc("vet[a^2^-(1234*abcd+47)] = 3;"), true, __LINE__)
        + test_parse(com_bloc("var[var[10^20^30*2^var[20^30/2^40]]] = 5;"), true, __LINE__)

    // CHAMADA DE FUNCAO

        + test_parse(com_bloc("func()"), false, __LINE__)
        + test_parse(com_bloc("func(a)"), false, __LINE__)
        + test_parse(com_bloc("func(foo())"), false, __LINE__)
        + test_parse(com_bloc("func(foo(123))"), false, __LINE__)
        + test_parse(com_bloc("func();"), true, __LINE__)
        + test_parse(com_bloc("func(a);"), true, __LINE__)
        + test_parse(com_bloc("func(foo());"), true, __LINE__)
        + test_parse(com_bloc("func(foo(123));"), true, __LINE__)
        + test_parse(com_bloc("func(1, a, );"), true, __LINE__)
        + test_parse(com_bloc("func(1, a, -(1234*abcd+47));"), true, __LINE__)

    // EXPRESSOES

        + test_parse(com_bloc("a = !!!!foo;"), true, __LINE__)
        + test_parse(com_bloc("a = !!!(!foo && bar);"), true, __LINE__)
        + test_parse(com_bloc("a = -(1234*!abcd+47);"), true, __LINE__)
        + test_parse(com_bloc("a = !(1234*-abcd+47);"), true, __LINE__)
        + test_parse(com_bloc("a = vet[a^2^-(1234*abcd+47)];"), true, __LINE__)
        + test_parse(com_bloc("a = b;"), true, __LINE__)
        + test_parse(com_bloc("a = func();"), true, __LINE__)
        + test_parse(com_bloc("a = (1234*-abcd+47/9) \% (1234*-abcd+47/9) == var*5 != 10\%2;"), true, __LINE__)
        + test_parse(com_bloc("a = (1234*-abcd+47/9) <= (1234*-abcd+47/9) >= var*5 != 10\%2;"), true, __LINE__)
        + test_parse(com_bloc("a = (1234*-abcd+47/9) && (1234*-abcd+47/9) || var*5 != 10\%2;"), true, __LINE__)
        + test_parse(com_bloc("a = (((1234*-abcd+47/9) && (1234*-abcd+47/9)) || ((var*5) != (10\%2)));"), true, __LINE__)

    // CONTROLE DE FLUXO

        //fluxos no global
        + test_parse("if ()", false, __LINE__)
        + test_parse("if () then", false, __LINE__)
        + test_parse("if () then {}", false, __LINE__)
        + test_parse("if () then {};", false, __LINE__)        
        + test_parse("if (a)", false, __LINE__)
        + test_parse("if (a) then", false, __LINE__)
        + test_parse("if (a) then {}", false, __LINE__)
        + test_parse("if (a) then {};", false, __LINE__)
        + test_parse("if (!a)", false, __LINE__)
        + test_parse("if (!a) then", false, __LINE__)
        + test_parse("if (!a) then {}", false, __LINE__)
        + test_parse("if (!a) then {};", false, __LINE__)
        + test_parse("if (a) then {} else", false, __LINE__)
        + test_parse("if (a) then {} else {}", false, __LINE__)
        + test_parse("if (a) then {} else {};", false, __LINE__)
        + test_parse("if (!a) then {} else", false, __LINE__)
        + test_parse("if (!a) then {} else {}", false, __LINE__)
        + test_parse("if (!a) then {} else {};", false, __LINE__)
        + test_parse("while", false, __LINE__)
        + test_parse("while ()", false, __LINE__)
        + test_parse("while () {}", false, __LINE__)
        + test_parse("while () {};", false, __LINE__)
        + test_parse("while (a)", false, __LINE__)
        + test_parse("while (a) {}", false, __LINE__)
        + test_parse("while (a) {};", false, __LINE__)
        + test_parse("while (!a)", false, __LINE__)
        + test_parse("while (!a) {}", false, __LINE__)
        + test_parse("while (!a) {};", false, __LINE__)
        
        //if-then
        + test_parse(com_bloc("if ()"), false, __LINE__)
        + test_parse(com_bloc("if () then"), false, __LINE__)
        + test_parse(com_bloc("if () then {}"), false, __LINE__)
        + test_parse(com_bloc("if () then {};"), false, __LINE__)
        + test_parse(com_bloc("if (a)"), false, __LINE__)
        + test_parse(com_bloc("if (a) then"), false, __LINE__)
        + test_parse(com_bloc("if (a) then {}"), false, __LINE__)
        + test_parse(com_bloc("if (a) then {};"), true, __LINE__)
        + test_parse(com_bloc("if (!a)"), false, __LINE__)
        + test_parse(com_bloc("if (!a) then"), false, __LINE__)
        + test_parse(com_bloc("if (!a) then {}"), false, __LINE__)
        + test_parse(com_bloc("if (!a) then {};"), true, __LINE__)
        
        //if-then-else      
        + test_parse(com_bloc("if (a) then {} else"), false, __LINE__)
        + test_parse(com_bloc("if (a) then {} else {}"), false, __LINE__)
        + test_parse(com_bloc("if (a) then {} else {};"), true, __LINE__)
        + test_parse(com_bloc("if (!a) then {} else"), false, __LINE__)
        + test_parse(com_bloc("if (!a) then {} else {}"), false, __LINE__)
        + test_parse(com_bloc("if (!a) then {} else {};"), true, __LINE__)

        //while
        + test_parse(com_bloc("while"), false, __LINE__)
        + test_parse(com_bloc("while ()"), false, __LINE__)
        + test_parse(com_bloc("while () {}"), false, __LINE__)
        + test_parse(com_bloc("while () {};"), false, __LINE__)
        + test_parse(com_bloc("while (a)"), false, __LINE__)
        + test_parse(com_bloc("while (a) {}"), false, __LINE__)
        + test_parse(com_bloc("while (a) {};"), true, __LINE__)
        + test_parse(com_bloc("while (!a)"), false, __LINE__)
        + test_parse(com_bloc("while (!a) {}"), false, __LINE__)
        + test_parse(com_bloc("while (!a) {};"), true, __LINE__)
    
    // RETURN

        + test_parse(com_bloc("return;"), false, __LINE__)
        + test_parse(com_bloc("return 1;"), true, __LINE__)
        + test_parse(com_bloc("return a;"), true, __LINE__)
        + test_parse(com_bloc("return -(1234*abcd+47);"), true, __LINE__)

    ;
	
    if (test_result != test_number)
        printf("\n\t\t\033[1;4;31mSOME TESTS FAILED!\n\n\033[0;31mFailed Tests:\033[0m %s\n\n", failed_tests);
    else
        printf("\n\t\t\033[1;4;32mALL TESTS PASSED!\033[0m\n\n");

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

char* com_bloc (char* input)
{
    char output[200] = "int commBlock() {\n    ";

    strcat(output, (const char*)input);
    strcat(output,"\n}");

    strcpy(com_bloc_output,output);

    return com_bloc_output;
}

int test_parse (char* string, int assertion, int line_number) {
    YY_BUFFER_STATE buffer = yy_scan_string(string);
    test_number++;
    printf("\n\n\033[1;3;30mln%d\033[0m\t\t\033[1;4;34mTEST NUMBER: %i\033[0m", line_number, test_number);
    printf("\n\033[1;30m-------------------input start-------------------\033[0m\n%s\n\033[1;30m--------------------input end--------------------\033[0m\n\033[4mResult\033[0m: ", string);    
    int ret = yyparse();
    char converted_int[sizeof(char)*sizeof(test_number)];
    sprintf(converted_int,"%d",test_number);
    yy_delete_buffer(buffer);
    yylex_destroy();
    
    if (ret==1) { // Failure
        if(assertion) { // Success expected
            printf("\033[0;31mFAILED!\033[0m\n");
            strcat(converted_int," ");
            strcat(failed_tests,(const char*)converted_int);
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
            strcat(converted_int," ");
            strcat(failed_tests,(const char*)converted_int);
            return false;
        }
    }
}
