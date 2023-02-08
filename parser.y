/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

%{
int yylex(void);
void yyerror (char const *error_message);

#include <stdio.h>

%}

%code requires { #include "AbstractSyntaxTree.h" }

%define parse.error verbose

%union {
  lexValue valor_lexico;
  ASTree *arvore;
}

%token<valor_lexico> TK_PR_INT
%token<valor_lexico> TK_PR_FLOAT
%token<valor_lexico> TK_PR_BOOL
%token<valor_lexico> TK_PR_CHAR
%token<valor_lexico> TK_PR_IF
%token<valor_lexico> TK_PR_THEN
%token<valor_lexico> TK_PR_ELSE
%token<valor_lexico> TK_PR_WHILE
%token<valor_lexico> TK_PR_INPUT
%token<valor_lexico> TK_PR_OUTPUT
%token<valor_lexico> TK_PR_RETURN
%token<valor_lexico> TK_PR_FOR
%token<valor_lexico> TK_OC_LE
%token<valor_lexico> TK_OC_GE
%token<valor_lexico> TK_OC_EQ
%token<valor_lexico> TK_OC_NE
%token<valor_lexico> TK_OC_AND
%token<valor_lexico> TK_OC_OR
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%token<valor_lexico> TK_LIT_CHAR
%token<valor_lexico> TK_IDENTIFICADOR
%token TK_ERRO

%type<arvore> lits
%type<arvore> tipo_var
%type<arvore> var_basic
%type<arvore> var_glob
%type<arvore> var_poly_glob
%type<arvore> var_multidim
%type<arvore> list_var
%type<arvore> list_dimensoes

%start programa_ou_vazio

%%

programa_ou_vazio:  programa
                    |;

programa:           programa var_glob
                    | programa funcao
                    | var_glob
                    | funcao;


// VARIAVEIS

tipo_var:           TK_PR_INT     { $$ = astree_new_node($1); }
                    | TK_PR_FLOAT { $$ = astree_new_node($1); }
                    | TK_PR_CHAR  { $$ = astree_new_node($1); }
                    | TK_PR_BOOL  { $$ = astree_new_node($1); };

var_basic:          tipo_var TK_IDENTIFICADOR { ASTree* ident = astree_new_node($2);
                                                astree_add_child($1, ident);
                                                $$ = $1;
                                                };

list_var:           list_var ',' TK_IDENTIFICADOR { astree_add_child($1, astree_new_node($3)); $$ = $1; }
                    | list_var ',' TK_IDENTIFICADOR TK_OC_LE expr
                    | list_var ',' TK_IDENTIFICADOR '[' list_dimensoes ']'
                    | TK_IDENTIFICADOR { $$ = astree_new_node($1); };
                    //| TK_IDENTIFICADOR TK_OC_LE expr
                    //| TK_IDENTIFICADOR '[' list_dimensoes ']';

var_poly_loc:       var_basic ',' list_var
                    | var_multidim ',' list_var
                    | var_inicializada ',' list_var;

var_poly_glob:      var_basic ',' list_var { astree_add_child($1->children[0], $3); $$ = $1; }
                    | var_multidim ',' list_var;

list_dimensoes:     list_dimensoes '^' TK_LIT_INT { astree_add_child(astree_get_leaf($1), astree_new_node($3)); $$ = $1; }
                    | TK_LIT_INT { $$ = astree_new_node($1); };

var_multidim:       var_basic '[' list_dimensoes ']'  { astree_add_child($1->children[0], $3); $$ = $1; };

var_glob:           var_basic ';'       { $$ = $1; } // astree_print_graphviz($$);
                    | var_poly_glob ';' { $$ = $1; }
                    | var_multidim ';'  { $$ = $1; astree_print_graphviz($$); };

lits:               TK_LIT_FALSE  { $$ = astree_new_node($1); }
                  | TK_LIT_TRUE   { $$ = astree_new_node($1); }
                  | TK_LIT_INT    { $$ = astree_new_node($1); }
                  | TK_LIT_FLOAT  { $$ = astree_new_node($1); }
                  | TK_LIT_CHAR   { $$ = astree_new_node($1); };

var_inicializada:   var_basic TK_OC_LE lits;

var_loc:            var_basic
                    | var_poly_loc
                    | var_inicializada;


// BLOCO COMANDOS

atribuicao:         TK_IDENTIFICADOR '=' expr
                    | TK_IDENTIFICADOR '[' list_expr ']' '=' expr;

arg:                expr
                    | ;

list_args:          list_args ',' arg
                    | arg;

comando_simples:    var_loc ';'
                    | atribuicao ';'
                    | chamada_func ';'
                    | TK_PR_RETURN expr ';'
                    | if_then_else ';'
                    | while ';'
                    | bloc_com ';';

comandos:           comandos comando_simples
		            | comando_simples;

bloc_com:           '{' comandos '}'
                    | '{' '}';

chamada_func:       TK_IDENTIFICADOR '(' list_args ')';


// EXPRESSOES

list_expr:          list_expr '^' expr
                    | expr;

expr:               expr_end
	                | expr_tier7;

expr_end:           '(' expr ')'
	                | TK_IDENTIFICADOR
	                | TK_IDENTIFICADOR '[' list_expr ']'
	                | lits
	                | chamada_func;

expr_tier7:         expr_end TK_OC_OR expr
                    | expr_tier6;

expr_tier6:         expr_end TK_OC_AND expr
                    | expr_tier5;

expr_tier5:         expr_end TK_OC_EQ expr
                    | expr_end TK_OC_NE expr
                    | expr_tier4;

expr_tier4:         expr_end '<' expr
                    | expr_end '>' expr
                    | expr_end TK_OC_LE expr
                    | expr_end TK_OC_GE expr
                    | expr_tier3;

expr_tier3:         expr_end '+' expr
                    | expr_end '-' expr
                    | expr_tier2;

expr_tier2:         expr_end '*' expr
                    | expr_end '/' expr
                    | expr_end '%' expr
                    | expr_tier1;

expr_tier1:         '-' expr
                    | '!' expr


// CONTROLE DE FLUXO

if_then:            TK_PR_IF '(' expr ')' TK_PR_THEN bloc_com;

if_then_else:       if_then
                    | if_then TK_PR_ELSE bloc_com;

while:              TK_PR_WHILE '(' expr ')' bloc_com;


// FUNCOES

parametros:         parametros ',' var_basic
                    | var_basic
                    | ;

funcao:             tipo_var TK_IDENTIFICADOR '(' parametros ')' bloc_com;

%%
