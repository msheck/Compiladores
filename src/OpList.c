/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "OpList.h"

OpList* opList_new() {
    return NULL;
}

OpList*  opList_pushLeft(OpList* list, Operation* value) {
    OpList* new_node;
    if(list == NULL) {
        new_node = malloc(sizeof(OpList*)+sizeof(NULL));
        new_node->next = NULL;
    }
    else {
        new_node = malloc(sizeof(OpList*)+sizeof(list));
        new_node->next = list;
    }
    new_node->value = value;
    return new_node;
}

void opList_free(OpList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            opList_free(list->next);
        // printf("\nFreeing %s: %ld...", list->value->lex_data.label, (long)list );
        op_free(list->value);
        free(list);
        list = NULL;
    }
}

void op_free(Operation* op) {
    for(int i=0; i<op->num_args; i++) {
        if(op->args[i] != NULL)
            free(op->args[i]);
    }
    free(op->args);
    free(op->dest);
    free(op->dest_shift);
    free(op);
}

void _opList_print(OpList* list) {
    if(list == NULL) {
        printf("\nLIST EMPTY");
        return;
    }
    printf("Operation %d", list->value->operation);
    if(list->next != NULL){
        printf(", ");
        _opList_print(list->next);
    }
    else{
        printf(".");
    }
}

void opList_print(OpList* list) {
    printf("\n");
    _opList_print(list);
    printf("\n");
}