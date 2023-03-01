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

#define LIT_SIZE_BOOL  1
#define LIT_SIZE_CHAR  1
#define LIT_SIZE_INT   4
#define LIT_SIZE_FLOAT 8

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
    char* key; // Optional
    struct content_list* next;
};

struct content_{
    lexValue lex_data;
    int nature;
    int node_type;
    //char* data_value;
    IntList *dimensions;
    int total_size;
    ContentList *args;
};

struct symbol_table{
    int size; // How many entries in the table
    char** keys;
    Content** content;
    SymbolTable *next;
    SymbolTable *parent;
    IntList* typeless;
};

//---------------------------- TABLE ----------------------------

SymbolTable *table_new();

int table_get_hash(char* key);

int table_get_index(SymbolTable* table, char* key);

int table_get_type(SymbolTable* table, char* key);

Content* table_get_content(SymbolTable* table, lexValue key);

void table_add_entry(SymbolTable *table, char* key, Content* content);

void table_free(SymbolTable* table);

void table_abort(SymbolTable* root);

void table_check_declared(SymbolTable* table, char* key, int line);

int table_check_undeclared(SymbolTable* table, char* key, int line);

void table_check_use(SymbolTable* table, Content* content, int line);

SymbolTable* table_nest(SymbolTable* root);

void table_pop_nest(SymbolTable* root);

void table_update_type(SymbolTable* table, int type);

void table_update_data_value(SymbolTable* table, char* key, ASTree* data_value);

void table_add_to_buffer(Content* content, char* key);

void table_flush_buffer(SymbolTable* table);

ContentList* table_dup_buffer();

void table_print_buffer();

char* int_to_type(int i);

void table_print(SymbolTable* table);

void table_print_contexts(SymbolTable* table);

//---------------------------- CONTENT ----------------------------

Content* content_new(lexValue lex_val, int nat, int node_type, char* data_value, IntList *dimensions, ContentList *args);

void content_free(Content *content);

//---------------------------- INT LIST ----------------------------

IntList* intList_new();

IntList*  intList_pushLeft(IntList* list, int value);

IntList*  intList_pushRight(IntList* list, int value);

IntList* intList_convert_tree(ASTree* ast);

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