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

OpList* opList_goToEnd(OpList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            return opList_goToEnd(list->next);
        else
            return list;
    }
}

OpList* opList_concatLeft(OpList* list, OpList* source) {
    if(list != NULL){
        OpList* rightmost_source = opList_goToEnd(source);
        rightmost_source->next = list;
    }
    return source;
}

void opList_free(OpList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            opList_free(list->next);
        op_free(list->value);
        free(list);
        list = NULL;
    }
}

void op_free(Operation* op) {
    free(op->arg0);
    free(op->arg1);
    free(op->arg2);
    free(op->arg3);
    free(op);
}

void _opList_print(OpList* list) {
    if(list == NULL) {
        printf("\nLIST EMPTY");
        return;
    }
    printf("%s", generate_code(list->value));
    if(list->next != NULL){
        _opList_print(list->next);
    }
}

void opList_print(OpList* list) {
    printf("\n");
    _opList_print(list);
    printf("\n");
}