/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "TablePrint.h"

extern ContentList *content_buffer;

void table_print_buffer(){
    ContentList* current = content_buffer;
    // printf("\n----BUFFER----");
    int i = 0;
    while(current!=NULL){
        // printf("\nbuffer[%d] \"%s\":\"%s\":\"%s\":\"%s\"", i, current->key, current->value->lex_data.label, current->value->lex_data.value, current->value->data_value);
        current = current->next;
        i++;
    }
    // printf("\n--------------");
}

int count_digits(int n)  
{  
    int counter=0;
    while(n!=0)  
    {  
        n=n/10;  
        counter++;  
    }  
    return counter;  
}  

void print_line(int width, int newline){
    printf("\n+");
    for(int i = 0; i<=2*width; i++){
        printf("-");
    }
    printf("+");
    if(newline)
        printf("\n");        
}

void print_sec_line(int width, int newline){
    printf("\n+");
    for(int i = 0; i<width; i++){
        printf("-");
    }
    printf("+");
    for(int i = 0; i<width; i++){
        printf("-");
    }
    printf("+");
    if(newline)
        printf("\n");
}

void table_print(SymbolTable* table) {
    int width = 40;
    int width_aux;
    //HEADER
    print_line(width,1);
    char* type = int_to_type(table->return_type);
    int i = 0;
    char current = type[0];
    while(current != '\0'){
        i += 1;
        current = type[i];
    }
    printf("|  Printing table of size %04d (%s)%*c", table->size, int_to_type(table->return_type), 2*width-27-i-3, '|');
    print_sec_line(width,0);
    //BODY
    int value_len;
    for(int i=0; i<table->size; i++){
        if(table->keys[i] != NULL){
            char* type = int_to_type(table->content[i]->node_type);
            if(table->content[i]->lex_data.value != NULL)
                value_len = strlen(table->content[i]->lex_data.value);
            else
                value_len = 6;
            printf("\n| %04d [%03d]: (%s) %s = %s %*c", i, table->content[i]->mem_shift, type, table->keys[i], table->content[i]->lex_data.value, (int)(width-(strlen(type)+strlen(table->keys[i])+value_len+19)),'|');
            if(table->content[i]->args == NULL)
                printf(" args is null %*c", width-13,'|');
            else{
                printf(" args: %*c", width-6,'|');
                ContentList *current = table->content[i]->args;
                while(current!=NULL){
                    printf("\n| %*c  %s%*c", width, '|', current->value->lex_data.label, width-(int)strlen(current->value->lex_data.label)-1, '|');
                    current = current->next;
                }
            }
            if(table->content[i]->dimensions != NULL){
                // printf("\n| %*c dims: %*c", width-1, '|', width-8,'|');
                printf("\n| %*c  dims: [", width, '|');
                width_aux = width - 13;
                IntList *current = table->content[i]->dimensions;
                while(current!=NULL){
                    if(width_aux-count_digits(current->value)-3 > 0)
                        width_aux -= count_digits(current->value)+3;
                    else{
                        printf("%*c", width_aux+5, '|');
                        width_aux = width-count_digits(current->value)-8;
                        printf("\n|  %*c ", width-2, '|');
                    }
                    // printf("\n| %*c [%d]%*c", width-1, '|', current->value, width-count_digits(current->value)-4, '|');
                    printf(" %d ", current->value);
                    if(current->next != NULL)
                        printf("^");
                    current = current->next;
                }
                printf("]%*c", width_aux+5, '|');
            }
        }
    }
    print_sec_line(width,1);
}