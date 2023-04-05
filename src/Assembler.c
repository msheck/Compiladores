/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#define ASM_DATA     0
#define ASM_LABEL    1
#define ASM_DD       1

#include "Assembler.h"

char* get_identifier() {
    static int label_ = 0;
    int num_digs = intDigitLength(label_);
    char* ret = malloc(sizeof(char*)*(num_digs+5));
    sprintf(ret, "var_%d", label_++);
    return ret;
}

OpList* get_global_vars(OpList* iloc_code) {
    OpList* ret = NULL;
    OpList* current = iloc_code;
    ret = opList_pushLeft(ret, op_new(ASM_DATA, NULL, NULL, NULL, NULL));
    // Go through all iloc operations
    while(current != NULL) {
        printf("\n\narg2: %s", current->value->arg2);
        printf("\narg2: %s", current->value->arg2);
        // Find any operation that uses rbss
        if (current->value->arg2 != NULL && strcmp(current->value->arg2, "rbss") == 0) {
            printf("\nFOUND RBSS!");
            printf("\nFOUND RBSS!");
            OpList* current_ret = ret;
            int var_found = false;
            // Try to find the rbss_shift in the variables already found, so we don't add them twice
            while (current_ret != NULL) {
                printf("\n\narg3: %s",current->value->arg3);
                printf("\narg3: %s", current->value->arg3);
                printf("\nret arg2: %s", current_ret->value->arg2);
                printf("\nret arg3: %s", current_ret->value->arg3);
                printf("\nret arg3: %s", current_ret->value->arg3);
                if(current->value->arg3 != NULL && current_ret->value->arg3 != NULL &&
                   strcmp(current->value->arg3, current_ret->value->arg3) == 0) {
                    var_found = true;
                    printf("\nvar found!");
                    printf("\nvar found!");
                    break;
                }
                if (!var_found) {
                    printf("\n!var found!");
                    printf("\n!var found!");
                    ret = opList_pushLeft(ret, op_new(ASM_DD, get_identifier(), NULL, "?", current->value->arg3));
                    printf("\nPASSOU");
                    printf("\nPASSOU");
                }
                current_ret = ret->next;
            }
        }
        current = current->next;
    }
    return ret;
}

// Avengers...
void assemble(OpList* iloc_code) {
    OpList* assembly_code = NULL;
    assembly_code = get_global_vars(iloc_code);
    opList_print(assembly_code);
}

void generateAsm(OpList* iloc_code) {
    assemble(iloc_code);
}
