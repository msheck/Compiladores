/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _CONTEXTLIST_H_
#define _CONTEXTLIST_H_

#include "Structures.h"
#include "CodeGen.h"
#include "OpList.h"

ContextList* contextList_new();

ContextList*  contextList_pushLeft(ContextList* function_context, OpList* context_operations);

void contextList_free(ContextList* list);

#endif