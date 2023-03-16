/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "CodeGen.h"
#include "Errors.h"
#include <stdio.h>
#include <stdlib.h>

// Returns a char* containing an unique label. NEEDS FREE!!!
char* get_label() {
    static int label = 0;
    int num = label;
    int num_digs = 0;
    while(num != 0){
        num = num/10;
        num_digs ++;
    }
    char* ret = malloc(sizeof(char*)*(num_digs+1));
    sprintf(ret, "L%d", label++);
    return ret;
}

// Returns a char* containing an unique name for a temporary. NEEDS FREE!!!
char* get_temp() {
    static int temp = 0;
    int num = temp;
    int num_digs = 0;
    while(num != 0){
        num = num/10;
        num_digs ++;
    }
    char* ret = malloc(sizeof(char*)*(num_digs+2));
    sprintf(ret, "r%d", temp++);
    return ret;
}

Operation* op_new(int operation, char* arg0, char* arg1, char* dest, char* dest_shift) {
    Operation* op_new = malloc(sizeof(Operation*));
    op_new->operation = operation;
    op_new->arg0 = strdup(arg0);
    op_new->arg1 = strdup(arg1);
    if (strcmp(dest, "0") == 0) {
        op_new->dest = strdup("rbss");
        op_new->dest_shift = strdup(dest_shift);
    }
    else {
        op_new->dest = strdup("rfp");
        op_new->dest_shift = strdup("-");
        op_new->dest_shift = strcat(op_new->dest_shift, dest_shift);
    }
    return op_new;
}

char* generate_code(Operation* op) {
    char* buffer = NULL;
    switch (op->operation) {
        case OP_NOP:
            return (strdup("nop\n"));
        case OP_ADD:
            buffer = strdup("add\t");
            break;
        case OP_SUB:
            buffer = strdup("sub\t");
            break;
        case OP_MULT:
            buffer = strdup("mult\t");
            break;
        case OP_DIV:
            buffer = strdup("div\t");
            break;
        case OP_ADDI:
            buffer = strdup("addI\t");
            break;
        case OP_SUBI:
            buffer = strdup("subI\t");
            break;
        case OP_RSUBI:
            buffer = strdup("rsubI\t");
            break;
        case OP_MULTI:
            buffer = strdup("multI\t");
            break;
        case OP_DIVI:
            buffer = strdup("divI\t");
            break;
        case OP_RDIVI:
            buffer = strdup("rdivI\t");
            break;
        case OP_LSHIFT:
            buffer = strdup("lshift\t");
            break;
        case OP_LSHIFTI:
            buffer = strdup("lshiftI\t");
            break;
        case OP_RSHIFT:
            buffer = strdup("rshift\t");
            break;
        case OP_RSHIFTI:
            buffer = strdup("rshiftI\t");
            break;
        case OP_AND:
            buffer = strdup("and\t");
            break;
        case OP_ANDI:
            buffer = strdup("andI\t");
            break;
        case OP_OR:
            buffer = strdup("or\t");
            break;
        case OP_ORI:
            buffer = strdup("orI\t");
            break;
        case OP_XOR:
            buffer = strdup("xor\t");
            break;
        case OP_XORI:
            buffer = strdup("xorI\t");
            break;
        case OP_LOADI:
            buffer = strdup("loadI\t");
            break;
        case OP_LOAD:
            buffer = strdup("load\t");
            break;
        case OP_LOADAI:
            buffer = strdup("loadAI\t");
            break;
        case OP_LOADAO:
            buffer = strdup("loadAO\t");
            break;
        case OP_CLOAD:
            buffer = strdup("cload\t");
            break;
        case OP_CLOADAI:
            buffer = strdup("cloadAI\t");
            break;
        case OP_CLOADAO:
            buffer = strdup("cloadAO\t");
            break;
        case OP_STORE:
            buffer = strdup("store\t");
            break;
        case OP_STOREAI:
            buffer = strdup("storeAI\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t\t=>\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->dest_shift);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_STOREAO:
            buffer = strdup("storeAO\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t\t=>\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->dest_shift);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_CSTORE:
            buffer = strdup("cstore\t");
            break;
        case OP_CSTOREAI:
            buffer = strdup("cstoreI\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t\t=>\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->dest_shift);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_CSTOREAO:
            buffer = strdup("cstoreAO\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t\t=>\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->dest_shift);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_I2I:
            buffer = strdup("i2i\t");
            break;
        case OP_C2C:
            buffer = strdup("c2c\t");
            break;
        case OP_C2I:
            buffer = strdup("c2i\t");
            break;
        case OP_I2C:
            buffer = strdup("i2c\t");
            break;
        case OP_JUMPI:
            buffer = strdup("jumpI\t");
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_JUMP:
            buffer = strdup("jump\t");
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_CBR:
            buffer = strdup("cbr\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_CMP_LT:
            buffer = strdup("cmp_LT\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_LE:
            buffer = strdup("cmp_LE\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_EQ:
            buffer = strdup("cmp_EQ\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_GE:
            buffer = strdup("cmp_GE\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_GT:
            buffer = strdup("cmp_GT\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_NE:
            buffer = strdup("cmp_NE\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t\t->\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        default:
            buffer = malloc(sizeof(char*)*4); // Free not necessary, will always call exit if the program gets here.
            sprintf(buffer,"%d",op->operation);
            emit_error(ERR_UNKNOWN_OPERATION, 0, NULL, buffer);

        if(op->arg1 == NULL){
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t\t=>\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer);
        }
        else{
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t\t=>\t"));
            buffer = strcat(buffer, op->dest);
            buffer = strcat(buffer, "\n");
            return (buffer);
        }
    }
}
