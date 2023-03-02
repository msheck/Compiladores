/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "Structures.h"

#ifndef _INTLIST_H_
#define _INTLIST_H_

IntList* intList_new();

IntList*  intList_pushLeft(IntList* list, int value);

IntList*  intList_pushRight(IntList* list, int value);

IntList* intList_convert_tree(ASTree* ast);

void intList_free(IntList* list);

void intList_print(IntList* list);

#endif