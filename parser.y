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
extern char* main_label;
extern int mainFound;


#include "src/Structures.h"
#include "src/Errors.h"
#include "src/AbstractSyntaxTree.h"
#include "src/ASTExpressions.h"
#include "src/Table.h"
#include "src/TablePrint.h"
#include "src/OpList.h"
#include "src/CodeGen.h"

char* return_label;

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

%type<astree> programa
%type<astree> comm_glob
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
%type<astree> args
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

%start programa

%%

programa:             comm_glob               { $$ = $1; arvore = $$; operacoes = ($$==NULL) ? NULL : $$->code; if(!mainFound) {emit_error(ERR_NO_MAIN, 0, NULL, NULL); } }
                    |                         { $$ = NULL; };

comm_glob:            dec_var_glob            { $$ = NULL; }
                    | funcao                  { $$ = $1; }
                    | dec_var_glob comm_glob  { $$ = $2; }
                    | funcao comm_glob        { ast_add_child($1, $2); $$ = $1; $$->code = opList_concatLeft($$->code, $2->code); };


// VARIAVEIS

tipo_var:             TK_PR_BOOL        { $$ = ast_new_node($1, NODE_TYPE_BOOL); }
                    | TK_PR_CHAR        { $$ = ast_new_node($1, NODE_TYPE_CHAR); }
                    | TK_PR_INT         { $$ = ast_new_node($1, NODE_TYPE_INT); }
                    | TK_PR_FLOAT       { $$ = ast_new_node($1, NODE_TYPE_FLOAT); };

lits:                 TK_LIT_FALSE      { $$ = ast_new_node($1, NODE_TYPE_BOOL); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, "0", NULL, $$->temp, NULL)); }
                    | TK_LIT_TRUE       { $$ = ast_new_node($1, NODE_TYPE_BOOL); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, "1", NULL, $$->temp, NULL)); }
                    | TK_LIT_INT        { $$ = ast_new_node($1, NODE_TYPE_INT); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, $1.value, NULL, $$->temp, NULL)); }
                    | TK_LIT_FLOAT      { $$ = ast_new_node($1, NODE_TYPE_FLOAT); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, $1.value, NULL, $$->temp, NULL)); }
                    | TK_LIT_CHAR       { $1.label[0]=$1.label[1]; $1.label[1] = '\0'; $1.label = realloc($1.label, sizeof(char)*2); free($1.value); $1.value = strdup($1.label); 
                                          $$ = ast_new_node($1, NODE_TYPE_CHAR); $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADI, $1.value, NULL, $$->temp, NULL)); 
                                        };

list_int:             TK_LIT_INT                { $$ = ast_new_node($1, NODE_TYPE_INT); }
                    | list_int '^' TK_LIT_INT   { $$ = ast_new_node($2, NODE_TYPE_INT); ast_add_child($$, $1); ast_add_child($$, ast_new_node($3, NODE_TYPE_INT)); };

dec_ident_multidim:   TK_IDENTIFICADOR '[' list_int ']'   { table_add_entry(escopo, $1.label, content_new($1, NAT_ARR, NODE_TYPE_UNDECLARED, NULL, intList_convert_tree($3), NULL));
                                                            free($2.label); $2.label=strdup("[]"); $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, NODE_TYPE_UNDECLARED)); ast_add_child($$, $3); 
                                                          };

ident_multidim:       TK_IDENTIFICADOR '[' list_expr ']'  { Content* content = table_get_content(escopo, $1.label, $1.line_number); table_check_use(content, NAT_ARR, $1.line_number);
                                                            free($2.label); $2.label=strdup("[]"); $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, table_get_type(escopo, $1.label, $1.line_number))); ast_add_child($$, $3); 
                                                          };

ident_init:           TK_IDENTIFICADOR TK_OC_LE lits      { char* temp = get_temp(); table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, temp, NULL, NULL)); free(temp);
                                                            $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, $3->node_type)); ast_add_child($$, $3);
                                                            $$->code = opList_pushLeft($$->code, op_new(OP_STOREAI, $3->temp, NULL, "rfp", "placeholder")); $$->code = opList_concatLeft($$->code, $3->code); 
                                                          };

dec_var_glob:         tipo_var var_glob ';'               { table_update_type(escopo, $1->node_type, NULL); ast_free($1); };

