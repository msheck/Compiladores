#ifndef _TABELA_H_
#define _TABELA_H_

#include "AbstractSyntaxTree.h"

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

typedef struct int_list{
    int value;
    struct int_list* next;
} IntList;

typedef struct content_list{
    Content* value;
    struct content_list* next;
} ContentList;

typedef struct content_{
    lexValue lex_value;
    int nature;
    int lit_type;
    int total_size;
    int** dimensions;
    int dimensions_size;
    ContentList *args;
    int args_size;
} Content;

typedef struct symbol_table{
    int size; // How many entries in the table
    char** keys;
    Content** content;
} SymbolTable;

//---------------------------- TABLE ----------------------------

SymbolTable *table_new();

int table_get_hash(char* key);

void table_add_entry(SymbolTable *table, char* key, Content* content);

int table_get_index(SymbolTable* table, char* key);

void table_free(SymbolTable* table);

int table_has_duplicate(SymbolTable* table, char* key);

//---------------------------- CONTENT ----------------------------

Content* content_new(lexValue lex_val, int nat, int lit_type, int** dimensions, int dims_size, Content** args, int args_size);

void content_free(Content *content);

//---------------------------- INT LIST ----------------------------

IntList* intList_new();

IntList*  intList_pushLeft(IntList* list, int value);

IntList*  intList_pushRight(IntList* list, int value);

void intList_free(IntList* list);

void intList_print(IntList* list);

//---------------------------- CONTENT LIST ----------------------------

ContentList* contentList_new();

ContentList*  contentList_pushLeft(ContentList* list, Content* value);

ContentList*  contentList_pushRight(ContentList* list, Content* value);

void contentList_free(ContentList* list);

void contentList_print(ContentList* list);

//---------------------------- MISC ----------------------------

int calculate_total_size(int lit_type, int** dimensions, int dims_size);


#endif