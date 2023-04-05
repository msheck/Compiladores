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
#define ASM_DD       2

#include "Assembler.h"

char* get_identifier() {
    static int label_ = 1;
    int num_digs = intDigitLength(label_);
    char* ret = malloc(sizeof(char*)*(num_digs+5));
    sprintf(ret, "var_%d", label_++);
    return ret;
}

Operation* asm_op_new(int operation, char* arg0, char* arg1, char* arg2, char* arg3) {
    Operation* op_new = calloc(1,sizeof(Operation));
    op_new->operation = operation;
    op_new->arg0 = NULL;
    op_new->arg1 = NULL;
    op_new->arg2 = NULL;
    op_new->arg3 = NULL;
    if (arg0 != NULL)
        op_new->arg0 = strdup(arg0);
    if (arg1 != NULL)
        op_new->arg1 = strdup(arg1);
    if (arg2 != NULL)
        op_new->arg2 = strdup(arg2);
    if (arg3 != NULL)
        op_new->arg3 = strdup(arg3);
    return op_new;
}

OpList* get_global_vars(OpList* iloc_code) {
    OpList* ret = NULL;
    OpList* current = iloc_code;
    ret = opList_pushLeft(ret, op_new(ASM_DATA, NULL, NULL, NULL, NULL));
    // Go through all iloc operations
    while(current != NULL) {
        // Find any operation that uses rbss
        if (current->value->arg2 != NULL && strcmp(current->value->arg2, "rbss") == 0) {
            OpList* current_ret = ret;
            int var_found = false;
            // Try to find the rbss_shift in the variables already found, so we don't add them twice
            while (current_ret != NULL) {
                if(current->value->arg3 != NULL && current_ret->value->arg3 != NULL &&
                   strcmp(current->value->arg3, current_ret->value->arg3) == 0) {
                    var_found = true;
                    break;
                }
                current_ret = ret->next;
            }
            if (!var_found) {
                ret = opList_pushRight(ret, asm_op_new(ASM_DD, get_identifier(), NULL, "?", current->value->arg3));
            }
        }
        current = current->next;
    }
    return ret;
}

void print_asp_op(Operation* op) {
    switch (op->operation)
    {
    case 0:
        printf("\n.DATA");
        break;
    case 1:
        printf("\n%s:", op->arg0);
        break;
    case 2:
        printf("\n%s\tDD %s", op->arg0, op->arg2);
    
    default:
        break;
    }
}

void opList_asm_print(OpList* operations) {
    OpList* op = operations;
    while(op!=NULL) {
        print_asp_op(op->value);
        op = op->next;
    }
}

// Avengers...
void assemble(OpList* iloc_code) {
    OpList* assembly_code = NULL;
    assembly_code = get_global_vars(iloc_code);
    opList_asm_print(assembly_code);
    opList_free(assembly_code);
}

void generateAsm(OpList* iloc_code) {
    assemble(iloc_code);
}