var_glob:             TK_IDENTIFICADOR                    { table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL));
                                                            free($1.label); $$ = NULL; 
                                                          }
                    | dec_ident_multidim                  { $$ = NULL; ast_free($1); }
                    | var_glob ',' TK_IDENTIFICADOR       { table_add_entry(escopo, $3.label, content_new($3, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL));
                                                            free($3.label); $$ = NULL; 
                                                          }
                    | var_glob ',' dec_ident_multidim     { $$ = NULL; };

var_loc:              TK_IDENTIFICADOR                    { table_add_entry(escopo, $1.label, content_new($1, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL));
                                                            free($1.label); $$ = NULL; 
                                                          }
                    | ident_init                          { $$ = $1; }
                    | var_loc ',' TK_IDENTIFICADOR        { table_add_entry(escopo, $3.label, content_new($3, NAT_VAR, NODE_TYPE_UNDECLARED, NULL, NULL, NULL));
                                                            free($3.label); 
                                                          }
                    | var_loc ',' ident_init              { ast_add_child($3, $1); $$ = $3;
                                                            $$->code = opList_concatLeft($$->code, $1->code); 
                                                          };

// FUNCOES

parametros:           tipo_var TK_IDENTIFICADOR                     { table_add_to_buffer(content_new($2, NAT_VAR, $1->node_type, NULL, NULL, NULL)); ast_free($1); free($2.label); }
                    | tipo_var TK_IDENTIFICADOR ',' parametros      { table_add_to_buffer(content_new($2, NAT_VAR, $1->node_type, NULL, NULL, NULL)); ast_free($1); free($2.label); }
                    | ;

funcao_dec:           tipo_var TK_IDENTIFICADOR '(' parametros ')'  { function_type_buffer = $1->node_type; char* label; 
                                                                      rfp_shift=2+calculate_total_size($1->node_type, NULL);
                                                                      if(strcmp($2.label, "main") == 0){label = main_label; mainFound = true;}
                                                                      else{label = get_label();}
                                                                      $2.value = label;
                                                                      return_label = get_label();
                                                                      table_add_entry(escopo, $2.label, content_new($2, NAT_FUN, $1->node_type, NULL, NULL, table_dup_buffer()));
                                                                      $$ = ast_new_node($2, $1->node_type); ast_free($1); 
                                                                    };

funcao:               funcao_dec bloc_com                           { $$ = $1; ast_add_child($$, $2); $$->code = opList_concatLeft($$->code, ($2==NULL) ? NULL : $2->code);
                                                                      Content* content = table_get_content(escopo, $$->data.label, $$->data.line_number);
                                                                      $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, content->lex_data.value, NULL, NULL, NULL));
                                                                      $$->code = opList_pushRight($$->code, op_new(OP_LABEL, return_label, NULL, NULL, NULL));
                                                                      char* temp = get_temp(); char* return_size = int_to_string(2+content->total_size);
                                                                      if(strcmp($1->data.label, "main") != 0) {
                                                                        $$->code = opList_pushRight($$->code, op_new(OP_I2I, "rfp", NULL, "rsp", NULL));
                                                                        $$->code = opList_pushRight($$->code, op_new(OP_LOAD, "rsp", NULL, "rfp", NULL));
                                                                        $$->code = opList_pushRight($$->code, op_new(OP_LOADAI, "rsp", "1", temp, NULL));
                                                                        $$->code = opList_pushRight($$->code, op_new(OP_JUMP, NULL, NULL, temp, NULL));
                                                                      }
                                                                      else {
                                                                        $$->code = opList_pushLeft($$->code, op_new(OP_ADDI, "rsp", return_size, "rsp", NULL));
                                                                        $$->code = opList_pushLeft($$->code, op_new(OP_I2I, "rfp", NULL, "rsp", NULL));
                                                                        $$->code = opList_pushLeft($$->code, op_new(OP_LOADI, "1024", NULL, "rfp", NULL));
                                                                        $$->code = opList_pushRight($$->code, op_new(OP_JUMPI, NULL, NULL, "L0", NULL));
                                                                      }
                                                                      free(temp); free(return_size);
                                                                    };


// BLOCO COMANDOS

