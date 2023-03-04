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
    //printf("\n%s: Pushing %ld before %ld", new_node->value->lex_data.label, (long)new_node, (long)new_node->next);
    return new_node;
}

ContentList* contentList_pushRight(ContentList* list, Content* value) {
    ContentList* new_node;
    ContentList* last_node = contentList_goToEnd(list);
    if(list == NULL) {
        list = malloc(sizeof(ContentList*)+sizeof(NULL));
        list->value = value;
        list->next = NULL;
        //printf("\n%s: Pushing %ld after %ld", list->value->lex_data.label, (long)list, (long)last_node);
        return list;
    }
    else {
        new_node = malloc(sizeof(ContentList*)+sizeof(list));
        last_node->next = new_node;
        new_node->next = NULL;
    }
    new_node->value = value;
    //printf("\n%s: Pushing %ld after %ld", new_node->value->lex_data.label, (long)new_node, (long)last_node);
    return list;
}

void contentList_free(ContentList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            contentList_free(list->next);
        // printf("\nFreeing %s: %ld...", list->value->lex_data.label, (long)list );
        content_free(list->value);
        free(list->key);
        free(list);
        list = NULL;
    }
}

void _contentList_print(ContentList* list) {
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