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
char* path = "aux/E4";

extern int yyparse(void);
extern int yylex_destroy(void);
extern int error_message_enabled;

void *tabela = NULL;
void *escopo = NULL;
void *escopo_anterior = NULL;
void *arvore = NULL;
void *operacoes = NULL;
void exporta (void *arvore);
void libera (void *arvore);
int rbss_shift = 0;
int rsp_shift = 0;
int rsp = 0;
int rfp = 0;
char* main_label;
int mainFound;

/* protótipos deste módulo - as implementações estão após a main */
int test_file(char* relative_file_path, int line_number);
int _test_file(char* command1, char* relative_file_path, char* command2);
void _run_iloc(int data, int stack);

char failed_tests[200];
int test_number = 0;

int main (int argc, char **argv)
{
	bool test_result = 0;
	error_message_enabled = 1;

    test_result = test_result

    + test_file("_auxFiles/E5/ijk00", __LINE__)
    + test_file("_auxFiles/E5/ijk01", __LINE__)
    + test_file("_auxFiles/E5/ijk1A", __LINE__)
    + test_file("_auxFiles/E5/ijk1B", __LINE__)
    + test_file("_auxFiles/E5/ijk02", __LINE__)
    + test_file("_auxFiles/E5/ijk03", __LINE__)
    + test_file("_auxFiles/E5/ijk04", __LINE__)
    + test_file("_auxFiles/E5/ijk05", __LINE__)
    + test_file("_auxFiles/E5/ijk06", __LINE__)
    + test_file("_auxFiles/E5/ijk07", __LINE__)
    + test_file("_auxFiles/E5/ijk08", __LINE__)
    + test_file("_auxFiles/E5/ijk09", __LINE__)
    + test_file("_auxFiles/E5/ijk10", __LINE__)
    + test_file("_auxFiles/E5/ijk11", __LINE__)
    + test_file("_auxFiles/E5/ijk12", __LINE__)
    + test_file("_auxFiles/E5/ijk13", __LINE__)
    + test_file("_auxFiles/E5/ijk14", __LINE__)
    + test_file("_auxFiles/E5/ijk15", __LINE__)
    + test_file("_auxFiles/E5/ijk16", __LINE__)
    + test_file("_auxFiles/E5/ijk17", __LINE__)
    + test_file("_auxFiles/E5/ijk18", __LINE__)
    + test_file("_auxFiles/E5/ijk19", __LINE__)
    + test_file("_auxFiles/E5/ijk20", __LINE__)
    + test_file("_auxFiles/E5/ijk22", __LINE__)
    + test_file("_auxFiles/E5/ijk23", __LINE__)
    + test_file("_auxFiles/E5/ijk24", __LINE__)
    + test_file("_auxFiles/E5/ijk25", __LINE__)
    + test_file("_auxFiles/E5/ijk26", __LINE__)
    + test_file("_auxFiles/E5/ijk27", __LINE__)
    + test_file("_auxFiles/E5/ijk28", __LINE__)
    + test_file("_auxFiles/E5/ijk29", __LINE__)
    + test_file("_auxFiles/E5/ijk30", __LINE__);
	
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
    // // Check if there is an expected output
    // int expected_output = 0;
    // if(line[0] == '/' && line[1] == '/') {
    //     if(strcmp(line, "//ERR_UNDECLARED\n") == 0)
    //         expected_output = 10;
    //     else if(strcmp(line, "//ERR_DECLARED\n") == 0)
    //         expected_output = 11;
    //     else if(strcmp(line, "//ERR_VARIABLE\n") == 0)
    //         expected_output = 20;
    //     else if(strcmp(line, "//ERR_ARRAY\n") == 0)
    //         expected_output = 21;
    //     else if(strcmp(line, "//ERR_FUNCTION\n") == 0)
    //         expected_output = 22;
    //     else if(strcmp(line, "//ERR_CHAR_TO_INT\n") == 0)
    //         expected_output = 31;
    //     else if(strcmp(line, "//ERR_CHAR_TO_FLOAT\n") == 0)
    //         expected_output = 32;
    //     else if(strcmp(line, "//ERR_CHAR_TO_BOOL\n") == 0)
    //         expected_output = 33;
    //     else if(strcmp(line, "//ERR_CHAR_VECTOR\n") == 0)
    //         expected_output = 34;
    //     else if(strcmp(line, "//ERR_X_TO_CHAR\n") == 0)
    //         expected_output = 35;
    //     else if(strcmp(line, "//ERR_TYPELESS\n") == 0)
    //         expected_output = 36;
    //     else if(strcmp(line, "//ERR_INVALID_EXPR\n") == 0)
    //         expected_output = 37;
    // }
    // free(line);
    
    int exit_code = _test_file("./etapa5 < ", relative_file_path, " > output.iloc");
    test_number ++;
    printf("\n\n\033[1;3;30mln%d\033[0m\t\t\033[1;4;34mTEST NUMBER: %i\033[0m", line_number, test_number);
    printf("\n\033[0mTesting file %s\nComment:\033[1;30m %s\n\033[0mExit code:\033[1;30m   %d\033[0m", relative_file_path, line, exit_code);
    _run_iloc(500, 1024);
    
    // if(output != expected_output) {
    //     char converted_int[sizeof(char)*sizeof(test_number)];
    //     sprintf(converted_int,"%d",test_number);
    //     strcat(converted_int," ");
    //     strcat(failed_tests,(const char*)converted_int);
    //     printf("\n\033[0;31mFAILED!\033[0m\n");
    // }
    // else
    //     printf("\n\033[0;32mPASSED!\033[0m\n");

    // return output==expected_output;
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

void _run_iloc(int data, int stack){
    int data_len = intDigitLength(data)+1;
    int stack_len = intDigitLength(stack)+1;
    char* buffer_data = malloc(data_len*sizeof(char));
    char* buffer_stack = malloc(stack_len*sizeof(char));
    char* buffer = malloc((data_len+stack_len+60) * sizeof(char*));
    strcpy(buffer, "./ilocsim.py -x -m --data ");
    strcat(buffer, buffer_data);
    strcat(buffer, "--stack ");
    strcat(buffer, buffer_stack);
    strcat(buffer, " output.iloc >iloc_result");
    int ret = system(buffer) >> 8;
    free(buffer);
}
