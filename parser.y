%{
int yylex(void);
void yyerror (char const *s);
%}

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

%%

programa:           programa funcao
                    | programa var_glob
                    | ;

// VARIAVEIS

tipo_var:           TK_PR_INT
                    | TK_PR_FLOAT
                    | TK_PR_CHAR
                    | TK_PR_BOOL;

lits:               TK_LIT_FALSE
                    | TK_LIT_TRUE
                    | TK_LIT_INT
                    | TK_LIT_FLOAT
                    | TK_LIT_CHAR;

list_ident:         list_ident ',' TK_IDENTIFICADOR
                    | TK_IDENTIFICADOR;

var_atrib:          tipo_var TK_IDENTIFICADOR '<''=' lits;

list_dimensoes:     list_dimensoes '^' TK_LIT_INT
                    | TK_LIT_INT;

var_multidim:       tipo_var TK_IDENTIFICADOR ':' list_dimensoes;

var_loc:            tipo_var list_ident ';'
                    | var_atrib ';'
                    | var_multidim ';';

var_glob:           tipo_var list_ident ';'
                    | var_multidim ';';

// BLOCO COMANDOS

list_lits:          list_lits lits
                    | lits;

atrib:              TK_IDENTIFICADOR '=' lits
                    | var_multidim ':' list_lits;

list_args:          list_args ',' lits
                    | lits
                    | ;

chamada_func:       TK_IDENTIFICADOR '(' list_args ')' ';';

comando_simples:    var_loc
                    | atrib ';'
                    | chamada_func ';'
                    | TK_PR_RETURN ';';
                    // FALTA CONTROLE DE FLUXO

comando:            comando_simples
                    | ;

bloc_com:           '{' bloc_com_ou_nulo comando '}';

bloc_com_ou_nulo:   bloc_com
                    | ;

// FUNCOES

parametros:         parametros ',' var_loc
                    | var_loc
                    | ;

funcao:             tipo_var TK_IDENTIFICADOR '(' parametros ')' bloc_com;

%%
