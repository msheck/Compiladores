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
#include <stdlib.h>
#include <string.h>
#define _(s) #s // https://gcc.gnu.org/onlinedocs/gcc-12.2.0/cpp/Stringizing.html
#include "parser.tab.h" //arquivo gerado com bison -d parser.y

#define bool int
#define true 1
#define false 0
char* path = "E4";

extern int yyparse(void);
extern int yylex_destroy(void);
extern int error_message_enabled;

void *tabela = NULL;
void *escopo = NULL;
void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

/* protótipos deste módulo - as implementações estão após a main */
int test_file(char* relative_file_path, int line_number);
int _test_file(char* command1, char* relative_file_path, char* command2);

char failed_tests[200];
int test_number = 0;

int main (int argc, char **argv)
{
	bool test_result = 0;
	error_message_enabled = 1;

    test_result = test_result

    + test_file("E4/abc00", __LINE__)
    + test_file("E4/abc01", __LINE__)
    + test_file("E4/abc02", __LINE__)
    + test_file("E4/abc03", __LINE__)
    + test_file("E4/abc04", __LINE__)
    + test_file("E4/abc05", __LINE__)
    + test_file("E4/abc06", __LINE__)
    + test_file("E4/abc07", __LINE__)
    + test_file("E4/abc08", __LINE__)
    + test_file("E4/abc09", __LINE__)
    + test_file("E4/abc10", __LINE__)
    + test_file("E4/abc11", __LINE__)
    + test_file("E4/abc13", __LINE__)
    + test_file("E4/abc14", __LINE__)
    + test_file("E4/kal01", __LINE__)
    + test_file("E4/kal02", __LINE__)
    + test_file("E4/kal03", __LINE__)
    + test_file("E4/kal04", __LINE__)
    + test_file("E4/kal06", __LINE__)
    + test_file("E4/kal07", __LINE__)
    + test_file("E4/kal08", __LINE__)
    + test_file("E4/kal09", __LINE__)
    + test_file("E4/kal11", __LINE__)
    + test_file("E4/kal12", __LINE__)
    + test_file("E4/kal13", __LINE__)
    + test_file("E4/kal14", __LINE__)
    + test_file("E4/kal15", __LINE__)
    + test_file("E4/kal16", __LINE__)
    + test_file("E4/kal17", __LINE__)
    + test_file("E4/kal18", __LINE__)
    + test_file("E4/kal19", __LINE__)
    + test_file("E4/kal28", __LINE__)
    + test_file("E4/kal29", __LINE__)
    + test_file("E4/kal30", __LINE__)
    + test_file("E4/kal31", __LINE__)
    + test_file("E4/kal32", __LINE__)
    + test_file("E4/kal33", __LINE__)
    + test_file("E4/kal34", __LINE__)
    + test_file("E4/kal35", __LINE__)
    + test_file("E4/kal36", __LINE__)
    + test_file("E4/kal37", __LINE__)
    + test_file("E4/kal38", __LINE__)
    + test_file("E4/kal39", __LINE__)
    + test_file("E4/kal40", __LINE__)
    + test_file("E4/kal41", __LINE__)
    + test_file("E4/kal42", __LINE__)
    + test_file("E4/kal43", __LINE__)
    + test_file("E4/kal44", __LINE__)
    + test_file("E4/kal45", __LINE__)
    + test_file("E4/kal46", __LINE__)
    + test_file("E4/kal47", __LINE__)
    + test_file("E4/kal48", __LINE__)
    + test_file("E4/kal49", __LINE__)
    + test_file("E4/kal50", __LINE__)
    + test_file("E4/kal51", __LINE__)
    + test_file("E4/kal56", __LINE__)
    + test_file("E4/kal57", __LINE__)
    + test_file("E4/kal58", __LINE__)
    + test_file("E4/kal59", __LINE__)
    + test_file("E4/kal70", __LINE__)
    + test_file("E4/kal71", __LINE__)
    + test_file("E4/kal89", __LINE__)
    + test_file("E4/kal90", __LINE__)
    + test_file("E4/kal91", __LINE__)
    + test_file("E4/kal92", __LINE__)
    + test_file("E4/kal93", __LINE__)
    + test_file("E4/kal94", __LINE__)
    + test_file("E4/kal99", __LINE__)
    + test_file("E4/mao05", __LINE__)
    + test_file("E4/mao06", __LINE__)
    + test_file("E4/mao07", __LINE__)
    + test_file("E4/mao09", __LINE__)
    + test_file("E4/mao10", __LINE__)
    + test_file("E4/mao11", __LINE__)
    + test_file("E4/mao12", __LINE__);
	
    if (test_result != test_number)
        printf("\n\t\t\033[1;4;31mSOME TESTS FAILED!\n\n\033[0;31mFailed Tests:\033[0m %s\n\n", failed_tests);
    else
        printf("\n\t\t\033[1;4;32mALL TESTS PASSED!\033[0m\n\n");

	return 0;
}

