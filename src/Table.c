/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "Table.h"
#include "TablePrint.h"

ContentList *content_buffer = NULL;

SymbolTable *table_new() {
    SymbolTable* ret = NULL;
    ret = calloc(1, sizeof(SymbolTable));
    if(ret != NULL) {
        ret->size = 0;
        ret->keys = NULL;
        ret->content = NULL;
        ret->next = NULL;
        ret->parent = NULL;
        ret->typeless = intList_new();
    }
    return ret;
}

int table_get_hash(char* key) {
    int sum = 0;
    char current = key[0];
    int i = 0;
    while(current != '\0') {
        current = key[i];
        sum += (int)current;
        i++;
    }
    return sum % 1000;
}

int table_get_index(SymbolTable* table, char* key) {
    int i = table_get_hash(key);
    // printf("\nGET INDEX ~%s~! INDEX IS %d", key, i);
    while(i<table->size) {
        if((table->keys[i] != NULL) && (strcmp(table->keys[i], key)==0)) {
            // printf("\nKEY FOUND!");
            break;
        }
        i++;
    }
    // printf("\nRETURNING %d", i);
	return i;
}

int table_get_type(SymbolTable* table, char* key, int line_number) {
    return table_check_undeclared(table, key, line_number)->node_type;
}

Content* table_get_content(SymbolTable* table, lexValue key){
    return table_check_undeclared(table, key.label, key.line_number);
}

void table_add_entry(SymbolTable *table, char* key, Content* content) {
    // printf("\nADDING \"%s\": \"%s\" to the table %p\n", key, content->lex_data.label, table);
    table_check_declared(table, key, content->lex_data.line_number);
    if(content->node_type != NODE_TYPE_UNDECLARED)
        table_update_type(table, content->node_type);
    int hash = table_get_hash(key);
    // Se hash >= table->size, aumenta a tabela até hash+1 populando com null. Coloca na última pos.
    if(hash >= table->size) {
        int start = table->size;
	    table->size = hash+1;
        table->keys = realloc(table->keys, table->size * sizeof(char**));
        table->content = realloc(table->content, table->size * sizeof(Content**));
        for(int i=start; i<table->size; i++) {
            table->keys[i] = NULL;
            table->content[i] = NULL;
        }
        table->keys[table->size-1] = strdup(key);
        table->content[table->size-1] = content;
        if(content->node_type==NODE_TYPE_UNDECLARED){
            // printf("TYPELESS!");
            table->typeless = intList_pushLeft(table->typeless, table->size-1);
        }
    }
    // Se hash < table->size, encontra a primeira pos vazia e coloca.
    // Caso esteja cheia, table->size+=1. Coloca na última pos.
    else {
        int i;
        int is_on_table = false;
        for(i=hash; i<table->size; i++) {
            if(table->keys[i] == NULL) {
                table->keys[i] = strdup(key);
                table->content[i] = content;
                if(content->node_type == NODE_TYPE_UNDECLARED){
                    // printf("TYPELESS!");
                    table->typeless = intList_pushLeft(table->typeless, i);
                }
                is_on_table = true;
                break;
            }
        }
        if(i == table->size-1 && !is_on_table) {
            table->size += 1;
            table->keys = realloc(table->keys, table->size * sizeof(char**));
            table->content = realloc(table->content, table->size * sizeof(Content**));
            table->keys[table->size-1] = strdup(key);
            table->content[table->size-1] = content;
            if(content->node_type == NODE_TYPE_UNDECLARED){
                // printf("TYPELESS!");
                table->typeless = intList_pushLeft(table->typeless, table->size-1);
            }
        }
    }
}

void table_free(SymbolTable* table) {
    if(table->next != NULL)
        table_free(table->next);
    for(int i=0; i<table->size; i++) {
        if(table->content[i] != NULL) {
            content_free(table->content[i]);
            table->content[i] = NULL;
            free(table->keys[i]);
        }
    }
    free(table->keys);
    free(table->content);
    free(table);
}

void table_abort(SymbolTable* table) {
    if(table->parent != NULL)
        table_abort(table->parent);
    else
        table_free(table);
}

// Retorna o Content declarado caso exista na tabela. Caso contrario, retorna NULL.
Content* table_has_declared(SymbolTable* table, char* key) {
    int i = table_get_index(table, key);
    if((i < table->size) && (strcmp(table->keys[i], key) == 0)){
        return table->content[i];
    }
    else{
        if(table->parent == NULL)
            return NULL;
        else
            return table_has_declared(table->parent, key);
    }
}

