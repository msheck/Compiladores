/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "Errors.h"
#include <stdio.h>
#include <stdlib.h>

extern void* tabela;
#include "TablePrint.h"

void emit_error(int code, int line_number, char* label, char* str) {
    switch (code) {
        case ERR_CHAR_TO_INT:
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para INT.\033[0m",
            line_number);
            break;
        case ERR_CHAR_TO_FLOAT:
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para FLOAT.\033[0m",
            line_number);
            break;
        case ERR_CHAR_TO_BOOL:
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para BOOL.\033[0m",
            line_number);
            break;
        case ERR_TYPELESS:
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Nodo %s sem tipo atribuido.\033[0m",
            line_number, label);
            break;
        case ERR_X_TO_CHAR:
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de %s para CHAR.\033[0m",
            line_number, str);
            break;
        case ERR_ARRAY:
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Arranjo %s sendo utilizado como %s.\033[0m",
            line_number, label, str);
            break;
        case ERR_VARIABLE:
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Variavel %s sendo usada como %s.\033[0m",
            line_number, label, str);
            break;
        case ERR_CHAR_VECTOR:
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Arranjo %s nao pode ser do tipo CHAR.\033[0m",
            line_number, label);
            break;
        
        default:
            printf("\n\033[1;4;31mA compilacao terminou com erro desconhecido numero %d.\033[0m",
            code);
            break;
    }
    printf("\nCURRENT TABLE:");
    table_print(tabela);
    exit(code);
}
