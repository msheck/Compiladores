/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "Structures.h"
#include "ContentList.h"

#ifndef _TABLE_H_
#define _TABLE_H_

SymbolTable *table_new();

int table_get_hash(char* key);

int table_get_index(SymbolTable* table, char* key);

int table_get_type(SymbolTable* table, char* key, int line_number);

Content* table_get_content(SymbolTable* table, lexValue key);

void table_add_entry(SymbolTable *table, char* key, Content* content);

void table_free(SymbolTable* table);

void table_abort(SymbolTable* root);

void table_check_declared(SymbolTable* table, char* key, int line);

Content* table_check_undeclared(SymbolTable* table, char* key, int line);

void table_check_use(Content* content, int line);

SymbolTable* table_nest(SymbolTable* root);

SymbolTable* table_pop_nest(SymbolTable* root);

void table_update_type(SymbolTable* table, int type);

void table_update_data_value(SymbolTable* table, char* key, ASTree* data_value);

void table_add_to_buffer(Content* content, char* key);

void table_flush_buffer(SymbolTable* table);

ContentList* table_dup_buffer();

char* int_to_type(int i);

#endif