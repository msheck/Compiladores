/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Adaptado pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
A partir do código fornecido pelo professor.
*/

#ifndef _ARVORE_H_
#define _ARVORE_H_

#include "Table.h"

#define TYPE_SPEC_CHAR 0
#define TYPE_KEY_WORD 1
#define TYPE_COMP_OP 2
#define TYPE_IDENT 3
#define TYPE_LIT 4

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct lexical_value{
  int line_number;
  int type;
  char* value;
} lexValue;

typedef struct AbstractSyntaxTree {
  lexValue value;
  int number_of_children;
  struct AbstractSyntaxTree **children;
  int node_type;
} ASTree;

ASTree *ast_new_node(lexValue value, int node_type);

void ast_free(ASTree *tree);

void ast_add_child(ASTree *tree, ASTree *child);

void ast_print_graphviz(ASTree *tree);

void ast_check_type(ASTree *node1, ASTree *node2);

#endif