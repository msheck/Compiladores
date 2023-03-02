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

extern void* arvore;
extern void* tabela;
extern void* escopo;

#include "src/Structures.h"
#include "src/AbstractSyntaxTree.h"
#include "src/ASTExpressions.h"
#include "src/Table.h"
#include "src/TablePrint.h"

%}

%code requires { #include "src/AbstractSyntaxTree.h" }
%code requires { #include "src/ASTExpressions.h" }
%code requires { #include "src/Content.h" }
%code requires { #include "src/Table.h" }
%code requires { #include "src/TablePrint.h" }

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
%type<astree> tipo_var
%type<astree> lits
%type<astree> list_int
%type<astree> dec_ident_multidim
%type<astree> ident_multidim
%type<astree> ident_init
%type<astree> var_glob
%type<astree> var_loc
%type<astree> funcao_dec
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

%left TK_OC_OR
%left TK_OC_AND
%left TK_OC_EQ TK_OC_NE
%left '>' '<' TK_OC_LE TK_OC_GE
%left '+' '-'
%left '*' '/' '%'
%left '!'

%start programa_ou_vazio

%%

programa_ou_vazio:    programa              { $$ = $1; arvore = $$; }
                    |                       { $$ = NULL; };

programa:             dec_var_glob          { $$ = NULL; }
                    | funcao                { $$ = $1; }
                    | dec_var_glob programa { $$ = $2; }
                    | funcao programa       { ast_add_child($1, $2); $$ = $1; };


// VARIAVEIS

tipo_var:             TK_PR_BOOL    { $$ = ast_new_node($1, NODE_TYPE_BOOL); }
                    | TK_PR_CHAR    { $$ = ast_new_node($1, NODE_TYPE_CHAR); }
                    | TK_PR_INT     { $$ = ast_new_node($1, NODE_TYPE_INT); }
                    | TK_PR_FLOAT   { $$ = ast_new_node($1, NODE_TYPE_FLOAT); };

lits:                 TK_LIT_FALSE  { $$ = ast_new_node($1, NODE_TYPE_BOOL); }
                    | TK_LIT_TRUE   { $$ = ast_new_node($1, NODE_TYPE_BOOL); }
                    | TK_LIT_INT    { $$ = ast_new_node($1, NODE_TYPE_INT); }
                    | TK_LIT_FLOAT  { $$ = ast_new_node($1, NODE_TYPE_FLOAT); }
                    | TK_LIT_CHAR   { $1.label[0]=$1.label[1]; $1.label[1] = '\0'; $1.label = realloc($1.label, sizeof(char)*2); $$ = ast_new_node($1, NODE_TYPE_CHAR); };

list_int:             TK_LIT_INT              { $$ = ast_new_node($1, NODE_TYPE_INT); }
                    | list_int '^' TK_LIT_INT { $$ = ast_new_node($2, NODE_TYPE_INT); ast_add_child($$, $1); ast_add_child($$, ast_new_node($3, NODE_TYPE_INT)); };

dec_ident_multidim:   TK_IDENTIFICADOR '[' list_int ']'   { table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, intList_convert_tree($3), NULL));
                                                            free($2.label); $2.label=strdup("[]"); $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, NODE_TYPE_UNDECLARED)); ast_add_child($$, $3); };

ident_multidim:       TK_IDENTIFICADOR '[' list_expr ']'  { table_check_undeclared(escopo, $1.label, $1.line_number);
                                                            free($2.label); $2.label=strdup("[]"); $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, table_get_type(escopo, $1.label))); ast_add_child($$, $3); };

ident_init:           TK_IDENTIFICADOR TK_OC_LE lits      { table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, $3->data.value, NULL, NULL)); 
                                                            $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, $3->node_type)); ast_add_child($$, $3); };

dec_var_glob:         tipo_var var_glob ';'               { table_update_type(escopo, $1->node_type); ast_free($1); };

var_glob:             TK_IDENTIFICADOR                    { table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL));
                                                            free($1.label); $$ = NULL; }
                    | dec_ident_multidim                  { $$ = NULL; }
                    | var_glob ',' TK_IDENTIFICADOR       { table_add_entry(escopo, $3.label, content_new($3, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL)); 
                                                            free($3.label); $$ = NULL; }
                    | var_glob ',' dec_ident_multidim     { $$ = NULL; };

var_loc:              TK_IDENTIFICADOR                    { table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL)); 
                                                            free($1.label); $$ = NULL; }
                    | ident_init                          { $$ = $1; }
                    | var_loc ',' TK_IDENTIFICADOR        { table_add_entry(escopo, $3.label, content_new($3, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL)); 
                                                            free($3.label); }
                    | var_loc ',' ident_init              { ast_add_child($3, $1); $$ = $3; };

// FUNCOES

