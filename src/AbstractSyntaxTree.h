/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Adaptado pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
A partir do código fornecido pelo professor.
*/
#include "Structures.h"
#include "Errors.h"

#ifndef _ARVORE_H_
#define _ARVORE_H_

ASTree* ast_new_node(lexValue value, int node_type);

ASTree* ast_get_node(ASTree *tree);

void ast_free(ASTree *tree);

void ast_add_child(ASTree *tree, ASTree *child);

void ast_print(ASTree *tree);

void ast_check_type(ASTree *node1, ASTree *node2);

void ast_check_not_char(ASTree *node, int expected_type);

void ast_check_node_type(ASTree *node1, int type);

void ast_check_type_node(int type, ASTree *node2);

#endif