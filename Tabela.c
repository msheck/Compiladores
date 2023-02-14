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

#define TYPE_BOOL  1
#define TYPE_CHAR  2
#define TYPE_INT   3
#define TYPE_FLOAT 4

#define SIZE_BOOL  1
#define SIZE_CHAR  1
#define SIZE_INT   4
#define SIZE_FLOAT 8


typedef struct content_{
    lexValue lex_value;
    int nature;
    int type;
    int size;
    //dimensions???
    int* dimensions;
    //TODO: args???
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

void table_add_entry(SymbolTable* table, char* key, Content* content){
	table->size += 1;
	table->keys = realloc(table->keys, table->size * sizeof(char**));
	table->keys[table->size-1] = key;
	table->content = realloc(table->content, table->size * sizeof(Content**));
	table->content[table->size-1] = content;
}

int table_get_index(SymbolTable* table, char* key){ //TODO
	return 0;
}

void table_remove_index(SymbolTable* table, int index){
	free(table->keys[index]);
	free(table->content[index]);
	// TODO: free table->content[index].dimension
	table->size -= 1;
}

void table_remove_entry(SymbolTable* table, char* key){
	table_remove_index(table, table_get_index(table, key));
}

int main(){
    printf("\nSTARTING");
    SymbolTable *table = table_new();
    lexValue lex_val;
    lex_val.line_number = 3;
    lex_val.type = TYPE_LIT;
    lex_val.value = "12345";
    printf("\nLEXVAL CREATED");
    Content content;
    content.lex_value = lex_val;
    content.nature = NAT_LIT;
    content.type = TYPE_INT;
    content.size = SIZE_INT;
    printf("\nCONTENT CREATED");

    table_add_entry(table, "key0", &content);
    printf("\nENTRY ADDED");

    printf("\nkey: %s", table->keys[0]);
    printf("\nvalue: %s\n", table->content[0]->lex_value.value);

    table_remove_entry(table, "");
    return 0;
}

#endif
