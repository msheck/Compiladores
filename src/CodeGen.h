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

// NOP
#define NOP         0;
// Aritmetica
#define OP_ADD      1;
#define OP_SUB      2;
#define OP_MULT     3;
#define OP_DIV      4;
#define OP_ADDI     5;
#define OP_SUBI     6;
#define OP_RSUBI    7;
#define OP_MULTI    8;
#define OP_DIVI     9;
#define OP_RDIVI    10;
// Shifts
#define OP_LSHIFT   11;
#define OP_LSHIFTI  12;
#define OP_RSHIFT   13;
#define OP_RSHIFTI  14;
// Load
#define OP_LOAD     15;
#define OP_LOADAI   16;
#define OP_LOADA0   17;
#define OP_CLOAD    18;
#define OP_CLOADAI  19;
#define OP_CLOADA0  20;
// Store
#define OP_STORE    21;
#define OP_STOREAI  22;
#define OP_STOREA0  23;
#define OP_CSTORE   24;
#define OP_CSTOREAI 25;
#define OP_CSTOREA0 26;
// Copia
#define i2i          20;
#define c2c          21;
#define c2i          22;
#define i2c          23;

// Nop
#define NOP         0;
// Aritmetica
#define ADD         1;
#define SUB         2;
#define MULT        3;
#define DIV         4;
#define ADDI        5;
#define SUBI        6;
#define RSUBI       7;
#define MULTI       8;
#define DIVI        9;
#define RDIVI       10;
// Shift
#define LSHIFT      11;
#define LSHIFTI     12;
#define RSHIFT      13;
#define RSHIFTI     14;
// Logica
#define AND         15;
#define ANDI        16;
#define OR          17;
#define ORI         18;
#define XOR         19;
#define XORI        20;
// Memoria
#define LOADI       21;
#define LOAD        22;
#define LOADAI      23;
#define LOADA0      24;
#define CLOAD       25;
#define CLOADAI     26;
#define CLOADA0     27;
#define STORE       28;
#define STOREAI     29;
#define STOREAO     30;
#define CSTORE      31;
#define CSTOREAI    32;
#define CSTOREAO    33;
// Copia
#define I2I         34;
#define C2C         35;
#define C2I         36;
#define I2C         37;
// Fluxo de controle
#define JUMPI       38;
#define JUMP        39;
#define CBR         40;
#define CMP_LT      41;
#define CMP_LE      42;
#define CMP_EQ      43;
#define CMP_GE      44;
#define CMP_GT      45;
#define CMP_NE      46;

char* get_label();
char* get_temp();


#endif