void table_check_declared(SymbolTable* table, char* key, int line) {
    Content* declared_content = table_has_declared(table, key);
    if(declared_content != NULL) {
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Identificador %s previamente declarado na linha %d.\033[0m", line, key, declared_content->lex_data.line_number);
        //table_abort(table);
        exit(ERR_DECLARED);
    }
}

Content* table_check_undeclared(SymbolTable* table, char* key, int line) {
    Content* declared_content = table_has_declared(table, key);
    if(declared_content == NULL) {
        //table_abort(table);
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Identificador %s nao declarado.\033[0m", line, key);
        exit(ERR_UNDECLARED);
    }
    return declared_content;
}

void table_check_use(Content* content, int line) {
    if(content->nature == NAT_VAR) {
        if(content->dimensions != NULL) {
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Variavel %s sendo usada como array.\033[0m", line, content->lex_data.label);
            exit(ERR_VARIABLE);
        }
        if(content->args != NULL) {
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Variavel %s sendo usada como funcao.\033[0m", line, content->lex_data.label);
            exit(ERR_VARIABLE);
        }
    }
    if(content->nature == NAT_ARR) {
        if(content->args != NULL) {
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Arranjo %s sendo usada como funcao.\033[0m", line, content->lex_data.label);
            exit(ERR_ARRAY);
        }
        if(content->dimensions == NULL) {
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Arranjo %s sendo usada como variavel.\033[0m", line, content->lex_data.label);
            exit(ERR_ARRAY);
        }
    }
    if(content->nature == NAT_FUN) {
        if(content->dimensions != NULL) {
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Funcao %s sendo usada como arranjo.\033[0m", line, content->lex_data.label);
            exit(ERR_FUNCTION);
        }
        if(content->args == NULL) {
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Funcao %s sendo usada como variavel.\033[0m", line, content->lex_data.label);
            exit(ERR_FUNCTION);
        }
    }
}

SymbolTable* table_nest(SymbolTable* root) {
    // printf("\n---------------NEW SCOPE---------------");
    SymbolTable* new_table = table_new();
    SymbolTable* current = root;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = new_table;
    new_table->parent = current;
    table_flush_buffer(new_table);
    return new_table;
}

SymbolTable* table_pop_nest(SymbolTable* root) {
    if(root->parent != NULL){
        while(root->next != NULL)
            root = root->next;
        root = root->parent;
        table_print(root->next);
        table_free(root->next);
        root->next = NULL;
        return root;
    }
}

void table_update_type(SymbolTable* table, int type){
    IntList* typeless_idx = table->typeless;
    while(typeless_idx!=NULL){
        // printf("\nUPDATING TABLE[%d]:\"%s\" to %s\n", typeless_idx->value, table->keys[typeless_idx->value], int_to_type(type));
        table->content[typeless_idx->value]->node_type = type;
        typeless_idx = typeless_idx->next;
    }
    intList_free(table->typeless);
    table->typeless = intList_new();
}

void table_update_data_value(SymbolTable* table, char* key, ASTree* node_value){
    Content* declared_content = table_check_undeclared(table, key, node_value->data.line_number);
    if(node_value->data.value == NULL)
        declared_content->lex_data.value = strdup(node_value->data.label); 
    else
        declared_content->lex_data.value = strdup(node_value->data.value);
}

void table_add_to_buffer(Content* content){
    // printf("\nAdding \"%s\":\"%s\" to the buffer", key, content->lex_data.label);
    content_buffer = contentList_pushLeft(content_buffer, content);
}

void table_flush_buffer(SymbolTable* table){
    ContentList* current = contentList_new();
    current = table_dup_buffer();
    while(current!=NULL){
        // printf("\nFlushing \"%s\":\"%s\" to new context", current->key, current->value->lex_data.label);
        table_add_entry(table, current->value->lex_data.label, current->value);
        current = current->next;
    }
    contentList_free(content_buffer);
    content_buffer = contentList_new();
}

ContentList* table_dup_buffer(){
    return contentList_dup(content_buffer);
}

char* int_to_type(int i){
    switch (i) {
        case NODE_TYPE_BOOL:
            return "bool";
        case NODE_TYPE_CHAR:
            return "char";
        case NODE_TYPE_FLOAT:
            return "float";
        case NODE_TYPE_INT:
            return "int";
        default:
            return "TYPE_ERROR";
    }
}