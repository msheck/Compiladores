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
#include <string.h>

extern void* arvore;

%}

%code requires { #include "AbstractSyntaxTree.h" }

%define parse.error verbose

%union {
  lexValue valor_lexico;
  ASTree *astree;
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

%type<valor_lexico> '['
%type<valor_lexico> '!'
%type<valor_lexico> '^'
%type<valor_lexico> '='
%type<valor_lexico> '<'
%type<valor_lexico> '>'
%type<valor_lexico> '+'
%type<valor_lexico> '-'
%type<valor_lexico> '*'
%type<valor_lexico> '/'
%type<valor_lexico> '%'

%type<astree> programa_ou_vazio
%type<astree> programa
%type<astree> ident_multidim
%type<astree> var_poly_glob
%type<astree> var_multidim
%type<astree> lits
%type<astree> var_inicializada
%type<astree> var_loc
%type<astree> funcao
%type<astree> atribuicao
%type<astree> arg
%type<astree> list_args
%type<astree> comando_simples
%type<astree> comandos
%type<astree> bloc_com
%type<astree> chamada_func
%type<astree> list_expr
%type<astree> expr
%type<astree> expr_end
%type<astree> expr_tier7
%type<astree> expr_tier6
%type<astree> expr_tier5
%type<astree> expr_tier4
%type<astree> expr_tier3
%type<astree> expr_tier2
%type<astree> expr_tier1
%type<astree> if_then
%type<astree> if_then_else
%type<astree> while

%start programa_ou_vazio

%%

programa_ou_vazio:    programa          { $$ = $1; arvore = $$; }
                    |                   { $$ = NULL; };

programa:             var_glob          { $$ = NULL; }
                    | funcao            { $$ = $1; }
                    | var_glob programa { $$ = $2; }
                    | funcao programa   { ast_add_child($1, $2); $$ = $1; };


// VARIAVEIS

tipo_var:             TK_PR_INT   { free($1.value); }
                    | TK_PR_FLOAT { free($1.value); }
                    | TK_PR_CHAR  { free($1.value); }
                    | TK_PR_BOOL  { free($1.value); };

// var_basic:          tipo_var TK_IDENTIFICADOR { $$ = ast_new_node($2); };

ident_multidim:     TK_IDENTIFICADOR '[' list_expr ']'  { free($2.value); $2.value=strdup("[]"); $$ = ast_new_node($2); ast_add_child($$, ast_new_node($1)); ast_add_child($$, $3); };

list_var:             TK_IDENTIFICADOR                            { free($1.value); }
                    | TK_IDENTIFICADOR TK_OC_LE expr              { free($1.value); free($2.value); }
                    | ident_multidim
                    | TK_IDENTIFICADOR ',' list_var               { free($1.value); }
                    | TK_IDENTIFICADOR TK_OC_LE expr ',' list_var { free($1.value); free($2.value); }
                    | ident_multidim ',' list_var;

var_poly_loc:         tipo_var TK_IDENTIFICADOR ',' list_var  { free($2.value); }       
                    | var_multidim ',' list_var
                    | var_inicializada ',' list_var;

var_poly_glob:        tipo_var TK_IDENTIFICADOR ',' list_var  { free($2.value); }
                    | var_multidim ',' list_var;

list_dimensoes:       TK_LIT_INT                      { free($1.value); }
                    | TK_LIT_INT '^' list_dimensoes   { free($1.value); free($2.value); };

var_multidim:       tipo_var TK_IDENTIFICADOR '[' list_dimensoes ']'  { free($2.value); free($3.value); };

var_glob:             tipo_var TK_IDENTIFICADOR ';' { free($2.value); }
                    | var_poly_glob ';'
                    | var_multidim ';';

lits:                 TK_LIT_FALSE  { $$ = ast_new_node($1); }
                    | TK_LIT_TRUE   { $$ = ast_new_node($1); }
                    | TK_LIT_INT    { $$ = ast_new_node($1); }
                    | TK_LIT_FLOAT  { $$ = ast_new_node($1); }
                    | TK_LIT_CHAR   { $$ = ast_new_node($1); };

var_inicializada:   tipo_var TK_IDENTIFICADOR TK_OC_LE lits { $$ = ast_new_node($3); ast_add_child($$, ast_new_node($2)); ast_add_child($$, $4); };

var_loc:              tipo_var TK_IDENTIFICADOR { $$ = $$; free($2.value); }
                    | var_poly_loc              { $$ = $$; }
                    | var_inicializada          { $$ = $1; };


// FUNCOES

parametros:           tipo_var TK_IDENTIFICADOR                 { free($2.value); }
                    | tipo_var TK_IDENTIFICADOR ',' parametros  { free($2.value); }
                    | ;

funcao:             tipo_var TK_IDENTIFICADOR '(' parametros ')' bloc_com { $$ = ast_new_node($2); ast_add_child($$, $6); };


// BLOCO COMANDOS

atribuicao:           TK_IDENTIFICADOR '=' expr     { $$ = ast_new_node($2); ast_add_child($$, ast_new_node($1)); ast_add_child($$, $3); }
                    | ident_multidim '=' expr       { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); };

arg:                expr                            { $$ = $1; }
                    |                               { $$ = $$; };

list_args:            arg                           { $$ = $1; }
                    | arg ',' list_args             { ast_add_child($1, $3); $$ = $1; };

comando_simples:      var_loc                       { $$ = $1; }
                    | atribuicao                    { $$ = $1; }
                    | chamada_func                  { $$ = $1; }
                    | TK_PR_RETURN expr             { $$ = ast_new_node($1); ast_add_child($$, $2); }
                    | if_then_else                  { $$ = $1; }
                    | while                         { $$ = $1; }
                    | bloc_com                      { $$ = $1; };

comandos:             comando_simples ';' comandos  { ast_add_child($1, $3); $$=$1; }
                    | comando_simples ';'           { $$ = $1; };

bloc_com:             '{' comandos '}'              { $$ = $2; }
                    | '{' '}'                       { $$ = $$; };

chamada_func:       TK_IDENTIFICADOR '(' list_args ')'  { char str[] = "call "; strcat(str, $1.value); free($1.value); $1.value=strdup(str); $$ = ast_new_node($1); ast_add_child($$, $3);};


// EXPRESSOES

list_expr:            expr                    { $$ = $1; }
                    | expr '^' list_expr      { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); };


