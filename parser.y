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

%start programa_ou_vazio

%left TK_OC_OR
%left TK_OC_AND
%left TK_OC_EQ TK_OC_NE
%left '>' '<' TK_OC_LE TK_OC_GE
%left '+' '-'
%left '*' '/' '%'
%left '!'

%%

programa_ou_vazio:  programa
                    |;

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

list_var:           list_var ',' TK_IDENTIFICADOR
                    | list_var ',' TK_IDENTIFICADOR TK_OC_LE expr
                    | list_var ',' TK_IDENTIFICADOR '[' list_dimensoes ']'
                    | TK_IDENTIFICADOR
                    | TK_IDENTIFICADOR TK_OC_LE expr
                    | TK_IDENTIFICADOR '[' list_dimensoes ']';

var_poly_loc:       var_basic ',' list_var
                    | var_multidim ',' list_var
                    | var_inicializada ',' list_var;

var_poly_glob:      var_basic ',' list_var
                    | var_multidim ',' list_var;

list_dimensoes:     list_dimensoes '^' TK_LIT_INT
                    | TK_LIT_INT;

var_multidim:       var_basic '[' list_dimensoes ']';

var_glob:           var_basic ';'
                    | var_poly_glob ';'
                    | var_multidim ';';

lits:               TK_LIT_FALSE
                    | TK_LIT_TRUE
                    | TK_LIT_INT
                    | TK_LIT_FLOAT
                    | TK_LIT_CHAR;

var_inicializada:   var_basic TK_OC_LE expr;

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
                    | if_then_else
                    | while
                    | bloc_com;

comandos:           comandos comando_simples
		            | comando_simples;

bloc_com:           '{' comandos '}'
                    | '{' '}';

chamada_func:       TK_IDENTIFICADOR '(' list_args ')';


// EXPRESSOES

list_expr:          list_expr '^' expr
                    | expr;

expr:               expr_end
	                | expr op_bin expr_end; // Operações binárias podem ser usadas com quaisquer expressões

expr_end:           '(' expr ')'
                    | op_pre expr_end // Prefixos só podem ser usados com identificador, literais, etc sozinhos OU com '(' expr ')'
	                | TK_IDENTIFICADOR
	                | TK_IDENTIFICADOR '[' list_expr ']'
	                | lits
	                | chamada_func;

op_bin:             '*'
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

op_pre:             '-' 
                    | '!';


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
