/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _ERRORS_H_
#define _ERRORS_H_

#define ERR_UNDECLARED 10
#define ERR_DECLARED 11
#define ERR_VARIABLE 20
#define ERR_ARRAY 21
#define ERR_FUNCTION 22
#define ERR_CHAR_TO_INT 31
#define ERR_CHAR_TO_FLOAT 32
#define ERR_CHAR_TO_BOOL 33
#define ERR_CHAR_VECTOR 34
#define ERR_X_TO_CHAR 35
#define ERR_TYPELESS 36
#define ERR_INVALID_EXPR 37
#define ERR_UNKNOWN_OPERATION 38
#define ERR_NO_MAIN 39

void emit_error(int code, int line_number, char* label, char* str);

#endif