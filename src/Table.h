/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/
#include "Structures.h"

#ifndef _TABLE_H_
#define _TABLE_H_

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

char* int_to_type(int i);

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