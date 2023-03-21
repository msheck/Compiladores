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
extern void* operacoes;
extern int function_type_buffer;
extern int rbss_shift;
extern int rfp_shift;

#include "src/Structures.h"
#include "src/Errors.h"
#include "src/AbstractSyntaxTree.h"
#include "src/ASTExpressions.h"
#include "src/Table.h"
#include "src/TablePrint.h"
#include "src/OpList.h"
#include "src/CodeGen.h"

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
%type<astree> if_then_expr
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

lits:                 TK_LIT_FALSE  { $$ = ast_new_node($1, NODE_TYPE_BOOL); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, "0", NULL, $$->temp, NULL)); }
                    | TK_LIT_TRUE   { $$ = ast_new_node($1, NODE_TYPE_BOOL); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, "1", NULL, $$->temp, NULL)); }
                    | TK_LIT_INT    { $$ = ast_new_node($1, NODE_TYPE_INT); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, $1.value, NULL, $$->temp, NULL)); }
                    | TK_LIT_FLOAT  { $$ = ast_new_node($1, NODE_TYPE_FLOAT); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, $1.value, NULL, $$->temp, NULL)); }
                    | TK_LIT_CHAR   { $1.label[0]=$1.label[1]; $1.label[1] = '\0'; $1.label = realloc($1.label, sizeof(char)*2); free($1.value); $1.value = strdup($1.label); 
                                      $$ = ast_new_node($1, NODE_TYPE_CHAR); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, $1.value, NULL, $$->temp, NULL)); };

list_int:             TK_LIT_INT              { $$ = ast_new_node($1, NODE_TYPE_INT); }
                    | list_int '^' TK_LIT_INT { $$ = ast_new_node($2, NODE_TYPE_INT); ast_add_child($$, $1); ast_add_child($$, ast_new_node($3, NODE_TYPE_INT)); };

dec_ident_multidim:   TK_IDENTIFICADOR '[' list_int ']'   { table_add_entry(escopo, $1.label, content_new($1, NAT_ARR, NODE_TYPE_UNDECLARED, NULL, intList_convert_tree($3), NULL));
                                                            free($2.label); $2.label=strdup("[]"); $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, NODE_TYPE_UNDECLARED)); ast_add_child($$, $3); };

ident_multidim:       TK_IDENTIFICADOR '[' list_expr ']'  { Content* content = table_get_content(escopo, $1.label, $1.line_number); table_check_use(content, NAT_ARR, $1.line_number);
                                                            free($2.label); $2.label=strdup("[]"); $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, table_get_type(escopo, $1.label, $1.line_number))); ast_add_child($$, $3); };

ident_init:           TK_IDENTIFICADOR TK_OC_LE lits      { table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, $3->data.value, NULL, NULL)); 
                                                            $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, $3->node_type)); ast_add_child($$, $3); };

dec_var_glob:         tipo_var var_glob ';'               { table_update_type(escopo, $1->node_type); ast_free($1); };

var_glob:             TK_IDENTIFICADOR                    { table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL));

                                                            free($1.label); $$ = NULL; }
                    | dec_ident_multidim                  { $$ = NULL; ast_free($1); }
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

parametros:           tipo_var TK_IDENTIFICADOR                     { table_add_to_buffer(content_new($2, NAT_VAR, $1->node_type, NULL, NULL, NULL)); ast_free($1); free($2.label); }
                    | tipo_var TK_IDENTIFICADOR ',' parametros      { table_add_to_buffer(content_new($2, NAT_VAR, $1->node_type, NULL, NULL, NULL)); ast_free($1); free($2.label); }
                    | ;

funcao_dec:           tipo_var TK_IDENTIFICADOR '(' parametros ')'  { function_type_buffer = $1->node_type; table_add_entry(escopo, $2.label, content_new($2, NAT_FUN, $1->node_type, NULL, NULL, table_dup_buffer()));
                                                                      $$ = ast_new_node($2, $1->node_type); ast_free($1); 
                                                                      rfp_shift = 0; };

funcao:               funcao_dec bloc_com                           { $$ = $1; ast_add_child($$, $2); };


// BLOCO COMANDOS

atribuicao:           TK_IDENTIFICADOR '=' expr     { //table_update_data_value(escopo, $1.label, $3);
                                                      Content* content = table_get_content(escopo, $1.label, $1.line_number); table_check_use(content, NAT_VAR, $1.line_number);
                                                      $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, table_get_type(escopo, content->lex_data.label, content->lex_data.line_number))); ast_add_child($$, $3);
                                                      ast_check_type($$->children[0], $$->children[1]);
                                                      char mem_shift_buff[10], scope_buff[10]; sprintf(mem_shift_buff, "%d", content->mem_shift);  sprintf(scope_buff, "%d", content->mem_shift); Operation* op_buffer = op_new(OP_STOREAI, $3->temp, NULL, scope_buff, mem_shift_buff);
                                                      $$->code = opList_pushLeft($$->code, op_buffer); }
                    | ident_multidim '=' expr       { $$ = ast_new_node($2, $3->node_type); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                      ast_check_type($1, $3); };

arg:                  expr                          { $$ = $1; }
                    |                               { $$ = NULL; };

list_args:            arg                           { $$ = $1; }
                    | arg ',' list_args             { ast_add_child($1, $3); $$ = $1; };