atribuicao:           TK_IDENTIFICADOR '=' expr       { Content* content = table_get_content(escopo, $1.label, $1.line_number); table_check_use(content, NAT_VAR, $1.line_number);
                                                        $$ = ast_new_node($2, $3->node_type); ast_add_child($$, ast_new_node($1, table_get_type(escopo, content->lex_data.label, content->lex_data.line_number))); ast_add_child($$, $3);
                                                        ast_check_type($$->children[0], $$->children[1]);
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_STOREAI, $3->temp, NULL, get_dataRegister(content->scope), get_memShift(content->scope, content->mem_shift)));
                                                        $$->code = opList_concatLeft($$->code, $3->code); 
                                                      }
                    | ident_multidim '=' expr         { $$ = ast_new_node($2, $3->node_type); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                        ast_check_type($1, $3); 
                                                      };

comando_simples:      tipo_var var_loc                { ast_check_type($1, $2); $$ = $2; table_update_type(escopo, $1->node_type, ($2==NULL) ? NULL : $2->code); ast_free($1); }
                    | atribuicao                      { $$ = $1; }
                    | chamada_func                    { $$ = $1; }
                    | TK_PR_RETURN expr               { ast_check_type_node(((SymbolTable*)escopo)->return_type, $2);
                                                        $$ = ast_new_node($1, $2->node_type); ast_add_child($$, $2);
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_JUMPI, NULL, NULL, return_label, NULL));
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_STOREAI, $2->temp, NULL, "rfp", "2"));
                                                        $$->code = opList_concatLeft($$->code, $2->code); 
                                                      }
                    | if_then_else                    { $$ = $1; }
                    | while                           { $$ = $1; }
                    | bloc_com                        { $$ = $1; };

comandos:             comando_simples ';' comandos    { if($1==NULL){ $$ = $3;}
                                                        else{ast_add_child(ast_get_node($1), $3); $$=$1; $$->code = opList_concatRight($$->code, $3->code);} 
                                                      }
                    | comando_simples ';'             { $$ = $1; };

bloc_com_dec:         '{'                             { escopo = table_nest(escopo); };

bloc_com:             bloc_com_dec comandos '}'       { $$ = $2; $$->code = allocate_vars($$->code, escopo); escopo = table_pop_nest(escopo); }
                    | bloc_com_dec '}'                { escopo = table_pop_nest(escopo); $$ = NULL; };

args:                 expr                            { $$ = $1; }
                    | expr ',' args                   { ast_add_child($1, $3); $$ = $1; $$->code = opList_concatRight($$->code, $3->code); }
                    |                                 { $$ = NULL; };

chamada_func:         TK_IDENTIFICADOR '(' args ')'   { Content* content = table_get_content(escopo, $1.label, $1.line_number); table_check_use(content, NAT_FUN, $1.line_number);
                                                        char str[] = "call "; strcat(str, $1.label); free($1.label); $1.label=strdup(str); $$ = ast_new_node($1, content->node_type); ast_add_child($$, $3);
                                                        char* reg = get_temp(); $$->temp = get_temp();
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_LOADAI, "rsp", "2", $$->temp, NULL));
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_JUMPI, NULL, NULL, content->lex_data.value, NULL)); 
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_STOREAI, reg, NULL, "rfp", "1")); 
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_ADDI, "rpc", "3", reg, NULL)); 
                                                        $$->code = opList_concatLeft($$->code, generate_args(content, $3));
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_I2I, "rsp", NULL, "rfp", NULL));
                                                        $$->code = opList_pushLeft($$->code, op_new(OP_STORE, "rfp", NULL, "rsp", NULL));
                                                        if($3 != NULL) $$->code = opList_concatLeft($$->code, $3->code);
                                                        free(reg);
                                                      };


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
                                                $$->temp = get_temp(); $$->code = opList_pushLeft(NULL, op_new(OP_LOADAI, get_dataRegister(identifier->scope), get_memShift(identifier->scope, identifier->mem_shift), $$->temp, NULL));
                                                free(identifier); free($1.label); if($1.value!=NULL) free($1.value); 
                                              };

expr_tier7:           expr TK_OC_OR expr      { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; 
                                              }
                    | expr_tier6              { $$ = $1; };

expr_tier6:           expr TK_OC_AND expr     { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; 
                                              }
                    | expr_tier5              { $$ = $1; };

