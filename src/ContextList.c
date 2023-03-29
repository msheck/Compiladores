/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "ContextList.h"

ContextList* contextList_new() {
    return NULL;
}

ContextList*  contextList_pushLeft(ContextList* function_context, char* function_name, OpList* context_operations) {
    ContextList* new_node;
    if(function_context == NULL) {
        new_node = malloc(sizeof(ContextList*)+sizeof(NULL));
        new_node->next = NULL;
    }
    else {
        new_node = malloc(sizeof(ContextList*)+sizeof(function_context));
        new_node->next = function_context;
    }
    new_node->function_name = function_name;
    new_node->operations = context_operations;
    return new_node;
}

ContextList* contextList_goToEnd(ContextList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            return contextList_goToEnd(list->next);
        else
            return list;
    }
}

void contextList_free(ContextList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            contextList_free(list->next);
        free(list->function_name);
        opList_free(list->operations);
        free(list);
        list = NULL;
    }
}