comando_simples:      tipo_var var_loc              { ast_check_type($1, $2); $$ = $2; table_update_type(escopo, $1->node_type); ast_free($1);}
                    | atribuicao                    { $$ = $1; }
                    | chamada_func                  { $$ = $1; }
                    | TK_PR_RETURN expr             { ast_check_type_node(((SymbolTable*)escopo)->return_type, $2);
                                                      $$ = ast_new_node($1, $2->node_type); ast_add_child($$, $2); }
                    | if_then_else                  { $$ = $1; }
                    | while                         { $$ = $1; }
                    | bloc_com                      { $$ = $1; };

comandos:             comando_simples ';' comandos  { if($1==NULL){ $1 = $3;} else{ast_add_child(ast_get_node($1), $3);} $$=$1; $$->code = opList_concatLeft($$->code, $3->code); } // TODO: concatenar code
                    | comando_simples ';'           { $$ = $1; };

bloc_com_dec:         '{'                           { escopo = table_nest(escopo); };

bloc_com:             bloc_com_dec comandos '}'     { escopo = table_pop_nest(escopo); $$ = $2; }
                    | bloc_com_dec '}'              { escopo = table_pop_nest(escopo); $$ = NULL; };

chamada_func:         TK_IDENTIFICADOR '(' list_args ')'  { Content* content = table_get_content(escopo, $1.label, $1.line_number); table_check_use(content, NAT_FUN, $1.line_number);
                                                            int function_type = content->node_type;
                                                            char str[] = "call "; strcat(str, $1.label); free($1.label); $1.label=strdup(str); $$ = ast_new_node($1, function_type); ast_add_child($$, $3);};


// EXPRESSOES

list_expr:            expr                    { $$ = $1; }
                    | list_expr '^' expr      { $$ = ast_new_node($2, $3->node_type); ast_add_child($$, $1); ast_add_child($$, $3); };

expr:                 expr_end                { $$ = $1; }
	                  | expr_tier7              { $$ = $1; };

expr_end:             '(' expr ')'            { $$ = $2; }
                    | chamada_func            { $$ = $1; }
                    | ident_multidim          { $$ = $1; }
                    | lits                    { $$ = $1; }
                    | TK_IDENTIFICADOR        { table_check_use(table_get_content(escopo, $1.label, $1.line_number), NAT_VAR, $1.line_number);
                                                Content* identifier = content_dup(table_get_content(escopo, $1.label, $1.line_number)); $$ = ast_new_node(identifier->lex_data, identifier->node_type); 
                                                $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, identifier->lex_data.label, NULL, $$->temp, NULL));
                                                free(identifier); free($1.label); if($1.value!=NULL) free($1.value); };

expr_tier7:           expr TK_OC_OR expr      { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; }
                    | expr_tier6              { $$ = $1; };

expr_tier6:           expr TK_OC_AND expr     { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; }
                    | expr_tier5              { $$ = $1; };

expr_tier5:           expr TK_OC_EQ expr      { ast_check_type($1, $3);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; }
                    | expr TK_OC_NE expr      { ast_check_type($1, $3);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; }
                    | expr_tier4              { $$ = $1; };

expr_tier4:           expr '<' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; }
                    | expr '>' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; }
                    | expr TK_OC_LE expr      { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; }
                    | expr TK_OC_GE expr      { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; }
                    | expr_tier3              { $$ = $1; };

expr_tier3:           expr '+' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr '-' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier2              { $$ = $1; };

expr_tier2:           expr '*' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr '/' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr '%' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); }
                    | expr_tier1              { $$ = $1; };

expr_tier1:           '-' expr                { ast_check_not_char($2, NODE_TYPE_INT);
                                                lexValue lv; lv.label = NULL; $$ = ast_expr_node(ast_new_node(lv, 0), $1, $2); ast_add_child($$, $2); }
                    | '!' expr                { ast_check_not_char($2, NODE_TYPE_INT);
                                                lexValue lv; lv.label = NULL; $$ = ast_expr_node(ast_new_node(lv, 0), $1, $2); ast_add_child($$, $2); };


// CONTROLE DE FLUXO

if_then_expr:         TK_PR_IF '(' expr ')' TK_PR_THEN        { ast_check_type_node(NODE_TYPE_BOOL, $3);
                                                                $$ = ast_new_node($1, $3->node_type); ast_add_child($$, $3); free($5.label); };

if_then:              if_then_expr bloc_com                   { $$ = $1; ast_add_child($$, $2); };

if_then_else:         if_then                                 { $$ = $1; }
                    | if_then TK_PR_ELSE bloc_com             { $$ = $1; ast_add_child($$, $3);
                                                                char* t = get_label(); char* e = get_label();
                                                                $$->code = opList_concatLeft($$->code, $$->children[2]->code);
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, e, NULL, NULL, NULL));
                                                                $$->code = opList_concatLeft($$->code, $$->children[1]->code);
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, t, NULL, NULL, NULL));
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_CBR, $$->children[0]->temp, NULL, t, e));
                                                                opList_print($$->code); };

while:                TK_PR_WHILE '(' expr ')' bloc_com       { ast_check_type_node(NODE_TYPE_BOOL, $3);
                                                                $$ = ast_new_node($1, $3->node_type); ast_add_child($$, $3); ast_add_child($$, $5); };

%%