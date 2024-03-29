/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "ContentList.h"

ContentList* contentList_new() {
    return NULL;
}

ContentList* contentList_dup(ContentList* original) {
    ContentList* duplicate = contentList_new();
    while (original != NULL) {
        duplicate = contentList_pushRight(duplicate, content_dup(original->value));
        original = original->next;
    }
    return duplicate;
}

ContentList* contentList_goToEnd(ContentList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            return contentList_goToEnd(list->next);
        else
            return list;
    }
}

ContentList*  contentList_pushLeft(ContentList* list, Content* value) {
    ContentList* new_node;
    if(list == NULL) {
        new_node = malloc(sizeof(ContentList*)+sizeof(NULL));
        new_node->next = NULL;
    }
    else {
        new_node = malloc(sizeof(ContentList*)+sizeof(list));
        new_node->next = list;
    }
    new_node->value = value;
    return new_node;
}

ContentList* contentList_pushRight(ContentList* list, Content* value) {
    ContentList* new_node;
    ContentList* last_node = contentList_goToEnd(list);
    if(list == NULL) {
        list = malloc(sizeof(ContentList*)+sizeof(NULL));
        list->value = value;
        list->next = NULL;
        return list;
    }
    new_node = malloc(sizeof(ContentList*)+sizeof(NULL));
    new_node->value = value;
    new_node->next = NULL;
    last_node->next = new_node;
    return list;
}

void contentList_free(ContentList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            contentList_free(list->next);
        content_free(list->value);
        free(list);
        list = NULL;
    }
}

void _contentList_print(ContentList* list) {
    if(list == NULL) {
        printf("\nLIST EMPTY");
        return;
    }
    printf("%s", list->value->lex_data.label);
    if(list->next != NULL){
        printf(", ");
        _contentList_print(list->next);
    }
    else{
        printf(".");
    }
}

void contentList_print(ContentList* list) {
    printf("\n");
    _contentList_print(list);
    printf("\n");
}