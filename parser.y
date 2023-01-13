%{
int yylex(void);
void yyerror (char const *error_message);

#include <stdio.h>
%}

%define parse.error verbose

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_FOR
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_IDENTIFICADOR
%token TK_ERRO

%start programa

%%

programa:           programa var_glob
                    | programa funcao
                    | var_glob
                    | funcao;


// VARIAVEIS

tipo_var:           TK_PR_INT
                    | TK_PR_FLOAT
                    | TK_PR_CHAR
                    | TK_PR_BOOL;

var_basic:          tipo_var TK_IDENTIFICADOR;

list_ident:         list_ident ',' TK_IDENTIFICADOR
                    | TK_IDENTIFICADOR;

var_poly:           var_basic ',' list_ident;

list_dimensoes:     list_dimensoes '^' TK_LIT_INT
                    | TK_LIT_INT;

var_multidim:       var_basic '[' list_dimensoes ']';

var_glob:           var_basic ';'
                    | var_poly ';'
                    | var_multidim ';';

lits:               TK_LIT_FALSE
                    | TK_LIT_TRUE
                    | TK_LIT_INT
                    | TK_LIT_FLOAT
                    | TK_LIT_CHAR;

var_inicializada:   var_basic '<''=' lits;

var_loc:            var_basic
                    | var_poly
                    | var_inicializada;


// BLOCO COMANDOS

list_lits:          list_lits '^' lits
                    | lits;

atribuicao:         TK_IDENTIFICADOR '=' lits
                    | var_multidim '=' '[' list_lits ']';

arg:                lits
                    | expr
                    | ;

list_args:          list_args ',' arg
                    | arg;

comando_simples:    var_loc ';'
                    | atribuicao ';'
                    | chamada_func ';'
                    | TK_PR_RETURN ';'
                    | if_then_else
                    | while;

comandos:           comandos comando_simples
		            | comando_simples;

bloc_com:           '{' comandos '}'
                    | '{' '}';

chamada_func:       TK_IDENTIFICADOR '(' list_args ')';


// EXPRESSOES

list_expr:          list_expr '^' expr
                    | expr;

expr_ident:         TK_IDENTIFICADOR
                    | TK_IDENTIFICADOR '[' list_expr ']';

expr_operando:      expr_ident
                    | lits
                    | chamada_func;

expr_op_prefix:     '-'
                    | '!';

expr_op_bin:        '*'
                    | '/'
                    | '%'
                    | '+'
                    | '-'
                    | '<'
                    | '>'
                    | TK_OC_LE
                    | TK_OC_GE
                    | TK_OC_EQ
                    | TK_OC_NE
                    | TK_OC_AND
                    | TK_OC_OR;

expr_bin:           expr_operando expr_op_bin expr_operando;

expr_prefix:        expr_op_prefix expr_operando;

expr_unit:          expr_bin
                    | expr_prefix
                    | expr_op_prefix expr_prefix;

expr_end:           expr
                    | expr_operando;

pre_bracket:        expr_op_prefix
                    | ;

post_bracket:       expr_op_bin expr_end
                    | ;

expr:               pre_bracket '(' expr ')' post_bracket
                    | expr_unit expr_op_bin expr_end
                    | expr_unit;


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