expr_tier5:           expr TK_OC_EQ expr      { ast_check_type($1, $3);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; 
                                              }
                    | expr TK_OC_NE expr      { ast_check_type($1, $3);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; 
                                              }
                    | expr_tier4              { $$ = $1; };

expr_tier4:           expr '<' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; 
                                              }
                    | expr '>' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; 
                                              }
                    | expr TK_OC_LE expr      { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; 
                                              }
                    | expr TK_OC_GE expr      { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                                $$->node_type = NODE_TYPE_BOOL; 
                                              }
                    | expr_tier3              { $$ = $1; };

expr_tier3:           expr '+' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                              }
                    | expr '-' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                              }
                    | expr_tier2              { $$ = $1; };

expr_tier2:           expr '*' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                              }
                    | expr '/' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                              }
                    | expr '%' expr           { ast_check_not_char($1, $3->node_type); ast_check_not_char($3, $1->node_type);
                                                $$ = ast_expr_node($1, $2, $3); ast_add_child($$, $1); ast_add_child($$, $3); 
                                              }
                    | expr_tier1              { $$ = $1; };

expr_tier1:           '-' expr                { ast_check_not_char($2, NODE_TYPE_INT);
                                                lexValue lv; lv.label = NULL; $$ = ast_expr_node(ast_new_node(lv, 0), $1, $2); ast_add_child($$, $2); 
                                              }
                    | '!' expr                { ast_check_not_char($2, NODE_TYPE_INT);
                                                lexValue lv; lv.label = NULL; $$ = ast_expr_node(ast_new_node(lv, 0), $1, $2); ast_add_child($$, $2); 
                                              };


// CONTROLE DE FLUXO

if_then_expr:         TK_PR_IF '(' expr ')' TK_PR_THEN        { ast_check_type_node(NODE_TYPE_BOOL, $3);
                                                                $$ = ast_new_node($1, NODE_TYPE_BOOL); ast_add_child($$, $3); free($5.label); 
                                                              };

if_then:              if_then_expr bloc_com                   { $$ = $1; ast_add_child($$, $2); };

if_then_else:         if_then                                 { $$ = $1; 
                                                                char* then_bloc = get_label(); char* next_command = get_label();
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, next_command, NULL, NULL, NULL));
                                                                $$->code = opList_concatLeft($$->code, $$->children[1]->code);
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, then_bloc, NULL, NULL, NULL));
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_CBR, $$->children[0]->temp, NULL, then_bloc, next_command));
                                                                $$->code = opList_concatLeft($$->code, $$->children[0]->code); 
                                                              }
                    | if_then TK_PR_ELSE bloc_com             { $$ = $1; ast_add_child($$, $3);
                                                                char* then_bloc = get_label(); char* else_bloc = get_label(); char* next_command = get_label();
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, next_command, NULL, NULL, NULL));
                                                                $$->code = opList_concatLeft($$->code, $$->children[2]->code);
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, else_bloc, NULL, NULL, NULL));
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_JUMPI, NULL, NULL, next_command, NULL));
                                                                $$->code = opList_concatLeft($$->code, $$->children[1]->code);
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, then_bloc, NULL, NULL, NULL));
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_CBR, $$->children[0]->temp, NULL, then_bloc, else_bloc));
                                                                $$->code = opList_concatLeft($$->code, $$->children[0]->code); 
                                                              };

while:                TK_PR_WHILE '(' expr ')' bloc_com       { ast_check_type_node(NODE_TYPE_BOOL, $3);
                                                                $$ = ast_new_node($1, NODE_TYPE_BOOL); ast_add_child($$, $3); ast_add_child($$, $5);
                                                                char* while_expr = get_label(); char* while_bloc = get_label(); char* next_command = get_label();
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, next_command, NULL, NULL, NULL));
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_JUMPI, NULL, NULL, while_expr, NULL));
                                                                $$->code = opList_concatLeft($$->code, $$->children[1]->code);
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, while_bloc, NULL, NULL, NULL));
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_CBR, $$->children[0]->temp, NULL, while_bloc, next_command));
                                                                $$->code = opList_concatLeft($$->code, $$->children[0]->code);
                                                                $$->code = opList_pushLeft($$->code, op_new(OP_LABEL, while_expr, NULL, NULL, NULL)); 
                                                              };

%%