#include "AbstractSyntaxTree.h"
#include "Table.h"
// ---------------------------- DEFAULT ----------------------------
//#include <stdio.h>
extern int yyparse(void);
extern int yylex_destroy(void);

void *tabela = NULL;
void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main(int argc, char **argv)
{
    tabela = table_new();
	int ret = yyparse(); 
	exporta (arvore);
	libera(arvore);
	arvore = NULL;
	yylex_destroy();

    table_print_contexts(tabela);

	return ret;
}

// ---------------------------- MAIN ----------------------------
int main2(){
	lexValue lex_val1;
	lex_val1.line_number = 1;
	lex_val1.type = TYPE_LIT;
	lex_val1.label = strdup("conteudo1");
	lex_val1.value = NULL;

	Content *cont1 = content_new(lex_val1, NAT_LIT, NODE_TYPE_INT, NULL, NULL, NULL);
	// char* key1 = strdup("key1");
	SymbolTable *table1 = table_new();
	table_add_entry(table1, "key1", cont1);

	table_free(table1);
}

int main3(){
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
	lex_val1.label = strdup("conteudo1");
	lex_val1.value = NULL;

	lexValue lex_val2;
	lex_val2.line_number = 2;
	lex_val2.type = TYPE_LIT;
	lex_val2.label = strdup("conteudo2");
	lex_val2.value = NULL;

	lexValue lex_val3;
	lex_val3.line_number = 3;
	lex_val3.type = TYPE_LIT;
	lex_val3.label = strdup("conteudo3");
	lex_val3.value = NULL;

	lexValue lex_val4;
	lex_val4.line_number = 4;
	lex_val4.type = TYPE_LIT;
	lex_val4.label = strdup("conteudo4");
	lex_val4.value = NULL;

	lexValue lex_arg1;
	lex_arg1.line_number = 3;
	lex_arg1.type = TYPE_LIT;
	lex_arg1.label = strdup("arg1");
	lex_arg1.value = NULL;

	lexValue lex_arg2;
	lex_arg2.line_number = 3;
	lex_arg2.type = TYPE_LIT;
	lex_arg2.label = strdup("arg2");
	lex_arg2.value = NULL;

	lexValue lex_arg3;
	lex_arg3.line_number = 3;
	lex_arg3.type = TYPE_LIT;
	lex_arg3.label = strdup("arg3");
	lex_arg3.value = NULL;

	lexValue lex_arg4;
	lex_arg4.line_number = 3;
	lex_arg4.type = TYPE_LIT;
	lex_arg4.label = strdup("arg4");
	lex_arg4.value = NULL;
	printf("\n\n--------------\tLEXVALS CREATED\t--------------\n");

	IntList* dimensions = intList_new();
	dimensions = intList_pushLeft(dimensions, 1);
	dimensions = intList_pushLeft(dimensions, 2);
	dimensions = intList_pushLeft(dimensions, 3);
	printf("\n\n--------------\tDIMENSIONS CREATED\t--------------\n");

	Content *arg1 = content_new(lex_arg1, NAT_LIT, NODE_TYPE_INT, NULL, NULL, NULL);
	Content *arg2 = content_new(lex_arg2, NAT_LIT, NODE_TYPE_INT, NULL, NULL, NULL);
	Content *arg3 = content_new(lex_arg3, NAT_LIT, NODE_TYPE_INT, NULL, NULL, NULL);
	Content *arg4 = content_new(lex_arg4, NAT_LIT, NODE_TYPE_INT, NULL, NULL, NULL);
	ContentList *args1 = contentList_new();
	args1 = contentList_pushLeft(args1, arg1);
	args1 = contentList_pushLeft(args1, arg2);
	ContentList *args2 = contentList_new();
	args2 = contentList_pushLeft(args2, arg3);
	args2 = contentList_pushLeft(args2, arg4);
	printf("\n\n--------------\tARGUMENTS CREATED\t--------------\n");

	Content *cont1 = content_new(lex_val1, NAT_LIT, NODE_TYPE_INT, NULL, NULL, args1);
	Content *cont2 = content_new(lex_val2, NAT_LIT, NODE_TYPE_INT, NULL, NULL, args2);
	Content *cont3 = content_new(lex_val3, NAT_LIT, NODE_TYPE_INT, NULL, dimensions, NULL);
	Content *cont4 = content_new(lex_val4, NAT_LIT, NODE_TYPE_INT, NULL, NULL, NULL);
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

	printf("\ntable1.label[key1]: %s", table1->content[table_get_index(table1, key1)]->lex_value.label);
	printf("\ntable2.label[key3]: %s", table1->next->content[table_get_index(table2, key3)]->lex_value.label);
	table_pop_nest(table1);
	printf("\ntable1.label[key1]: %s", table1->content[table_get_index(table1, key1)]->lex_value.label);

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