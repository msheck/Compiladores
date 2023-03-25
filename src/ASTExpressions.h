/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "AbstractSyntaxTree.h"
#include "Errors.h"
#include "CodeGen.h"
#include "OpList.h"

#ifndef _ASTEXPRESSIONS_H_
#define _ASTEXPRESSIONS_H_

OpList* rename_logic_labels(OpList* code);

ASTree* ast_expr_node(ASTree *expr1, lexValue operator, ASTree *expr2);

#endif