parametros:           tipo_var TK_IDENTIFICADOR                   { table_add_to_buffer(content_new($2, NAT_VAR, $1->node_type, NULL, NULL, NULL), $2.label); ast_free($1); }
                    | tipo_var TK_IDENTIFICADOR ',' parametros    { table_add_to_buffer(content_new($2, NAT_VAR, $1->node_type, NULL, NULL, NULL), $2.label); ast_free($1); }
                    | ;

funcao_dec:           tipo_var TK_IDENTIFICADOR '(' parametros ')'  { table_add_entry(escopo, $2.label, content_new($2, NAT_FUN, $1->node_type, NULL, NULL, table_dup_buffer()));
                                                                      $$ = ast_new_node($2, $1->node_type); ast_free($1); };

funcao:               funcao_dec bloc_com                           { $$ = $1; ast_add_child($$, $2); };


// BLOCO COMANDOS

atribuicao:           TK_IDENTIFICADOR '=' expr     { table_update_data_value(escopo, $1.label, $3);
                                                      $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, table_get_type(escopo, $1.label))); ast_add_child($$, $3); }
                    | ident_multidim '=' expr       { $$ = ast_new_node($2, $3->node_type); ast_add_child($$, $1); ast_add_child($$, $3); };

arg:                  expr                          { $$ = $1; }
                    |                               { $$ = NULL; };

list_args:            arg                           { $$ = $1; }
                    | arg ',' list_args             { ast_add_child($1, $3); $$ = $1; };

comando_simples:      tipo_var var_loc              { $$ = $2; table_update_type(escopo, $1->node_type); ast_free($1); }
                    | atribuicao                    { $$ = $1; }
                    | chamada_func                  { $$ = $1; }
                    | TK_PR_RETURN expr             { $$ = ast_new_node($1, $2->node_type); ast_add_child($$, $2); }
                    | if_then_else                  { $$ = $1; }
                    | while                         { $$ = $1; }
                    | bloc_com                      { $$ = $1; };

comandos:             comando_simples ';' comandos  { if($1==NULL){ $1 = $3;} else{ast_add_child(ast_get_node($1), $3);} $$=$1; }
                    | comando_simples ';'           { $$ = $1; };

bloc_com_dec:         '{'                           {escopo = table_nest(escopo);};

bloc_com:             bloc_com_dec comandos '}'     { $$ = $2; }
                    | bloc_com_dec '}'              { $$ = NULL; };

chamada_func:         TK_IDENTIFICADOR '(' list_args ')'  { char str[] = "call "; strcat(str, $1.label); free($1.label); $1.label=strdup(str); $$ = ast_new_node($1, $3->node_type); ast_add_child($$, $3);};
                                                                                                                                                                   //table_get_type(escopo, $1.label)

// EXPRESSOES

list_expr:            expr                    { $$ = $1; }
                    | list_expr '^' expr      { $$ = ast_new_node($2, $3->node_type); ast_add_child($$, $1); ast_add_child($$, $3); };

expr:                 expr_end                { $$ = $1; }
	                  | expr_tier7              { $$ = $1; };

expr_end:             '(' expr ')'            { $$ = $2; }
                    | TK_IDENTIFICADOR        { Content* identifier = table_get_content(escopo, $1); $$ = ast_new_node(identifier->lex_data, identifier->node_type); }
                    | ident_multidim          { $$ = $1; }
                    | lits                    { $$ = $1; }
                    | chamada_func            { $$ = $1; };

expr_tier7:           expr TK_OC_OR expr      { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier6              { $$ = $1; };

expr_tier6:           expr TK_OC_AND expr     { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier5              { $$ = $1; };

expr_tier5:           expr TK_OC_EQ expr      { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr TK_OC_NE expr      { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier4              { $$ = $1; };

expr_tier4:           expr '<' expr           { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr '>' expr           { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr TK_OC_LE expr      { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr TK_OC_GE expr      { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier3              { $$ = $1; };

expr_tier3:           expr '+' expr           { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr '-' expr           { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier2              { $$ = $1; };

expr_tier2:           expr '*' expr           { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr '/' expr           { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr '%' expr           { $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier1              { $$ = $1; };

expr_tier1:           '-' expr                { lexValue lv; lv.label = NULL; $$ = ast_expr_node(ast_new_node(lv, 0), $1, $2); ast_add_child($$, $2); }
                    | '!' expr                { lexValue lv; lv.label = NULL; $$ = ast_expr_node(ast_new_node(lv, 0), $1, $2); ast_add_child($$, $2); };


// CONTROLE DE FLUXO

if_then:              TK_PR_IF '(' expr ')' TK_PR_THEN bloc_com { $$ = ast_new_node($1, $3->node_type); ast_add_child($$, $3); ast_add_child($$, $6); };

if_then_else:         if_then                                 { $$ = $1; }
                    | if_then TK_PR_ELSE bloc_com             { $$ = $1; ast_add_child($$, $3); };

while:                TK_PR_WHILE '(' expr ')' bloc_com       { $$ = ast_new_node($1, $3->node_type); ast_add_child($$, $3); ast_add_child($$, $5); };

%%