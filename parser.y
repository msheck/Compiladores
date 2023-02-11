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

// %type<arvore> programa_ou_vazio
// %type<arvore> programa
// %type<arvore> tipo_var
%type<arvore> var_basic
// %type<arvore> list_var
// %type<arvore> var_poly_loc
// %type<arvore> var_poly_glob
// %type<arvore> list_dimensoes
// %type<arvore> var_multidim
// %type<arvore> var_glob
%type<arvore> lits
%type<arvore> var_inicializada
%type<arvore> var_loc
// %type<arvore> parametros
%type<arvore> funcao
// %type<arvore> atribuicao
// %type<arvore> arg
// %type<arvore> list_arg
%type<arvore> comando_simples
%type<arvore> comandos
%type<arvore> bloc_com
// %type<arvore> chamada_func
// %type<arvore> list_expr
// %type<arvore> expr
// %type<arvore> expr_end
// %type<arvore> expr_tier7
// %type<arvore> expr_tier6
// %type<arvore> expr_tier5
// %type<arvore> expr_tier4
// %type<arvore> expr_tier3
// %type<arvore> expr_tier2
// %type<arvore> expr_tier1
// %type<arvore> if_then
// %type<arvore> if_then_else
// %type<arvore> while

%type<arvore> ';'
%type<arvore> ','
%type<arvore> '^'
%type<arvore> '='
%type<arvore> '<'
%type<arvore> '>'
%type<arvore> '+'
%type<arvore> '-'
%type<arvore> '*'
%type<arvore> '/'
%type<arvore> '%'

%start programa_ou_vazio

%%

programa_ou_vazio:    programa
                    |;

programa:             programa var_glob
                    | programa funcao
                    | var_glob
                    | funcao;


// VARIAVEIS

tipo_var:             TK_PR_INT
                    | TK_PR_FLOAT
                    | TK_PR_CHAR
                    | TK_PR_BOOL;

var_basic:          tipo_var TK_IDENTIFICADOR { $$ = ast_new_node($2); };

list_var:             TK_IDENTIFICADOR
                    | TK_IDENTIFICADOR TK_OC_LE expr
                    | TK_IDENTIFICADOR '[' list_dimensoes ']'
                    | TK_IDENTIFICADOR ',' list_var
                    | TK_IDENTIFICADOR TK_OC_LE expr ',' list_var
                    | TK_IDENTIFICADOR '[' list_dimensoes ']' ',' list_var;

var_poly_loc:         var_basic ',' list_var
                    | var_multidim ',' list_var
                    | var_inicializada ',' list_var;

var_poly_glob:        var_basic ',' list_var
                    | var_multidim ',' list_var;

list_dimensoes:       TK_LIT_INT
                    | TK_LIT_INT '^' list_dimensoes;

var_multidim:       var_basic '[' list_dimensoes ']';

var_glob:             var_basic ';'
                    | var_poly_glob ';'
                    | var_multidim ';';

lits:               TK_LIT_FALSE  { $$ = ast_new_node($1); }
                  | TK_LIT_TRUE   { $$ = ast_new_node($1); }
                  | TK_LIT_INT    { $$ = ast_new_node($1); }
                  | TK_LIT_FLOAT  { $$ = ast_new_node($1); }
                  | TK_LIT_CHAR   { $$ = ast_new_node($1); };

var_inicializada:   var_basic TK_OC_LE lits { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); };

var_loc:            //  var_basic         { $$ = $1; }
                    //| var_poly_loc      { $$ = $1; }
                     var_inicializada  { $$ = $1; };


// FUNCOES

parametros:           var_basic
                    | var_basic ',' parametros
                    | ;

funcao:             tipo_var TK_IDENTIFICADOR '(' parametros ')' bloc_com { $$ = ast_new_node($2); ast_add_child($$, $6); ast_print_graphviz($$); };


// BLOCO COMANDOS

atribuicao:           TK_IDENTIFICADOR '=' expr
                    | TK_IDENTIFICADOR '[' list_expr ']' '=' expr;

arg:                expr
                    | ;

list_args:            arg
                    | arg ',' list_args;

comando_simples:      var_loc ';'           { $$ = $1; };
                    // | atribuicao ';'
                    // | chamada_func ';'
                    // | TK_PR_RETURN expr ';'
                    // | if_then_else ';'
                    // | while ';'
                    // | bloc_com ';';

comandos:             comando_simples { $$ = $1; };
		            // | comando_simples comandos;

bloc_com:             '{' comandos '}' { $$ = $2; };
                    // | '{' '}';

chamada_func:       TK_IDENTIFICADOR '(' list_args ')';


// EXPRESSOES

list_expr:            expr
                    | expr '^' list_expr;

expr:                 expr_end
	                | expr_tier7;

expr_end:             '(' expr ')'
	                | TK_IDENTIFICADOR
	                | TK_IDENTIFICADOR '[' list_expr ']'
	                | lits
	                | chamada_func;

expr_tier7:           expr_end TK_OC_OR expr
                    | expr_tier6;

expr_tier6:           expr_end TK_OC_AND expr
                    | expr_tier5;

expr_tier5:           expr_end TK_OC_EQ expr
                    | expr_end TK_OC_NE expr
                    | expr_tier4;

expr_tier4:           expr_end '<' expr
                    | expr_end '>' expr
                    | expr_end TK_OC_LE expr
                    | expr_end TK_OC_GE expr
                    | expr_tier3;

expr_tier3:           expr_end '+' expr
                    | expr_end '-' expr
                    | expr_tier2;

expr_tier2:           expr_end '*' expr
                    | expr_end '/' expr
                    | expr_end '%' expr
                    | expr_tier1;

expr_tier1:           '-' expr
                    | '!' expr


// CONTROLE DE FLUXO

if_then:            TK_PR_IF '(' expr ')' TK_PR_THEN bloc_com;

if_then_else:         if_then
                    | if_then TK_PR_ELSE bloc_com;

while:              TK_PR_WHILE '(' expr ')' bloc_com;

%%
