/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "Misc.h"

int intDigitLength(int source){
    int num_digs = 0;
    while(source != 0){
        source = source/10;
        num_digs ++;
    }
    return num_digs;
}

char* int_to_string(int source){
    char buffer[intDigitLength(source)+2];
    sprintf(buffer, "%d", source);
    return strdup(buffer);
}

char* get_dataRegister(int scope){
    char* buffer;
    if (scope == 0) {
        buffer = strdup("rbss");
    }
    else {
        buffer = strdup("rfp");
    }
    return buffer;
}

char* get_memShift(int scope, int shift){
    // if (scope != 0) {
    //     shift = 0 - shift;        
    // }
    return int_to_string(shift);    
}

void* getAddr_dataRegister(int scope){
    char* buffer = get_dataRegister(scope);
    buffer = realloc(buffer, sizeof(char)*5);
    void* address = buffer;
    return address;
}

void* getAddr_memShift(int scope, int shift){
    char* buffer = get_memShift(scope, shift);
    buffer = realloc(buffer, sizeof(char)*intDigitLength(shift)+2);
    void* address = buffer;
    return address;
}

OpList* generate_args(ContentList* args, ASTree* args_tree) {
    OpList* ret = opList_new();
    ASTree* current_node = args_tree;
    ContentList* current_content = args;
    int shift = 2;
    while(current_content!=NULL) {
        ret = opList_pushRight(ret, op_new(OP_STOREAI, current_node->temp, NULL, "rfp", int_to_string(shift)));
        shift += current_content->value->total_size;
        current_content = current_content->next;
        if(current_node->number_of_children > 0)
            current_node = current_node->children[0];
    }
    ret = opList_pushRight(ret, op_new(OP_I2I, "rsp", NULL, "rfp", NULL));
    ret = opList_pushRight(ret, op_new(OP_ADDI, "rsp", int_to_string(shift), "rsp", NULL));
    return ret;
}