expr:                 expr_end                { $$ = $1; }
	                  | expr_tier7              { $$ = $1; };

expr_end:             '(' expr ')'            { $$ = $2; }
                    | TK_IDENTIFICADOR        { $$ = ast_new_node($1); }
                    | ident_multidim          { $$ = $1; }
                    | lits                    { $$ = $1; }
                    | chamada_func            { $$ = $1; };

expr_tier7:           expr_end TK_OC_OR expr  { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier6              { $$ = $1; };

expr_tier6:           expr_end TK_OC_AND expr { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier5              { $$ = $1; };

expr_tier5:           expr_end TK_OC_EQ expr  { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_end TK_OC_NE expr  { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier4              { $$ = $1; };

expr_tier4:           expr_end '<' expr       { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_end '>' expr       { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_end TK_OC_LE expr  { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_end TK_OC_GE expr  { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier3              { $$ = $1; };

expr_tier3:           expr_end '+' expr       { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_end '-' expr       { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier2              { $$ = $1; };

expr_tier2:           expr_end '*' expr       { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_end '/' expr       { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_end '%' expr       { $$ = ast_new_node($2); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier1              { $$ = $1; };

expr_tier1:           '-' expr                { $$ = ast_new_node($1); ast_add_child($$, $2); }
                    | '!' expr                { $$ = ast_new_node($1); ast_add_child($$, $2); };


// CONTROLE DE FLUXO

if_then:            TK_PR_IF '(' expr ')' TK_PR_THEN bloc_com { $$ = ast_new_node($1); ast_add_child($$, $3); ast_add_child($$, $6); };

if_then_else:       if_then                                   { $$ = $1; }
                    | if_then TK_PR_ELSE bloc_com             { $$ = $1; ast_add_child($$, $3); };

while:              TK_PR_WHILE '(' expr ')' bloc_com         { $$ = ast_new_node($1); ast_add_child($$, $3); ast_add_child($$, $5); };

%%
