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
extern int rbss_shift;
extern int rfp_shift;

ContentList *content_buffer = NULL;
int function_type_buffer = NODE_TYPE_UNDECLARED;

SymbolTable *table_new() {
    SymbolTable* ret = NULL;
    ret = calloc(1, sizeof(SymbolTable));
    if(ret != NULL) {
        ret->scope_level = 0;
        ret->size = 0;
        ret->keys = NULL;
        ret->content = NULL;
        ret->next = NULL;
        ret->parent = NULL;
        ret->typeless = intList_new();
        ret->return_type = NODE_TYPE_UNDECLARED;
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

Content* table_get_content(SymbolTable* table, char* key, int line_number){
    return table_check_undeclared(table, key, line_number);
}

void table_add_entry(SymbolTable *table, char* key, Content* content) {
    //printf("\nADDING \"%s\": \"%s\" to the table %p\n", key, content->lex_data.label, table);
    table_check_declared(table, key, content->lex_data.line_number);
    content->table = table;
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
            table->typeless = intList_pushRight(table->typeless, table->size-1);
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
    if(declared_content != NULL && declared_content->table == table) {
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Identificador %s previamente declarado na linha %d.\033[0m", line, key, declared_content->lex_data.line_number);
        exit(ERR_DECLARED);
    }
}

Content* table_check_undeclared(SymbolTable* table, char* key, int line) {
    Content* declared_content = table_has_declared(table, key);
    if(declared_content == NULL) {
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Identificador %s nao declarado.\033[0m", line, key);
        exit(ERR_UNDECLARED);
    }
    return declared_content;
}

void table_check_use(Content* content, int expected_nature, int line_number) {
    if(expected_nature == content->nature) // No error, return
        return;
    switch (content->nature)
    {
        case NAT_ARR:
            if (expected_nature == NAT_VAR)
                emit_error(ERR_ARRAY, line_number, content->lex_data.label, "variavel");
            if (expected_nature == NAT_FUN)
                emit_error(ERR_ARRAY, line_number, content->lex_data.label, "funcao");
            break;
        case NAT_FUN:
            if(expected_nature == NAT_ARR)
                emit_error(ERR_FUNCTION, line_number, content->lex_data.label, "arranjo");
            if(expected_nature == NAT_VAR)
                emit_error(ERR_FUNCTION, line_number, content->lex_data.label, "variavel");
            break;
        case NAT_VAR:
            if(expected_nature == NAT_ARR)
                emit_error(ERR_VARIABLE, line_number, content->lex_data.label, "arranjo");
            if (expected_nature == NAT_FUN)
                emit_error(ERR_VARIABLE, line_number, content->lex_data.label, "funcao");
            break;
        
        default:
            break;
    }
}

SymbolTable* table_nest(SymbolTable* root) {
    //printf("\n---------------NEW SCOPE---------------");
    SymbolTable* new_table = table_new();
    new_table->return_type = function_type_buffer; // This is set by the parser
    new_table->scope_level = root->scope_level + 1;
    function_type_buffer = NODE_TYPE_UNDECLARED;
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
        //printf("\nREMOVING TABLE:");
        //table_print(root->next);
        table_free(root->next);
        root->next = NULL;
        //printf("\nNEW SCOPE:");
        //table_print(root);
        return root;
    }
}

void table_update_type(SymbolTable* table, int type){
    IntList* typeless_idx = table->typeless;
    while(typeless_idx!=NULL){
        if(type == NODE_TYPE_CHAR && table->content[typeless_idx->value]->nature == NAT_ARR)
            emit_error(ERR_CHAR_VECTOR, table->content[typeless_idx->value]->lex_data.line_number,  table->content[typeless_idx->value]->lex_data.label, NULL);
        table->content[typeless_idx->value]->node_type = type;
        table->content[typeless_idx->value]->total_size = calculate_total_size(type, table->content[typeless_idx->value]->dimensions);
        table->content[typeless_idx->value]->scope = table->scope_level;
        if (table->scope_level == 0) {
            table->content[typeless_idx->value]->mem_shift = rbss_shift;
            rbss_shift += table->content[typeless_idx->value]->total_size;
        }
        else {
            table->content[typeless_idx->value]->mem_shift = rfp_shift;
            rfp_shift += table->content[typeless_idx->value]->total_size;
        }
        typeless_idx = typeless_idx->next;
    }
    intList_free(table->typeless);
    table->typeless = intList_new();
}

void table_update_data_value(SymbolTable* table, char* key, ASTree* node_value){
    Content* declared_content = table_check_undeclared(table, key, node_value->data.line_number);
    if(declared_content->lex_data.value != NULL)
        free(declared_content->lex_data.value);
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
    ContentList* current_root = current;
    while(current!=NULL){
        // printf("\nFlushing \"%s\":\"%s\" to new context", current->key, current->value->lex_data.label);
        table_add_entry(table, current->value->lex_data.label, content_dup(current->value));
        current = current->next;
    }
    contentList_free(current_root);
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
            return "typeless";
    }
}