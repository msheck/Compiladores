/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _MISC_H_
#define _MISC_H_

#include "Structures.h"
#include "OpList.h"

int intDigitLength(int source);

char* int_to_string(int source);

char* get_dataRegister(int scope);

char* get_memShift(int scope, int shift);

void* getAddr_dataRegister(int scope);

void* getAddr_memShift(int scope, int shift);

OpList* generate_args(Content* content, ASTree* args_tree, char* reg);

#endif