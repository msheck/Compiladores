/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "Table.h"
ContentList *content_buffer = NULL;

//---------------------------- TABLE ----------------------------

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

int table_get_type(SymbolTable* table, char* key) {
    int i = table_get_index(table, key);
    if(i != 0)
        return table->content[i]->node_type;
    else
        return 0;
}

Content* table_get_content(SymbolTable* table, lexValue key){
    int hash = table_check_undeclared(table, key.label, key.line_number);
    return table->content[hash];
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

// Retorna a linha em que o simbolo foi previamente declarado caso exista na tabela. Caso contrario, retorna 0.
int table_has_declared(SymbolTable* table, char* key) {
    int i = table_get_index(table, key);
    if((i < table->size) && (strcmp(table->keys[i], key) == 0)){
        return i;
    }
    else{
        if(table->parent == NULL)
            return 0;
        else
            return table_has_declared(table->parent, key);
    }
}

void table_check_declared(SymbolTable* table, char* key, int line) {
    int line_declared = table_has_declared(table, key);
    if(line_declared) {
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Identificador %s previamente declarado na linha %d.\033[0m", line, key, table->content[line_declared]->lex_data.line_number);
        //table_abort(table);
        exit(ERR_DECLARED);
    }
}

int table_check_undeclared(SymbolTable* table, char* key, int line) {
    int line_declared = table_has_declared(table, key);
    if(line_declared == 0) {
        //table_abort(table);
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Identificador %s nao declarado.\033[0m", line, key);
        exit(ERR_UNDECLARED);
    }
    return line_declared;
}

void table_check_use(SymbolTable* table, Content* content, int line) {
    if(content->nature == NAT_VAR) {
        if(content->dimensions != NULL) {
            //table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Variavel %s sendo usada como array.\033[0m", line, content->lex_data.label);
            exit(ERR_VARIABLE);
        }
        if(content->args != NULL) {
            //table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Variavel %s sendo usada como funcao.\033[0m", line, content->lex_data.label);
            exit(ERR_VARIABLE);
        }
    }
    if(content->nature == NAT_ARR) {
        if(content->args != NULL) {
            //table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Arranjo %s sendo usada como funcao.\033[0m", line, content->lex_data.label);
            exit(ERR_ARRAY);
        }
        if(content->dimensions == NULL) {
            //table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Arranjo %s sendo usada como variavel.\033[0m", line, content->lex_data.label);
            exit(ERR_ARRAY);
        }
    }
    if(content->nature == NAT_FUN) {
        if(content->dimensions != NULL) {
            //table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Funcao %s sendo usada como arranjo.\033[0m", line, content->lex_data.label);
            exit(ERR_FUNCTION);
        }
        if(content->args == NULL) {
            //table_abort(table);
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

void table_pop_nest(SymbolTable* root) {
    if(root != NULL){
        SymbolTable* current = root;
        SymbolTable* last = root;
        while(current->next != NULL) {
            last = current;
            current = current->next;
        }
        if(last->next != NULL) {
            table_free(last->next);
            contentList_free(content_buffer);
            last->next = NULL;
        }
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
    int hash = table_check_undeclared(table, key, node_value->data.line_number);
    if(node_value->data.value == NULL)
        table->content[hash]->lex_data.value = strdup(node_value->data.label); 
    else
        table->content[hash]->lex_data.value = strdup(node_value->data.value);
}

void table_add_to_buffer(Content* content, char* key){
    // printf("\nAdding \"%s\":\"%s\" to the buffer", key, content->lex_data.label);
    content_buffer = contentList_pushLeft(content_buffer, content);
    content_buffer->key = strdup(key);
}

void table_flush_buffer(SymbolTable* table){
    ContentList* current = content_buffer;
    while(current!=NULL){
        // printf("\nFlushing \"%s\":\"%s\" to new context", current->key, current->value->lex_data.label);
        table_add_entry(table, current->key, current->value);
        current = current->next;
    }
    // contentList_free(content_buffer);
}

ContentList* table_dup_buffer(){
    ContentList* ret = contentList_new();
    ContentList* current = content_buffer;
    while(current!=NULL){
        lexValue new_val = current->value->lex_data;
        if(current->value->lex_data.label != NULL)
            new_val.label = strdup(current->value->lex_data.label);
        else
            new_val.label = NULL;
        ret = contentList_pushLeft(ret, content_new(new_val, current->value->nature, current->value->node_type, current->value->lex_data.value, NULL, NULL));
        if(current->key != NULL)
            ret->key = strdup(current->key);
        else
            ret->key = NULL;
        current = current->next;
    }
/*  current = ret;
    printf("\n----DUPLICATE----");
    int i = 0;
    while(current!=NULL){
        printf("\nbuffer[%d] \"%s\":\"%s\":\"%s\":\"%s\"", i, current->key, current->value->lex_data.label, current->value->lex_data.value, current->value->data_value);
        current = current->next;
        i++;
    }
    printf("\n-----------------");*/
    return ret;
}

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
    printf("|  Printing table of size %04d%*c", table->size, 2*width-27, '|');
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
            printf("\n| %04d: (%s) %s = %s %*c", i, type, table->keys[i], table->content[i]->lex_data.value, (int)(width-(strlen(type)+strlen(table->keys[i])+value_len+13)),'|');
            if(table->content[i]->args == NULL)
                printf(" args is null %*c", width-13,'|');
            else{
                printf(" args: %*c", width-6,'|');
                ContentList *current = table->content[i]->args;
                while(current!=NULL){
                    printf("\n| %*c  %s%*c", width, '|', current->key, width-(int)strlen(current->key)-1, '|');
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

void _table_print_contexts(SymbolTable* table, int i) {
    i++;
    printf("\n=================  PRINTING CONTEXT %d!  =================\n", i);
    table_print(table);
    if(table->next != NULL)
        _table_print_contexts(table->next, i);
}

void table_print_contexts(SymbolTable* table) {
    int i = 1;
    printf("\n=============  PRINTING CONTEXT %02d! (ROOT)  ==============\n", i);
    table_print(table);
    if(table->next != NULL)
        _table_print_contexts(table->next, i);
}

//---------------------------- CONTENT ----------------------------

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

//---------------------------- INT LIST ----------------------------

IntList* intList_new() {
    return NULL;
}

IntList* intList_goToEnd(IntList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            return intList_goToEnd(list->next);
        else
            return list;
    }
}

IntList* intList_pushLeft(IntList* list, int value) {
    IntList* new_node;
    if(list == NULL) {
        new_node = malloc(sizeof(IntList*)+sizeof(NULL));
        new_node->next = NULL;
    }
    else {
        new_node = malloc(sizeof(IntList*)+sizeof(list));
        new_node->next = list;
    }
    new_node->value = value;
    //printf("\n%d: Pushing %ld before %ld", new_node->value, (long)new_node, (long)new_node->next);
    return new_node;
}

IntList* intList_pushRight(IntList* list, int value) {
    IntList* new_node;
    IntList* last_node = intList_goToEnd(list);
    if(list == NULL) {
        list = malloc(sizeof(IntList*)+sizeof(NULL));
        list->value = value;
        list->next = NULL;
        //printf("\n%d: Pushing %ld after %ld", list->value, (long)list, (long)last_node);
        return list;
    }
    else {
        new_node = malloc(sizeof(IntList*)+sizeof(list));
        last_node->next = new_node;
        new_node->next = NULL;
    }
    new_node->value = value;
    //printf("\n%d: Pushing %ld after %ld", new_node->value, (long)new_node, (long)last_node);
    return list;
}

IntList* intList_convert_tree(ASTree* ast) {
    ASTree* current = ast;
    IntList* ret = intList_new();
    while(strcmp(current->data.label, "^")==0){
        ret = intList_pushLeft(ret, atoi(current->children[current->number_of_children-1]->data.label));
        current = current->children[0];
    }
    ret = intList_pushLeft(ret, atoi(current->data.label));
    return ret;
}

void intList_free(IntList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            intList_free(list->next);
        //printf("\nFreeing %d: %ld...", list->value, (long)list );
        free(list);
    }    
}

void _intList_print(IntList* list) {
    printf("%d", list->value);
    if(list->next != NULL) {
        printf(", ");
        _intList_print(list->next);
    }
    else {
        printf(".");
    }
}

void intList_print(IntList* list) {
    printf("\n");
    _intList_print(list);
    printf("\n");
}

//---------------------------- CONTENT LIST ----------------------------

ContentList* contentList_new() {
    return NULL;
}

ContentList* contentList_goToEnd(ContentList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            return contentList_goToEnd(list->next);
        else
            return list;
    }
}

ContentList*  contentList_pushLeft(ContentList* list, Content* value) {
    ContentList* new_node;
    if(list == NULL) {
        new_node = malloc(sizeof(ContentList*)+sizeof(NULL));
        new_node->next = NULL;
    }
    else {
        new_node = malloc(sizeof(ContentList*)+sizeof(list));
        new_node->next = list;
    }
    new_node->value = value;
    //printf("\n%s: Pushing %ld before %ld", new_node->value->lex_data.label, (long)new_node, (long)new_node->next);
    return new_node;
}

ContentList* contentList_pushRight(ContentList* list, Content* value) {
    ContentList* new_node;
    ContentList* last_node = contentList_goToEnd(list);
    if(list == NULL) {
        list = malloc(sizeof(ContentList*)+sizeof(NULL));
        list->value = value;
        list->next = NULL;
        //printf("\n%s: Pushing %ld after %ld", list->value->lex_data.label, (long)list, (long)last_node);
        return list;
    }
    else {
        new_node = malloc(sizeof(ContentList*)+sizeof(list));
        last_node->next = new_node;
        new_node->next = NULL;
    }
    new_node->value = value;
    //printf("\n%s: Pushing %ld after %ld", new_node->value->lex_data.label, (long)new_node, (long)last_node);
    return list;
}

void contentList_free(ContentList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            contentList_free(list->next);
        // printf("\nFreeing %s: %ld...", list->value->lex_data.label, (long)list );
        content_free(list->value);
        free(list->key);
        free(list);
        list = NULL;
    }
}

void _contentList_print(ContentList* list) {
    printf("%s", list->value->lex_data.label);
    if(list->next != NULL){
        printf(", ");
        _contentList_print(list->next);
    }
    else{
        printf(".");
    }
}

void contentList_print(ContentList* list) {
    printf("\n");
    _contentList_print(list);
    printf("\n");
}

//---------------------------- MISC ----------------------------

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