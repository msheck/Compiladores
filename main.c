#include "src/Structures.h"
#include "src/Errors.h"
#include "src/AbstractSyntaxTree.h"
#include "src/ASTExpressions.h"
#include "src/Content.h"
#include "src/Table.h"
#include "src/TablePrint.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *tabela = NULL;
void *escopo = NULL;
void *arvore = NULL;
void *operacoes = NULL;
void exporta (void *arvore);
void libera (void *arvore);
int rbss_shift = 0;
int rfp_shift = 0;

int main(int argc, char **argv)
{
    tabela = table_new();
	escopo = tabela;
	int ret = yyparse(); 
    table_print_contexts(tabela);
	exporta (arvore);
	libera(arvore);
	arvore = NULL;

	table_free(tabela);
	
	yylex_destroy();

	return 0;
}

/* Comentário:
	Não tivemos tempo o suficiente para fazer o trabalho. Faltou organização? Não. Começamos o trabalho logo depois do
	laboratório. Em conjunto, gastamos por volta de 30h nesta etapa. Ambos estudamos e trabalhamos, não podíamos dedicar
	mais tempo.
*/