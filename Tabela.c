#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Tabela.h"

// ---------------------------- MAIN ----------------------------

int main(){

    IntList *intList = intList_new();
    printf("\nPUSHING");
    intList = intList_pushLeft(intList, 1);
    intList = intList_pushLeft(intList, 2);
    intList = intList_pushLeft(intList, 3);
    intList = intList_pushLeft(intList, 4);
    intList = intList_pushLeft(intList, 69);
    
    printf("\nPRINTING");
    intList_print(intList);

    printf("\nFREEING");
    intList_free(intList);


    //SymbolTable *table = table_new();
    lexValue lex_val;
    lex_val.line_number = 3;
    lex_val.type = TYPE_LIT;
    lex_val.value = strdup("1");
    
    lexValue lex_val2;
    lex_val2.line_number = 3;
    lex_val2.type = TYPE_LIT;
    lex_val2.value = strdup("2");
    printf("\nLEXVALS CREATED");
    
    lexValue lex_val3;
    lex_val3.line_number = 3;
    lex_val3.type = TYPE_LIT;
    lex_val3.value = strdup("3");
    printf("\nLEXVALS CREATED");

    int **dimensions = malloc(sizeof(int*) * 3);
    int dim1 = 1;
    int dim2 = 2;
    int dim3 = 3;
    dimensions[0] = &dim1;
    dimensions[1] = &dim2;
    dimensions[2] = &dim3;
    printf("\nDIMS CREATED");

    Content *arg1 = content_new(lex_val, NAT_LIT, LIT_TYPE_INT, NULL, 0, NULL, 0);
    Content *arg2 = content_new(lex_val2, NAT_LIT, LIT_TYPE_INT, NULL, 0, NULL, 0);
    Content** args = malloc(sizeof(Content*)*2);
    args[0] = arg1;
    args[1] = arg2;
    printf("\nARGS CREATED");
    printf("\nARGS CREATED");
    printf("\nARGS CREATED");
    printf("\nARGS CREATED");

    //Content *content = content_new(lex_val, NAT_LIT, LIT_TYPE_INT, NULL, NULL);
    Content *content = content_new(lex_val, NAT_LIT, LIT_TYPE_INT, dimensions, 3, args, 2);
    printf("\nCONTENT CREATED");
    printf("\nCONTENT CREATED");
    printf("\nCONTENT CREATED");
    printf("\nCONTENT CREATED");



    Content *content1 = content_new(lex_val, NAT_LIT, LIT_TYPE_INT, dimensions, 3, args, 2);
    Content *content2 = content_new(lex_val2, NAT_LIT, LIT_TYPE_INT, dimensions, 3, args, 2);
    Content *content3 = content_new(lex_val3, NAT_LIT, LIT_TYPE_INT, dimensions, 3, args, 2);
    ContentList *cList = contentList_new();
    printf("\nPUSHING");
    cList = contentList_pushLeft(cList, content1);
    cList = contentList_pushLeft(cList, content2);
    cList = contentList_pushLeft(cList, content3);
    
    printf("\nPRINTING");
    contentList_print(cList);

    printf("\nFREEING");
    contentList_free(cList);

    //table_add_entry(table, "key0", content);
    printf("\nENTRY ADDED");
    printf("\nENTRY ADDED");
    printf("\nENTRY ADDED");
    printf("\nENTRY ADDED");

    //printf("\nkey: %s", table->keys[table_get_index(table, "key0")]);
    //printf("\nvalue: %s\n", table->content[table_get_index(table, "key0")]->lex_value.value);

    //table_free(table);
    //printf("\nFREE DONE!!!\n");
    return 0;
}

//---------------------------- TABLE ----------------------------

SymbolTable *table_new()
{
    SymbolTable* ret = NULL;
    ret = calloc(1, sizeof(SymbolTable));
    if (ret != NULL) {
        ret->size = 0;
        ret->keys = NULL;
        ret->content = NULL;
    }
    return ret;
}

int table_get_hash(char* key) {
    int sum = 0;
    char current = key[0];
    int i = 0;
    while(current != '\0'){
        current = key[i];
        sum += (int)current;
        i++;
    }
    return sum % 1000;
}

