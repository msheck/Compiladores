/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _CONTENT_H_
#define _CONTENT_H_

#include "IntList.h"
#include "ContentList.h"

Content* content_new(lexValue lex_val, int nat, int node_type, char* data_value, IntList *dimensions, ContentList *args);

void content_free(Content *content);

int calculate_total_size(int node_type, IntList* dimensions);

#endif