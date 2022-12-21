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

programa:           programa func
                    | programa var_glob
                    | ;

tipo-var:           TK_PR_INT
                    | TK_PR_FLOAT
                    | TK_PR_CHAR
                    | TK_PR_BOOL;

list-identificador: list-identificador ',' TK_IDENTIFICADOR
                    | TK_IDENTIFICADOR;

variavel-declarada  TK_PR_BOOL TK_IDENTIFICADOR '<''=' TK_LIT_FALSE
                    | TK_PR_BOOL TK_IDENTIFICADOR '<''=' TK_LIT_TRUE
                    | TK_PR_INT TK_IDENTIFICADOR '<''=' TK_LIT_INT
                    | TK_PR_FLOAT TK_IDENTIFICADOR '<''=' TK_LIT_FLOAT
                    | TK_PR_CHAR TK_IDENTIFICADOR '<''=' TK_LIT_CHAR;

variavel:           tipo-var list-identificador ';'
                    | variavel-declarada ';';

parametros:         parametros ',' --variavel
                    | --variavel
                    | ;

funcao:             tipo-var TK_IDENTIFICADOR '(' --parametros ')' bloco_comando

%%
