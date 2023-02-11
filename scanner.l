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

void get_node_info(int type){
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
%}

%option noyywrap

%option yylineno

spec_char 			[\,\;\:\(\)\{\}\[\]\+\-\*\/\%\^\<\>\=\!]

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

int          		{ get_node_info(TYPE_KEY_WORD); return TK_PR_INT; }
float        		{ get_node_info(TYPE_KEY_WORD); return TK_PR_FLOAT; }
bool         		{ get_node_info(TYPE_KEY_WORD); return TK_PR_BOOL; }
char         		{ get_node_info(TYPE_KEY_WORD); return TK_PR_CHAR; }
if           		{ get_node_info(TYPE_KEY_WORD); return TK_PR_IF; }
then         		{ get_node_info(TYPE_KEY_WORD); return TK_PR_THEN; }
else         		{ get_node_info(TYPE_KEY_WORD); return TK_PR_ELSE; }
while        		{ get_node_info(TYPE_KEY_WORD); return TK_PR_WHILE; } 
input        		{ get_node_info(TYPE_KEY_WORD); return TK_PR_INPUT; }
output       		{ get_node_info(TYPE_KEY_WORD); return TK_PR_OUTPUT; }
return       		{ get_node_info(TYPE_KEY_WORD); return TK_PR_RETURN; }
for          		{ get_node_info(TYPE_KEY_WORD); return TK_PR_FOR; }
"<="         		{ get_node_info(TYPE_COMP_OP); return TK_OC_LE; }
">="         		{ get_node_info(TYPE_COMP_OP); return TK_OC_GE; }
"=="         		{ get_node_info(TYPE_COMP_OP); return TK_OC_EQ; }
"!="         		{ get_node_info(TYPE_COMP_OP); return TK_OC_NE; }
"&&"         		{ get_node_info(TYPE_COMP_OP); return TK_OC_AND; }
"||"         		{ get_node_info(TYPE_COMP_OP); return TK_OC_OR; }
{lit_i}      		{ get_node_info(TYPE_LIT); return TK_LIT_INT; }
{lit_f}      		{ get_node_info(TYPE_LIT); return TK_LIT_FLOAT; }
true         		{ get_node_info(TYPE_LIT); return TK_LIT_TRUE; }
false        		{ get_node_info(TYPE_LIT); return TK_LIT_FALSE; }
{lit_c}      		{ get_node_info(TYPE_LIT); return TK_LIT_CHAR; }					  
{ident}      		{ get_node_info(TYPE_IDENT); return TK_IDENTIFICADOR; }

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
}
void libera(void* arvore){
}
