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
#include "CodeGen.h"

OpList* opList_new();

OpList*  opList_pushLeft(OpList* list, Operation* value);

OpList* opList_concatLeft(OpList* list, OpList* source);

void op_free(Operation* op);

void opList_free(OpList* list);

void opList_print(OpList* list);

#endif