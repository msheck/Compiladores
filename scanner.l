/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

%{
#include "parser.tab.h"

void set_lexValue(int type){
	lexValue lv; 
	lv.value = strdup(yytext);
	lv.line_number = yylineno;
	lv.type = type; 
	yylval.valor_lexico = lv;
}
lexValue get_lexValue(char* value, int line_number, int type){
	lexValue lv; 
	lv.value = strdup(value);
	lv.line_number = line_number;
	lv.type = type; 
	return lv;
}

extern void* arvore;
%}

%option noyywrap

%option yylineno

spec_char 			[\,\;\:\(\)\{\}\]]
spec_char_val		[\[\+\-\*\/\%\^\<\>\=\!]

let 				[a-zA-Z]
dig 				[0-9]

ident 				{let}+
lit_i 				{dig}+
lit_f_regular 		({lit_i}\.{dig}+)
lit_f_scientific 	{lit_f_regular}(E|e){lit_i}
lit_f 				{lit_f_regular}|{lit_f_scientific}
lit_c 				\'.\'|\'\'

%x B_COMM	
line_comm 			\/\/.*
espaco				[\t\r ]
ignore 				{espaco}|{line_comm}

%%

{spec_char}  		{ return yytext[0]; }
{spec_char_val}		{ set_lexValue(TYPE_SPEC_CHAR); return yytext[0]; }

int          		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_INT; }
float        		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_FLOAT; }
bool         		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_BOOL; }
char         		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_CHAR; }
if           		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_IF; }
then         		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_THEN; }
else         		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_ELSE; }
while        		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_WHILE; } 
input        		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_INPUT; }
output       		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_OUTPUT; }
return       		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_RETURN; }
for          		{ set_lexValue(TYPE_KEY_WORD); return TK_PR_FOR; }
"<="         		{ set_lexValue(TYPE_COMP_OP); return TK_OC_LE; }
">="         		{ set_lexValue(TYPE_COMP_OP); return TK_OC_GE; }
"=="         		{ set_lexValue(TYPE_COMP_OP); return TK_OC_EQ; }
"!="         		{ set_lexValue(TYPE_COMP_OP); return TK_OC_NE; }
"&&"         		{ set_lexValue(TYPE_COMP_OP); return TK_OC_AND; }
"||"         		{ set_lexValue(TYPE_COMP_OP); return TK_OC_OR; }
{lit_i}      		{ set_lexValue(TYPE_LIT); return TK_LIT_INT; }
{lit_f}      		{ set_lexValue(TYPE_LIT); return TK_LIT_FLOAT; }
true         		{ set_lexValue(TYPE_LIT); return TK_LIT_TRUE; }
false        		{ set_lexValue(TYPE_LIT); return TK_LIT_FALSE; }
{lit_c}      		{ set_lexValue(TYPE_LIT); return TK_LIT_CHAR; }					  
{ident}      		{ set_lexValue(TYPE_IDENT); return TK_IDENTIFICADOR; }

"/*"         		{ BEGIN(B_COMM); }
<B_COMM>"*/" 		{ BEGIN(INITIAL); }
<B_COMM>\n   		{ }
<B_COMM>.    		{ }

{ignore}     		{ }

"\n"         		{ }

.            		{ return TK_ERRO; }

%%
int error_message_enabled = 1;
int get_line_number() {
	return yylineno;
}
void yyerror (char const *error_message) {
	if (error_message_enabled)
		fprintf (stderr, "%i: %s\n",get_line_number(), error_message);
}
void exporta(void* arvore){
	ast_print_graphviz(arvore);
}
void libera(void* arvore){
	ast_free(arvore);
}
