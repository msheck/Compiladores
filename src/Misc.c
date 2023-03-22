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
    if (scope != 0) {
        shift = 0 - shift;        
    }
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