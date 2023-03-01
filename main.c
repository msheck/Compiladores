#include "AbstractSyntaxTree.h"
#include "Table.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *tabela = NULL;
void *escopo = NULL;
void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main(int argc, char **argv)
{
    tabela = table_new();
	escopo = tabela;
	int ret = yyparse(); 
	exporta (arvore);
	// libera(arvore);
	arvore = NULL;
	yylex_destroy();

	// table_free(tabela);
    table_print_contexts(tabela);

	return 0;
}

/* Comentário:
	Não tivemos tempo o suficiente para fazer o trabalho. Faltou organização? Não. Começamos o trabalho logo depois do
	laboratório. Em conjunto, gastamos por volta de 30h nesta etapa. Ambos estudamos e trabalhamos, não podíamos dedicar
	mais tempo.
*/