/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#define TYPE_SPEC_CHAR 0
#define TYPE_KEY_WORD 1
#define TYPE_COMP_OP 2
#define TYPE_IDENT 3
#define TYPE_LIT 4

#define NODE_TYPE_UNDECLARED  0
#define NODE_TYPE_BOOL  1
#define NODE_TYPE_CHAR  2
#define NODE_TYPE_INT   3
#define NODE_TYPE_FLOAT 4

#define NAT_LIT 1
#define NAT_VAR 2
#define NAT_ARR 3
#define NAT_FUN 4

#define LIT_SIZE_BOOL  1
#define LIT_SIZE_CHAR  1
#define LIT_SIZE_INT   4
#define LIT_SIZE_FLOAT 8

#define false 0
#define true  1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct lexical_value lexValue;
typedef struct AbstractSyntaxTree ASTree;
typedef struct int_list IntList;
typedef struct content_list ContentList;
typedef struct content_ Content;
typedef struct symbol_table SymbolTable;
typedef struct iloc_operation Operation;
typedef struct operation_list OpList;

struct lexical_value{
    int line_number;
    int type;
    char* label;
    char* value;
};

struct AbstractSyntaxTree {
    lexValue data;
    int number_of_children;
    struct AbstractSyntaxTree **children;
    int node_type;
    char* temp;
    OpList* code;
};

struct int_list{
    int value;
    struct int_list* next;
};

struct content_list{
    Content* value;
    struct content_list* next;
};

struct content_{
    lexValue lex_data;
    int nature;
    int node_type;
    IntList *dimensions;
    int total_size;
    ContentList *args;
    SymbolTable *table;
    int mem_shift;
    int scope;
};

struct symbol_table{
    int scope_level; // 0 = global, >0 = local
    int size;
    char** keys;
    Content** content;
    SymbolTable *next;
    SymbolTable *parent;
    IntList* typeless;
    int return_type;
};

struct iloc_operation {
    int operation;      // Operation code, defined on CodeGen.h.
    char* arg0;        // The first argument for the operation. Can be a register, constant, label.
    char* arg1;        // The second argument for the operation. Can be a register, constant, label.
    char* dest;         // The destination for the result. Can be a register or memory(register+shift).
    char* dest_shift;   // In case destination is in the format memory(register+shift), this is the shift.
};

struct operation_list{
    Operation* value;
    OpList* next;
};

#endif