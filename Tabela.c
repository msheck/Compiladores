#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Tabela.h"

// ---------------------------- MAIN ----------------------------

int main(){
    /*INT LIST*/
    IntList *list_ints = intList_new();
    list_ints = intList_pushLeft(list_ints, 667);
    list_ints = intList_pushLeft(list_ints, 420);
    list_ints = intList_pushLeft(list_ints, 69);
    list_ints = intList_pushLeft(list_ints, 505);
    list_ints = intList_pushLeft(list_ints, 137);
    printf("\n\n--------------\tINT LIST PUSHING COMPLETE\t--------------\n");

    intList_print(list_ints);

    intList_free(list_ints);
    printf("\n\n--------------\tINT LIST FREEING COMPLETE\t--------------\n");

    /*CONTENT*/
    lexValue lex_val1;
    lex_val1.line_number = 1;
    lex_val1.type = TYPE_LIT;
    lex_val1.value = strdup("conteudo1");

    lexValue lex_val2;
    lex_val2.line_number = 2;
    lex_val2.type = TYPE_LIT;
    lex_val2.value = strdup("conteudo2");

    lexValue lex_val3;
    lex_val3.line_number = 3;
    lex_val3.type = TYPE_LIT;
    lex_val3.value = strdup("conteudo3");

    lexValue lex_val4;
    lex_val4.line_number = 4;
    lex_val4.type = TYPE_LIT;
    lex_val4.value = strdup("conteudo4");

    lexValue lex_arg1;
    lex_arg1.line_number = 3;
    lex_arg1.type = TYPE_LIT;
    lex_arg1.value = strdup("arg1");

    lexValue lex_arg2;
    lex_arg2.line_number = 3;
    lex_arg2.type = TYPE_LIT;
    lex_arg2.value = strdup("arg2");

    lexValue lex_arg3;
    lex_arg3.line_number = 3;
    lex_arg3.type = TYPE_LIT;
    lex_arg3.value = strdup("arg3");

    lexValue lex_arg4;
    lex_arg4.line_number = 3;
    lex_arg4.type = TYPE_LIT;
    lex_arg4.value = strdup("arg4");
    printf("\n\n--------------\tLEXVALS CREATED\t--------------\n");

    IntList* dimensions = intList_new();
    dimensions = intList_pushLeft(dimensions, 1);
    dimensions = intList_pushLeft(dimensions, 2);
    dimensions = intList_pushLeft(dimensions, 3);
    printf("\n\n--------------\tDIMENSIONS CREATED\t--------------\n");

    Content *arg1 = content_new(lex_arg1, NAT_LIT, NODE_TYPE_INT, NULL, NULL);
    Content *arg2 = content_new(lex_arg2, NAT_LIT, NODE_TYPE_INT, NULL, NULL);
    Content *arg3 = content_new(lex_arg3, NAT_LIT, NODE_TYPE_INT, NULL, NULL);
    Content *arg4 = content_new(lex_arg4, NAT_LIT, NODE_TYPE_INT, NULL, NULL);
    ContentList *args1 = contentList_new();
    args1 = contentList_pushLeft(args1, arg1);
    args1 = contentList_pushLeft(args1, arg2);
    ContentList *args2 = contentList_new();
    args2 = contentList_pushLeft(args2, arg3);
    args2 = contentList_pushLeft(args2, arg4);
    printf("\n\n--------------\tARGUMENTS CREATED\t--------------\n");

    Content *cont1 = content_new(lex_val1, NAT_LIT, NODE_TYPE_INT, NULL, args1);
    Content *cont2 = content_new(lex_val2, NAT_LIT, NODE_TYPE_INT, NULL, args2);
    Content *cont3 = content_new(lex_val3, NAT_LIT, NODE_TYPE_INT, dimensions, NULL);
    Content *cont4 = content_new(lex_val4, NAT_LIT, NODE_TYPE_INT, NULL, NULL);
    printf("\n\n--------------\tCONTENT NEW COMPLETE\t--------------\n");

    /*CONTENT LIST*/
    /*ContentList *list_cont = contentList_new();
    list_cont = contentList_pushLeft(list_cont, cont1);
    list_cont = contentList_pushLeft(list_cont, cont2);
    list_cont = contentList_pushLeft(list_cont, cont3);
    list_cont = contentList_pushLeft(list_cont, cont4);
    printf("\n\n--------------\tCONTENT LIST PUSHING COMPLETE\t--------------\n");

    contentList_print(list_cont);

    contentList_free(list_cont);
    printf("\n\n--------------\tCONTENT LIST FREEING COMPLETE\t--------------\n");*/

    // intList_free(list_ints);
    // printf("\n\n--------------\tINT LIST FREEING COMPLETE\t--------------\n");

    char* key1 = strdup("key1");
    char* key2 = strdup("key2");
    char* key3 = strdup("key3");
    char* key4 = strdup("key4");
    SymbolTable *table1 = table_new();
    table_add_entry(table1, key1, cont1);
    table_add_entry(table1, key2, cont2);
    SymbolTable *table2 = table_new();
    table_add_entry(table2, key3, cont3);
    table_add_entry(table2, key4, cont4);
    table_nest(table1, table2);

    /* TESTES PARA ERROS DE USO DE IDENTIFICADORES */
    // table_check_declared(table2, key1, __LINE__); // Descomentar p/ testar ERR_DECLARED
    // table_check_undeclared(table2, "key7", __LINE__); // Descomentar p/ testar ERR_UNDECLARED
    
    // Content *cont_err_1 = content_new(lex_arg1, NAT_VAR, NODE_TYPE_INT, dimensions, NULL);
    // table_check_use(table2, cont_err_1, __LINE__);

    // Content *cont_err_2 = content_new(lex_arg2, NAT_VAR, NODE_TYPE_INT, NULL, args1);
    // table_check_use(table2, cont_err_2, __LINE__);

    // Content *cont_err_3 = content_new(lex_arg3, NAT_ARR, NODE_TYPE_INT, NULL, NULL);
    // table_check_use(table2, cont_err_3, __LINE__);

    // Content *cont_err_4 = content_new(lex_arg3, NAT_ARR, NODE_TYPE_INT, NULL, args1);
    // table_check_use(table2, cont_err_4, __LINE__);

    // Content *cont_err_5 = content_new(lex_arg4, NAT_FUN, NODE_TYPE_INT, NULL, NULL);
    // table_check_use(table2, cont_err_5, __LINE__);

    // Content *cont_err_6 = content_new(lex_arg4, NAT_FUN, NODE_TYPE_INT, dimensions, NULL);
    // table_check_use(table2, cont_err_6, __LINE__);

    printf("\ntable1.value[key1]: %s", table1->content[table_get_index(table1, key1)]->lex_value.value);
    printf("\ntable2.value[key3]: %s", table1->next->content[table_get_index(table2, key3)]->lex_value.value);
    table_pop_nest(table1);
    printf("\ntable1.value[key1]: %s", table1->content[table_get_index(table1, key1)]->lex_value.value);

    table_free(table1);

/*
    IntList *intList = intList_new();
    printf("\nPUSHING");
    intList = intList_pushLeft(intList, 1);
    intList = intList_pushLeft(intList, 2);
    intList = intList_pushLeft(intList, 3);
    intList = intList_pushLeft(intList, 4);
    intList = intList_pushLeft(intList, 69);
    
    printf("\nPRINTING");
    intList_print(intList);

    printf("\nFREEING");
    intList_free(intList);


    //SymbolTable *table = table_new();
    lexValue lex_val;
    lex_val.line_number = 3;
    lex_val.type = TYPE_LIT;
    lex_val.value = strdup("1");
    
    lexValue lex_val2;
    lex_val2.line_number = 3;
    lex_val2.type = TYPE_LIT;
    lex_val2.value = strdup("2");
    printf("\nLEXVALS CREATED");
    
    lexValue lex_val3;
    lex_val3.line_number = 3;
    lex_val3.type = TYPE_LIT;
    lex_val3.value = strdup("3");
    printf("\nLEXVALS CREATED");

    int **dimensions = malloc(sizeof(int*) * 3);
    int dim1 = 1;
    int dim2 = 2;
    int dim3 = 3;
    dimensions[0] = &dim1;
    dimensions[1] = &dim2;
    dimensions[2] = &dim3;
    printf("\nDIMS CREATED");

    Content *arg1 = content_new(lex_val, NAT_LIT, NODE_TYPE_INT, NULL, 0, NULL, 0);
    Content *arg2 = content_new(lex_val2, NAT_LIT, NODE_TYPE_INT, NULL, 0, NULL, 0);
    Content** args = malloc(sizeof(Content*)*2);
    args[0] = arg1;
    args[1] = arg2;
    printf("\nARGS CREATED");
    printf("\nARGS CREATED");
    printf("\nARGS CREATED");
    printf("\nARGS CREATED");

    //Content *content = content_new(lex_val, NAT_LIT, NODE_TYPE_INT, NULL, NULL);
    Content *content = content_new(lex_val, NAT_LIT, NODE_TYPE_INT, dimensions, 3, args, 2);
    printf("\nCONTENT CREATED");
    printf("\nCONTENT CREATED");
    printf("\nCONTENT CREATED");
    printf("\nCONTENT CREATED");



    Content *content1 = content_new(lex_val, NAT_LIT, NODE_TYPE_INT, dimensions, 3, args, 2);
    Content *content2 = content_new(lex_val2, NAT_LIT, NODE_TYPE_INT, dimensions, 3, args, 2);
    Content *content3 = content_new(lex_val3, NAT_LIT, NODE_TYPE_INT, dimensions, 3, args, 2);
    ContentList *cList = contentList_new();
    printf("\nPUSHING");
    cList = contentList_pushLeft(cList, content1);
    cList = contentList_pushLeft(cList, content2);
    cList = contentList_pushLeft(cList, content3);
    
    printf("\nPRINTING");
    contentList_print(cList);

    printf("\nFREEING");
    contentList_free(cList);

    //table_add_entry(table, "key0", content);
    printf("\nENTRY ADDED");
    printf("\nENTRY ADDED");
    printf("\nENTRY ADDED");
    printf("\nENTRY ADDED");

    //printf("\nkey: %s", table->keys[table_get_index(table, "key0")]);
    //printf("\nvalue: %s\n", table->content[table_get_index(table, "key0")]->lex_value.value);

    //table_free(table);
    //printf("\nFREE DONE!!!\n");
*/
    printf("\n\n\n");
    return 0;
}

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
    while(i<table->size) {
        if(table->keys[i]==key) {
            break;
        }
        i++;
    }
	return i;
}

