/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _CONTENTLIST_H_
#define _CONTENTLIST_H_

#include "Content.h"

ContentList* contentList_new();

ContentList*  contentList_pushLeft(ContentList* list, Content* value);

ContentList*  contentList_pushRight(ContentList* list, Content* value);

void contentList_free(ContentList* list);

void contentList_print(ContentList* list);

#endif