void table_add_entry(SymbolTable *table, char* key, Content* content){
    int hash = table_get_hash(key);
    // Se hash >= table->size, aumenta a tabela até hash+1 populando com null. Coloca na última pos.
    if(hash >= table->size) {
        int start = table->size;
	    table->size = hash+1;
        table->keys = realloc(table->keys, table->size * sizeof(char**));
        table->content = realloc(table->content, table->size * sizeof(Content**));
        for(int i=start; i++; i<table->size) {
            table->keys[i] = NULL;
            table->content[i] = NULL;
        }
        table->keys[table->size-1] = key;
        table->content[table->size-1] = content;
    }
    // Se hash < table->size, encontra a primeira pos vazia e coloca.
    // Caso esteja cheia, table->size+=1. Coloca na última pos.
    else {
        int i;
        int is_on_table = false;
        for(i=hash; i<table->size; i++) {
            if(table->keys[i]==NULL) {
                table->keys[i] = key;
                table->content[i] = content;
                is_on_table = true;
                break;
            }
        }
        if(i==table->size-1 && !is_on_table) {
            table->size += 1;
            table->keys = realloc(table->keys, table->size * sizeof(char**));
            table->content = realloc(table->content, table->size * sizeof(Content**));
            table->keys[table->size-1] = key;
            table->content[table->size-1] = content;
        }
    }
}

int table_get_index(SymbolTable* table, char* key){
    int i = table_get_hash(key);
    while(i<table->size) {
        if (table->keys[i]==key){
            break;
        }
        i++;
    }
	return i;
}

void table_free(SymbolTable* table){
    for(int i=0; i<table->size; i++){
        if (table->content[i] != NULL){
            content_free(table->content[i]);
        }
    }
    free(table->keys);
    free(table->content);
    free(table);
}

//---------------------------- CONTENT ----------------------------

Content* content_new(lexValue lex_val, int nat, int lit_type, int** dimensions, int dims_size, Content** args, int args_size){
    Content* content = NULL;
    content = calloc(1, sizeof(Content));
    content->lex_value = lex_val;
    content->nature = nat;
    content->lit_type = lit_type;
    content->dimensions = dimensions;
    content->dimensions_size = dims_size;
    content->args = args;
    content->args_size = args_size;
    content->total_size = calculate_total_size(lit_type, dimensions, dims_size);
    return content;
}

void content_free(Content *content){
    if (content->dimensions != NULL){
        free(content->dimensions);
    }
    printf("\nDIMENSIONS FREED");
    if (content->args != NULL){
        Content *arg = content->args[0];
        for(int i=0; arg != NULL; i++){
            content_free(arg);
            arg = content->args[i];
        }
    }
    free(content->lex_value.value);
    free(content);
}

//---------------------------- INT LIST ----------------------------

IntList* intList_new() {
    return NULL;
}

IntList*  intList_pushLeft(IntList* list, int value) {
    IntList* new_node;
    new_node = malloc(sizeof(IntList*));
    if(list==NULL)
        new_node->next = NULL;
    else
        new_node->next = list;
    new_node->value = value;
    return new_node;
}

void intList_free(IntList* list){
    if(list->next != NULL)
        intList_free(list->next);
    free(list);
}

void _intList_print(IntList* list){
    printf("%d", list->value);
    if(list->next != NULL){
        printf(", ");
        _intList_print(list->next);
    }
    else{
        printf(".");
    }
}

void intList_print(IntList* list){
    printf("\n");
    _intList_print(list);
    printf("\n");
}

//---------------------------- CONTENT LIST ----------------------------

ContentList* contentList_new() {
    return NULL;
}

ContentList*  contentList_pushLeft(ContentList* list, Content* value) {
    ContentList* new_node;
    new_node = malloc(sizeof(ContentList*));
    if(list==NULL)
        new_node->next = NULL;
    else
        new_node->next = list;
    new_node->value = value;
    return new_node;
}

void contentList_free(ContentList* list){
    if(list->next != NULL)
        contentList_free(list->next);
    content_free(list->value);
    free(list);
}

void _contentList_print(ContentList* list){
    printf("%s", list->value->lex_value.value);
    if(list->next != NULL){
        printf(", ");
        _contentList_print(list->next);
    }
    else{
        printf(".");
    }
}

void contentList_print(ContentList* list){
    printf("\n");
    _contentList_print(list);
    printf("\n");
}

//---------------------------- MISC ----------------------------

int calculate_total_size(int lit_type, int** dimensions, int dims_size) {
    int total_size = 0;
    
    switch (lit_type){
        case LIT_TYPE_INT:
            total_size = LIT_SIZE_INT * dims_size;
            break;

        case LIT_TYPE_FLOAT:
            total_size = LIT_SIZE_FLOAT * dims_size;
            break;

        case LIT_TYPE_CHAR:
            total_size = LIT_SIZE_CHAR * dims_size;
            break;

        case LIT_TYPE_BOOL:
            total_size = LIT_SIZE_BOOL * dims_size;
            break;
    }

    return total_size;
}