/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _TABLE_H_
#define _TABLE_H_

#include "AbstractSyntaxTree.h"

#define NAT_LIT 1
#define NAT_VAR 2
#define NAT_ARR 3
#define NAT_FUN 4

#define NODE_TYPE_BOOL  1
#define NODE_TYPE_CHAR  2
#define NODE_TYPE_INT   3
#define NODE_TYPE_FLOAT 4

#define LIT_SIZE_BOOL  1
#define LIT_SIZE_CHAR  1
#define LIT_SIZE_INT   4
#define LIT_SIZE_FLOAT 8

#define ERR_UNDECLARED 10
#define ERR_DECLARED 11
#define ERR_VARIABLE 20
#define ERR_ARRAY 21
#define ERR_FUNCTION 22
#define ERR_CHAR_TO_INT 31
#define ERR_CHAR_TO_FLOAT 32
#define ERR_CHAR_TO_BOOL 33
#define ERR_CHAR_VECTOR 34
#define ERR_X_TO_CHAR 35

#define false 0
#define true  1

typedef struct int_list IntList;
typedef struct content_list ContentList;
typedef struct content_ Content;
typedef struct symbol_table SymbolTable;

struct int_list{
    int value;
    struct int_list* next;
};

struct content_list{
    Content* value;
    struct content_list* next;
};

struct content_{
    lexValue lex_value;
    int nature;
    int node_type;
    int total_size;
    IntList *dimensions;
    ContentList *args;
};

struct symbol_table{
    int size; // How many entries in the table
    char** keys;
    Content** content;
    SymbolTable *next;
    SymbolTable *parent;
};

//---------------------------- TABLE ----------------------------

SymbolTable *table_new();

int table_get_hash(char* key);

int table_get_index(SymbolTable* table, char* key);

int table_get_type(SymbolTable* table, char* key);

void table_add_entry(SymbolTable *table, char* key, Content* content);

void table_free(SymbolTable* table);

void table_abort(SymbolTable* root);

void table_check_declared(SymbolTable* table, char* key, int line);

void table_check_undeclared(SymbolTable* table, char* key, int line);

void table_check_use(SymbolTable* table, Content* content, int line);

void table_nest(SymbolTable* root, SymbolTable* new_table);

void table_pop_nest(SymbolTable* root);

//---------------------------- CONTENT ----------------------------

Content* content_new(lexValue lex_val, int nat, int node_type, IntList *dimensions, ContentList *args);

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

int calculate_total_size(int node_type, IntList* dimensions);


#endif