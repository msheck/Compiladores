/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _OPLIST_H_
#define _OPLIST_H_

#include "Structures.h"

OpList* opList_new();

OpList*  opList_pushLeft(OpList* list, Operation* value);

void op_free(Operation* op);

void opList_free(OpList* list);

void opList_print(OpList* list);

#endif