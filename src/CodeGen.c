/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/
#include "CodeGen.h"
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
    sprintf(ret, "%d", label++);
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
