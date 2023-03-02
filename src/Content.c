/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "Content.h"

Content* content_new(lexValue lex_val, int nat, int node_type, char* data_value, IntList *dimensions, ContentList *args) {
    Content* content = NULL;
    content = calloc(1, sizeof(Content));
    content->lex_data = lex_val;
    if(lex_val.label != NULL)
        content->lex_data.label = strdup(lex_val.label);
    else
        content->lex_data.label = NULL;
    content->nature = nat;
    content->node_type = node_type;
    if(data_value != NULL)
        content->lex_data.value = strdup(data_value);
    else if(lex_val.value != NULL)
        content->lex_data.value = strdup(lex_val.value);
    content->dimensions = dimensions;
    content->total_size = calculate_total_size(node_type, dimensions);
    content->args = args;
    return content;
}

void content_free(Content *content) {
    if (content->dimensions != NULL) {
        intList_free(content->dimensions);
        // printf("\n\tDimensions freed.");
    }
    if (content->args != NULL) {
        contentList_free(content->args);
        // printf("\n\tArguments freed.");
    }
    free(content->lex_data.label);
    free(content->lex_data.value);
    free(content);
}

int calculate_total_size(int node_type, IntList* dimensions) {
    int total_size = 0;

    IntList *next = dimensions;
    int dims_size = 0;
    while(next != NULL) {
        dims_size ++;
        next = next->next;
    }
    
    switch (node_type) {
        case NODE_TYPE_INT:
            total_size = LIT_SIZE_INT * dims_size;
            break;

        case NODE_TYPE_FLOAT:
            total_size = LIT_SIZE_FLOAT * dims_size;
            break;

        case NODE_TYPE_CHAR:
            total_size = LIT_SIZE_CHAR * dims_size;
            break;

        case NODE_TYPE_BOOL:
            total_size = LIT_SIZE_BOOL * dims_size;
            break;
    }

    return total_size;
}