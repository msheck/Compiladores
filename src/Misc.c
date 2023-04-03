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

OpList* generate_args(Content* content, ASTree* args_tree) {
    OpList* ret = opList_new();
    ContentList* args = content->args;
    ASTree* current_node = args_tree;
    ContentList* current_content = args;
    int shift = 2+content->total_size;
    char* buffer_shift;
    buffer_shift = int_to_string(shift);
    ret = opList_pushRight(ret, op_new(OP_ADDI, "rsp", buffer_shift, "rsp", NULL));
    while(current_content!=NULL) {
        buffer_shift = int_to_string(shift);
        ret = opList_pushRight(ret, op_new(OP_STOREAI, current_node->temp, NULL, "rfp", buffer_shift));
        shift += current_content->value->total_size;
        current_content = current_content->next;
        if(current_node->number_of_children > 0)
            current_node = current_node->children[0];
    }
    free(buffer_shift);
    return ret;
}

OpList* allocate_vars(OpList* code, SymbolTable* table) {
    if(table->content == NULL)
        return code;

    int total_size = 0;
    char* buffer;
    for(int i=0; i < table->size; i++) {
        if(table->content[i] != NULL) {
            if(table->content[i]->nature == NAT_VAR) {
                total_size += table->content[i]->total_size;
            }
        }
    }
    buffer = int_to_string(total_size);
    code = opList_pushLeft(code, op_new(OP_ADDI, "rsp", buffer, "rsp", NULL));
    free(buffer);
    return code;
}
