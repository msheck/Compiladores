/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "IntList.h"

IntList* intList_new() {
    return NULL;
}

IntList* intList_dup(IntList* original) {
    IntList* duplicate = intList_new();
    while (original != NULL){
        duplicate = intList_pushRight(duplicate, original->value);
        original = original->next;
    }
    return duplicate;
}

IntList* intList_goToEnd(IntList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            return intList_goToEnd(list->next);
        else
            return list;
    }
}

IntList* intList_pushLeft(IntList* list, int value) {
    IntList* new_node;
    if(list == NULL) {
        new_node = malloc(sizeof(IntList*)+sizeof(NULL));
        new_node->next = NULL;
    }
    else {
        new_node = malloc(sizeof(IntList*)+sizeof(list));
        new_node->next = list;
    }
    new_node->value = value;
    //printf("\n%d: Pushing %ld before %ld", new_node->value, (long)new_node, (long)new_node->next);
    return new_node;
}

IntList* intList_pushRight(IntList* list, int value) {
    IntList* new_node;
    IntList* last_node = intList_goToEnd(list);
    if(list == NULL) {
        list = malloc(sizeof(IntList*)+sizeof(NULL));
        list->value = value;
        list->next = NULL;
        //printf("\n%d: Pushing %ld after %ld", list->value, (long)list, (long)last_node);
        return list;
    }
    else {
        new_node = malloc(sizeof(IntList*)+sizeof(list));
        last_node->next = new_node;
        new_node->next = NULL;
    }
    new_node->value = value;
    //printf("\n%d: Pushing %ld after %ld", new_node->value, (long)new_node, (long)last_node);
    return list;
}

IntList* intList_convert_tree(ASTree* ast) {
    ASTree* current = ast;
    IntList* ret = intList_new();
    while(strcmp(current->data.label, "^")==0){
        ret = intList_pushLeft(ret, atoi(current->children[current->number_of_children-1]->data.label));
        current = current->children[0];
    }
    ret = intList_pushLeft(ret, atoi(current->data.label));
    return ret;
}

void intList_free(IntList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            intList_free(list->next);
        //printf("\nFreeing %d: %ld...", list->value, (long)list );
        free(list);
    }    
}

void _intList_print(IntList* list) {
    printf("%d", list->value);
    if(list->next != NULL) {
        printf(", ");
        _intList_print(list->next);
    }
    else {
        printf(".");
    }
}

void intList_print(IntList* list) {
    if(list == NULL) {
        printf("\nLIST EMPTY");
        return;
    }
    printf("\n");
    _intList_print(list);
    printf("\n");
}