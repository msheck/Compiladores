/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _CODDEGEN_H_
#define _CODDEGEN_H_

// Label
#define OP_LABEL       -1
// Nop
#define OP_NOP         0
// Aritmetica
#define OP_ADD         1
#define OP_SUB         2
#define OP_MULT        3
#define OP_DIV         4
#define OP_ADDI        5
#define OP_SUBI        6
#define OP_RSUBI       7
#define OP_MULTI       8
#define OP_DIVI        9
#define OP_RDIVI       10
// Shift
#define OP_LSHIFT      11
#define OP_LSHIFTI     12
#define OP_RSHIFT      13
#define OP_RSHIFTI     14
// Logica
#define OP_AND         15
#define OP_ANDI        16
#define OP_OR          17
#define OP_ORI         18
#define OP_XOR         19
#define OP_XORI        20
// Memoria
#define OP_LOADI       21
#define OP_LOAD        22
#define OP_LOADAI      23
#define OP_LOADAO      24
#define OP_CLOAD       25
#define OP_CLOADAI     26
#define OP_CLOADAO     27
#define OP_STORE       28
#define OP_STOREAI     29
#define OP_STOREAO     30
#define OP_CSTORE      31
#define OP_CSTOREAI    32
#define OP_CSTOREAO    33
// Copia
#define OP_I2I         34
#define OP_C2C         35
#define OP_C2I         36
#define OP_I2C         37
// Fluxo de controle
#define OP_JUMPI       38
#define OP_JUMP        39
#define OP_CBR         40
#define OP_CMP_LT      41
#define OP_CMP_LE      42
#define OP_CMP_EQ      43
#define OP_CMP_GE      44
#define OP_CMP_GT      45
#define OP_CMP_NE      46

#include "Structures.h"
#include "Misc.h"

char* get_label();

char* get_temp();

Operation* op_new(int operation, char* arg0, char* arg1, char* arg2, char* arg3);

char* generate_code(Operation* op);

OpList function_call(ASTree node, ContentList* args);

#endif