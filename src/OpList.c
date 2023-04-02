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

OpList* opList_goToEnd(OpList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            return opList_goToEnd(list->next);
        else
            return list;
    }
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

OpList* opList_pushRight(OpList* list, Operation* value) {
    OpList* new_node;
    OpList* last_node = opList_goToEnd(list);
    if(list == NULL) {
        list = malloc(sizeof(OpList*)+sizeof(NULL));
        list->value = value;
        list->next = NULL;
        return list;
    }
    new_node = malloc(sizeof(OpList*)+sizeof(NULL));
    new_node->value = value;
    new_node->next = NULL;
    last_node->next = new_node;
    return list;
}

OpList* opList_concatLeft(OpList* list, OpList* source) {
    if(list != NULL){
        if(source->value != NULL){
            OpList* rightmost_source = opList_goToEnd(source);
            rightmost_source->next = list;
        }
        else
            return list;
    }
    return source;
}

OpList* opList_concatRight(OpList* list, OpList* source) {
    if(list != NULL){
        if(source != NULL){
            OpList* rightmost_list = opList_goToEnd(list);
            rightmost_list->next = source;
        }
        return list;
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
    char* buffer = generate_code(list->value);
    printf("%s", buffer);
    free(buffer);
    if(list->next != NULL){
        _opList_print(list->next);
    }
}

void opList_print(OpList* list) {
    printf("\n");
    if(list == NULL) {
        printf("\nLIST EMPTY");
    }
    else{
        _opList_print(list);
        printf("L0:\thalt\n");
    }
    printf("\n");
}