int table_get_type(SymbolTable* table, char* key) {
    int i = table_get_index(table, key);
    return table->content[i]->node_type;
}

void table_add_entry(SymbolTable *table, char* key, Content* content) {
    table_check_declared(table, key, content->lex_value.line_number);
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
        table->keys[table->size-1] = key;
        table->content[table->size-1] = content;
    }
    // Se hash < table->size, encontra a primeira pos vazia e coloca.
    // Caso esteja cheia, table->size+=1. Coloca na última pos.
    else {
        int i;
        int is_on_table = false;
        for(i=hash; i<table->size; i++) {
            if(table->keys[i] == NULL) {
                table->keys[i] = key;
                table->content[i] = content;
                is_on_table = true;
                break;
            }
        }
        if(i == table->size-1 && !is_on_table) {
            table->size += 1;
            table->keys = realloc(table->keys, table->size * sizeof(char**));
            table->content = realloc(table->content, table->size * sizeof(Content**));
            table->keys[table->size-1] = key;
            table->content[table->size-1] = content;
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
    for(int i=0; i<table->size; i++) {
        if(table->keys[i] == key) {
            return table->content[i]->lex_value.line_number;
        }
    }
    if(table->parent == NULL)
        return false;
    return table_has_declared(table->parent, key);
}

void table_check_declared(SymbolTable* table, char* key, int line) {
    int line_declared = table_has_declared(table, key);
    if(line_declared) {
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Identificador %s previamente declarado na linha %d.\033[0m", line, key, line_declared);
        table_abort(table);
        exit(ERR_DECLARED);
    }
}

void table_check_undeclared(SymbolTable* table, char* key, int line) {
    int line_declared = table_has_declared(table, key);
    printf("\nDECLARADO NA LINHA %d", line_declared);
    if(line_declared==0) {
        table_abort(table);
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Identificador %s nao declarado.\033[0m", line, key);
        exit(ERR_UNDECLARED);
    }
}

void table_check_use(SymbolTable* table, Content* content, int line) {
    if(content->nature==NAT_VAR) {
        if(content->dimensions!=NULL) {
            table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Variavel %s sendo usada como array.\033[0m", line, content->lex_value.value);
            exit(ERR_VARIABLE);
        }
        if(content->args!=NULL) {
            table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Variavel %s sendo usada como funcao.\033[0m", line, content->lex_value.value);
            exit(ERR_VARIABLE);
        }
    }
    if(content->nature==NAT_ARR) {
        if(content->args!=NULL) {
            table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Arranjo %s sendo usada como funcao.\033[0m", line, content->lex_value.value);
            exit(ERR_ARRAY);
        }
        if(content->dimensions==NULL) {
            table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Arranjo %s sendo usada como variavel.\033[0m", line, content->lex_value.value);
            exit(ERR_ARRAY);
        }
    }
    if(content->nature==NAT_FUN) {
        if(content->dimensions!=NULL) {
            table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Funcao %s sendo usada como arranjo.\033[0m", line, content->lex_value.value);
            exit(ERR_FUNCTION);
        }
        if(content->args==NULL) {
            table_abort(table);
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Funcao %s sendo usada como variavel.\033[0m", line, content->lex_value.value);
            exit(ERR_FUNCTION);
        }
    }
}

void table_nest(SymbolTable* root, SymbolTable* new_table) {
    SymbolTable* current = root;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = new_table;
    new_table->parent = current;
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
            last->next = NULL;
        }
    }
}

//---------------------------- CONTENT ----------------------------

Content* content_new(lexValue lex_val, int nat, int node_type, IntList *dimensions, ContentList *args) {
    Content* content = NULL;
    content = calloc(1, sizeof(Content));
    content->lex_value = lex_val;
    content->nature = nat;
    content->node_type = node_type;
    content->dimensions = dimensions;
    content->args = args;
    content->total_size = calculate_total_size(node_type, dimensions);
    return content;
}

void content_free(Content *content) {
    if (content->dimensions != NULL) {
        intList_free(content->dimensions);
        printf("\n\tDimensions freed.");
    }
    if (content->args != NULL) {
        contentList_free(content->args);
        printf("\n\tArguments freed.");
    }
    free(content->lex_value.value);
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
    printf("\n%d: Pushing %ld before %ld", new_node->value, (long)new_node, (long)new_node->next);
    return new_node;
}

IntList* intList_pushRight(IntList* list, int value) {
    IntList* new_node;
    IntList* last_node = intList_goToEnd(list);
    if(list == NULL) {
        list = malloc(sizeof(IntList*)+sizeof(NULL));
        list->value = value;
        list->next = NULL;
        printf("\n%d: Pushing %ld after %ld", list->value, (long)list, (long)last_node);
        return list;
    }
    else {
        new_node = malloc(sizeof(IntList*)+sizeof(list));
        last_node->next = new_node;
        new_node->next = NULL;
    }
    new_node->value = value;
    printf("\n%d: Pushing %ld after %ld", new_node->value, (long)new_node, (long)last_node);
    return list;
}

void intList_free(IntList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            intList_free(list->next);
        printf("\nFreeing %d: %ld...", list->value, (long)list );
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
    printf("\n%s: Pushing %ld before %ld", new_node->value->lex_value.value, (long)new_node, (long)new_node->next);
    return new_node;
}

ContentList* contentList_pushRight(ContentList* list, Content* value) {
    ContentList* new_node;
    ContentList* last_node = contentList_goToEnd(list);
    if(list == NULL) {
        list = malloc(sizeof(ContentList*)+sizeof(NULL));
        list->value = value;
        list->next = NULL;
        printf("\n%s: Pushing %ld after %ld", list->value->lex_value.value, (long)list, (long)last_node);
        return list;
    }
    else {
        new_node = malloc(sizeof(ContentList*)+sizeof(list));
        last_node->next = new_node;
        new_node->next = NULL;
    }
    new_node->value = value;
    printf("\n%s: Pushing %ld after %ld", new_node->value->lex_value.value, (long)new_node, (long)last_node);
    return list;
}

void contentList_free(ContentList* list) {
    if(list != NULL) {
        if(list->next != NULL)
            contentList_free(list->next);
        printf("\nFreeing %s: %ld...", list->value->lex_value.value, (long)list );
        content_free(list->value);
        free(list);
    }
}

void _contentList_print(ContentList* list) {
    printf("%s", list->value->lex_value.value);
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