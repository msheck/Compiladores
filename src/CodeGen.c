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
    int num_digs = intDigitLength(label);
    char* ret = malloc(sizeof(char*)*(num_digs+1));
    sprintf(ret, "L%d", label++);
    return ret;
}

// Returns a char* containing an unique name for a temporary. NEEDS FREE!!!
char* get_temp() {
    static int temp = 0;
    int num_digs = intDigitLength(temp);
    char* ret = malloc(sizeof(char*)*(num_digs+2));
    sprintf(ret, "r%d", temp++);
    return ret;
}

Operation* op_new(int operation, char* arg0, char* arg1, char* arg2, char* arg3) {
    Operation* op_new = calloc(1,sizeof(Operation));
    op_new->operation = operation;
    op_new->arg0 = NULL;
    op_new->arg1 = NULL;
    op_new->arg2 = NULL;
    op_new->arg3 = NULL;
    switch (operation) {
        case OP_LABEL:
            op_new->arg0 = strdup(arg0);
            return op_new;
        case OP_NOP:
            return op_new;
        case OP_LOADI:
        case OP_LOAD:
        case OP_CLOAD:
        case OP_STORE:
        case OP_CSTORE:
        case OP_I2I:
        case OP_C2C:
        case OP_C2I:
        case OP_I2C:
            op_new->arg0 = strdup(arg0);
        case OP_JUMPI:
        case OP_JUMP:
            op_new->arg2 = strdup(arg2);
            return op_new;
        case OP_STOREAI:
        case OP_STOREAO:
        case OP_CSTOREAI:
        case OP_CSTOREAO:
        case OP_CBR:
            op_new->arg0 = strdup(arg0);
            op_new->arg2 = strdup(arg2);
            op_new->arg3 = strdup(arg3);
            return op_new;
        default:
            op_new->arg0 = strdup(arg0);
            op_new->arg1 = strdup(arg1);
            op_new->arg2 = strdup(arg2);
            return op_new;
    }
}

char* generate_code(Operation* op) {
    char* buffer = NULL;
    switch (op->operation) {
        case OP_LABEL:
            buffer = strdup(op->arg0);
            buffer = strcat(buffer, ":");
            return buffer;
        case OP_NOP:
            return (strdup("\tnop\n"));
        case OP_ADD:
            buffer = strdup("\tadd\t\t");
            break;
        case OP_SUB:
            buffer = strdup("\tsub\t\t");
            break;
        case OP_MULT:
            buffer = strdup("\tmult\t\t");
            break;
        case OP_DIV:
            buffer = strdup("\tdiv\t\t");
            break;
        case OP_ADDI:
            buffer = strdup("\taddI\t\t");
            break;
        case OP_SUBI:
            buffer = strdup("\tsubI\t\t");
            break;
        case OP_RSUBI:
            buffer = strdup("\trsubI\t\t");
            break;
        case OP_MULTI:
            buffer = strdup("\tmultI\t\t");
            break;
        case OP_DIVI:
            buffer = strdup("\tdivI\t\t");
            break;
        case OP_RDIVI:
            buffer = strdup("\trdivI\t\t");
            break;
        case OP_LSHIFT:
            buffer = strdup("\tlshift\t\t");
            break;
        case OP_LSHIFTI:
            buffer = strdup("\tlshiftI\t\t");
            break;
        case OP_RSHIFT:
            buffer = strdup("\trshift\t\t");
            break;
        case OP_RSHIFTI:
            buffer = strdup("\trshiftI\t\t");
            break;
        case OP_AND:
            buffer = strdup("\tand\t\t");
            break;
        case OP_ANDI:
            buffer = strdup("\tandI\t\t");
            break;
        case OP_OR:
            buffer = strdup("\tor\t\t");
            break;
        case OP_ORI:
            buffer = strdup("\torI\t\t");
            break;
        case OP_XOR:
            buffer = strdup("\txor\t\t");
            break;
        case OP_XORI:
            buffer = strdup("\txorI\t\t");
            break;
        case OP_LOADI:
            buffer = strdup("\tloadI\t\t");
            break;
        case OP_LOAD:
            buffer = strdup("\tload\t\t");
            break;
        case OP_LOADAI:
            buffer = strdup("\tloadAI\t\t");
            break;
        case OP_LOADAO:
            buffer = strdup("\tloadAO\t\t");
            break;
        case OP_CLOAD:
            buffer = strdup("\tcload\t\t");
            break;
        case OP_CLOADAI:
            buffer = strdup("\tcloadAI\t\t");
            break;
        case OP_CLOADAO:
            buffer = strdup("\tcloadAO\t\t");
            break;
        case OP_STORE:
            buffer = strdup("\tstore\t\t");
            break;
        case OP_STOREAI:
            buffer = strdup("\tstoreAI\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t=>\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg3);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_STOREAO:
            buffer = strdup("\tstoreAO\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t=>\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg3);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_CSTORE:
            buffer = strdup("\tcstore\t\t");
            break;
        case OP_CSTOREAI:
            buffer = strdup("\tcstoreI\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t=>\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg3);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_CSTOREAO:
            buffer = strdup("\tcstoreAO\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t=>\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg3);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_I2I:
            buffer = strdup("\ti2i\t\t");
            break;
        case OP_C2C:
            buffer = strdup("\tc2c\t\t");
            break;
        case OP_C2I:
            buffer = strdup("\tc2i\t\t");
            break;
        case OP_I2C:
            buffer = strdup("\ti2c\t\t");
            break;
        case OP_JUMPI:
            buffer = strdup("\tjumpI\t\t");
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_JUMP:
            buffer = strdup("\tjump\t\t");
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_CBR:
            buffer = strdup("\tcbr\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg3);
            buffer = strcat(buffer, "\n");
            return (buffer);
        case OP_CMP_LT:
            buffer = strdup("\tcmp_LT\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_LE:
            buffer = strdup("\tcmp_LE\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_EQ:
            buffer = strdup("\tcmp_EQ\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_GE:
            buffer = strdup("\tcmp_GE\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_GT:
            buffer = strdup("\tcmp_GT\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        case OP_CMP_NE:
            buffer = strdup("\tcmp_NE\t\t");
            buffer = strcat(buffer, op->arg0);
            buffer = strcat(buffer, ", ");
            buffer = strcat(buffer, op->arg1);
            buffer = strcat(buffer, strdup("\t->\t"));
            buffer = strcat(buffer, op->arg2);
            buffer = strcat(buffer, "\n");
            return (buffer); 
        default:
            buffer = malloc(sizeof(char*)*4); // Free not necessary, will always call exit if the program gets here.
            sprintf(buffer,"%d",op->operation);
            emit_error(ERR_UNKNOWN_OPERATION, 0, NULL, buffer);
    }
    if(op->arg1 == NULL){
        buffer = strcat(buffer, op->arg0);
        buffer = strcat(buffer, strdup("\t=>\t"));
        buffer = strcat(buffer, op->arg2);
        buffer = strcat(buffer, "\n");
        return (buffer);
    }
    else{
        buffer = strcat(buffer, op->arg0);
        buffer = strcat(buffer, ", ");
        buffer = strcat(buffer, op->arg1);
        buffer = strcat(buffer, strdup("\t=>\t"));
        buffer = strcat(buffer, op->arg2);
        buffer = strcat(buffer, "\n");
        return (buffer);
    }
}