int test_file(char* relative_file_path, int line_number) {
    // Open file
    FILE *file_ptr;
    file_ptr = fopen(relative_file_path, "r");
    if(file_ptr == NULL){
        printf("\nErro ao abrir arquivo de teste %s!", relative_file_path);
        exit(-1);
    }
    // Get first line from file
    char f_char[2];
    f_char[0] = 'a';
    f_char[1] = '\0';
    char* line = malloc(sizeof(char*)*2);
    line = strdup("");
    int char_count = 0;
    while(f_char[0] != '\n') {
        f_char[0] = fgetc(file_ptr);
        char_count += 1;
        line = realloc(line, sizeof(char*) * char_count);
        strcat(line, f_char);
    }
    fclose(file_ptr);
    // Check if there is an expected output
    int expected_output = 0;
    if(line[0] == '/' && line[1] == '/') {
        if(strcmp(line, "//ERR_UNDECLARED\n") == 0)
            expected_output = 10;
        else if(strcmp(line, "//ERR_DECLARED\n") == 0)
            expected_output = 11;
        else if(strcmp(line, "//ERR_VARIABLE\n") == 0)
            expected_output = 20;
        else if(strcmp(line, "//ERR_ARRAY\n") == 0)
            expected_output = 21;
        else if(strcmp(line, "//ERR_FUNCTION\n") == 0)
            expected_output = 22;
        else if(strcmp(line, "//ERR_CHAR_TO_INT\n") == 0)
            expected_output = 31;
        else if(strcmp(line, "//ERR_CHAR_TO_FLOAT\n") == 0)
            expected_output = 32;
        else if(strcmp(line, "//ERR_CHAR_TO_BOOL\n") == 0)
            expected_output = 33;
        else if(strcmp(line, "//ERR_CHAR_VECTOR\n") == 0)
            expected_output = 34;
        else if(strcmp(line, "//ERR_X_TO_CHAR\n") == 0)
            expected_output = 35;
        else if(strcmp(line, "//ERR_TYPELESS\n") == 0)
            expected_output = 36;
        else if(strcmp(line, "//ERR_INVALID_EXPR\n") == 0)
            expected_output = 37;
    }
    free(line);
    
    int output = _test_file("./etapa4 < ", relative_file_path, " > /dev/null");
    test_number ++;
    printf("\n\n\033[1;3;30mln%d\033[0m\t\t\033[1;4;34mTEST NUMBER: %i\033[0m", line_number, test_number);
    printf("\n\033[0mTesting file %s\nExpected output:\033[1;30m %d\n\033[0mActual output:\033[1;30m   %d\033[0m", relative_file_path, expected_output, output);
    if(output != expected_output) {
        char converted_int[sizeof(char)*sizeof(test_number)];
        sprintf(converted_int,"%d",test_number);
        strcat(converted_int," ");
        strcat(failed_tests,(const char*)converted_int);
        printf("\n\033[0;31mFAILED!\033[0m\n");
    }
    else
        printf("\n\033[0;32mPASSED!\033[0m\n");

    return output==expected_output;
}

int _test_file(char* command1, char* relative_file_path, char* command2) {
    char* buffer = malloc((strlen(command1)+1+strlen(relative_file_path)+1+strlen(command2)+1) * sizeof(char*));
    strcpy(buffer, command1);
    strcat(buffer, relative_file_path);
    strcat(buffer, command2);
    int ret = system(buffer) >> 8;
    free(buffer);
    return ret;
}
