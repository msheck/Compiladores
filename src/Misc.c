/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "Misc.h"

int intDigitLength(int source){
    int num_digs = 0;
    while(source != 0){
        source = source/10;
        num_digs ++;
    }
    return num_digs;
}

char* int_to_string(int source){
    char buffer[intDigitLength(source)+2];
    sprintf(buffer, "%d", source);
    return strdup(buffer);
}

char* get_dataRegister(int scope){
    char* buffer;
    if (scope == 0) {
        buffer = strdup("rbss");
    }
    else {
        buffer = strdup("rfp");
    }
    return buffer;
}

char* get_memShift(int scope, int shift){
    // if (scope != 0) {
    //     shift = 0 - shift;        
    // }
    return int_to_string(shift);    
}

void* getAddr_dataRegister(int scope){
    char* buffer = get_dataRegister(scope);
    buffer = realloc(buffer, sizeof(char)*5);
    void* address = buffer;
    return address;
}

void* getAddr_memShift(int scope, int shift){
    char* buffer = get_memShift(scope, shift);
    buffer = realloc(buffer, sizeof(char)*intDigitLength(shift)+2);
    void* address = buffer;
    return address;
}

OpList* patch_temps(char* func_name, ASTree* args) {
    /*ContextList* current_context = context_switch_list;
    while(current_context != NULL) {
        if(strcmp(current_context->function_name, func_name) == 0)
            break;
        current_context = current_context->next;
    }
    if (current_context == NULL) {
        printf("\nErro: Contexto nao encontrado.");
        exit(-1);
    }

    ASTree* current_node = args;
    OpList* operations = current_context->operations;
    OpList* current_operation = operations;
    while(current_node->number_of_children > 0) {
        current_node = current_node->children[0];
        current_operation->value->arg0 = strdup(current_node->temp);
        current_operation = current_operation->next;
    }

    return operations;*/
}
