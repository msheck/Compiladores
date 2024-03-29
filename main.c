#include "src/Structures.h"
#include "src/Errors.h"
#include "src/AbstractSyntaxTree.h"
#include "src/ASTExpressions.h"
#include "src/Content.h"
#include "src/Table.h"
#include "src/TablePrint.h"
#include "src/Assembler.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *tabela = NULL;
void *escopo = NULL;
void *escopo_anterior = NULL;
void *arvore = NULL;
void *operacoes = NULL;
void exporta (void *arvore);
void libera (void *arvore);
int rbss_shift = 0;
int rfp_shift = 0;
int rsp = 0;
int rfp = 0;
char* main_label;
int mainFound;

int main(int argc, char **argv)
{
	mainFound = false;
	main_label = get_label();
    tabela = table_new();
	escopo = tabela;
	int ret = yyparse(); 
	exporta(arvore);
	generateAsm(operacoes);
	//opList_print(operacoes);
	opList_free(operacoes);
	libera(arvore);
	arvore = NULL;

	table_free(tabela);
	
	yylex_destroy();

	return 0;
}