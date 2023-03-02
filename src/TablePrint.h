/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _TABLEPRINT_H_
#define _TABLEPRINT_H_

#include "Table.h"

void table_print_buffer();

void table_print(SymbolTable* table);

void table_print_contexts(SymbolTable* table);

#endif