#ifndef _TABELA_H_
#define _TABELA_H_

#include "AbstractSyntaxTree.c"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NAT_LIT 1
#define NAT_VAR 2
#define NAT_ARR 3
#define NAT_FUN 4

#define LIT_TYPE_BOOL  1
#define LIT_TYPE_CHAR  2
#define LIT_TYPE_INT   3
#define LIT_TYPE_FLOAT 4

#define LIT_SIZE_BOOL  1
#define LIT_SIZE_CHAR  1
#define LIT_SIZE_INT   4
#define LIT_SIZE_FLOAT 8

#define false 0
#define true  1


typedef struct content_{
    lexValue lex_value;
    int nature;
    int lit_type;
    int total_size;
    int** dimensions; // ALWAYS END WITH NULL!!!
    struct content_ **args; // ALWAYS END WITH NULL!!!
} Content;

typedef struct symbol_table{
    int size; // How many entries in the table
    char** keys;
    Content** content;
} SymbolTable;

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

int calculate_by_type(int size, int** dimensions){
    int num_dimensions = 0;
    int i = 0;
    
    if (dimensions != NULL){        
        while (dimensions[i] != NULL){
            num_dimensions += *dimensions[i];
            i++;
        }
    }
    else{
        num_dimensions = 1;
    }

    return num_dimensions * size;
}

int calculate_total_size(int lit_type, int** dimensions) {
    int total_size = 0;
    
    switch (lit_type){
        case LIT_TYPE_INT:
            total_size = calculate_by_type(LIT_SIZE_INT, dimensions);
            break;

        case LIT_TYPE_FLOAT:
            total_size = calculate_by_type(LIT_SIZE_FLOAT, dimensions);
            break;

        case LIT_TYPE_CHAR:
            total_size = calculate_by_type(LIT_SIZE_CHAR, dimensions);
            break;

        case LIT_TYPE_BOOL:
            total_size = calculate_by_type(LIT_SIZE_BOOL, dimensions);
            break;
    }

    return total_size;
}

Content* content_new(lexValue lex_val, int nat, int lit_type, int** dimensions, Content** args){
    Content* content = NULL;
    content = calloc(1, sizeof(Content));
    content->lex_value = lex_val;
    content->nature = nat;
    content->lit_type = lit_type;
    content->dimensions = dimensions;
    content->args = args;
    content->total_size = calculate_total_size(lit_type, dimensions);
    return content;
}

    


int main(){
    printf("\nSTARTING");
    SymbolTable *table = table_new();
    lexValue lex_val;
    lex_val.line_number = 3;
    lex_val.type = TYPE_LIT;
    lex_val.value = strdup("12345");
    
    lexValue lex_val2;
    lex_val2.line_number = 3;
    lex_val2.type = TYPE_LIT;
    lex_val2.value = strdup("12345");
    printf("\nLEXVALS CREATED");

    int **dimensions = malloc(sizeof(int*) * 3);
    int dim1 = 1;
    int dim2 = 2;
    dimensions[0] = &dim1;
    dimensions[1] = &dim2;
    dimensions[2] = NULL;
    printf("\nDIMS CREATED");

    Content *arg1 = content_new(lex_val, NAT_LIT, LIT_TYPE_INT, NULL, NULL);
    Content *arg2 = content_new(lex_val2, NAT_LIT, LIT_TYPE_INT, NULL, NULL);
    Content** args = malloc(sizeof(Content*)*4);
    args[0] = arg1;
    args[1] = arg2;
    args[2] = NULL;
    printf("\nARGS CREATED");

    //Content *content = content_new(lex_val, NAT_LIT, LIT_TYPE_INT, NULL, NULL);
    Content *content = content_new(lex_val, NAT_LIT, LIT_TYPE_INT, dimensions, args);
    printf("\nCONTENT CREATED");

    table_add_entry(table, "key0", content);
    printf("\nENTRY ADDED");

    printf("\nkey: %s", table->keys[table_get_index(table, "key0")]);
    printf("\nvalue: %s\n", table->content[table_get_index(table, "key0")]->lex_value.value);

    table_free(table);
    printf("\nFREE DONE!!!\n");
    exit(0);
    return 0;
}

#endif
