%{
#include "tokens.h"
int lineNumber = 1;
%}

spec_char [\,\;\:\(\)\{\}\+\-\*\/\%\^\<\>\=\!]

let [a-zA-Z]
dig [0-9]
sig \+|\-

ident {let}+
lit_i {sig}?{dig}+
lit_f_regular ({lit_i}\.{dig}+)
lit_f_scientific {lit_f_regular}E{lit_i}
lit_f {lit_f_regular}|{lit_f_scientific}
lit_b (false)|(true)
lit_c \'.\'|\'\'

%x B_COMM
line_comm \/\/.*
ignore [" ""\r"]|{line_comm}

%%

{spec_char}  { return yytext[0]; }
int          { return TK_PR_INT; }
float        { return TK_PR_FLOAT; }
bool         { return TK_PR_BOOL; }
char         { return TK_PR_CHAR; }
if           { return TK_PR_IF; }
then         { return TK_PR_THEN; }
else         { return TK_PR_ELSE; }
while        { return TK_PR_WHILE; } 
input        { return TK_PR_INPUT; }
output       { return TK_PR_OUTPUT; }
return       { return TK_PR_RETURN; }
for          { return TK_PR_FOR; }
"<="         { return TK_OC_LE; }
">="         { return TK_OC_GE; }
"=="         { return TK_OC_EQ; }
"!="         { return TK_OC_NE; }
"&&"         { return TK_OC_AND; }
"||"         { return TK_OC_OR; }
{lit_i}      { return TK_LIT_INT; }
{lit_f}      { return TK_LIT_FLOAT; }
{lit_b}      { return TK_LIT_TRUE; }
{lit_c}      { return TK_LIT_CHAR; }
{ident}      { return TK_IDENTIFICADOR; }

"/*"         { BEGIN(B_COMM); }
<B_COMM>"*/" { BEGIN(INITIAL); }
<B_COMM>\n   { ++lineNumber; }
<B_COMM>.    { }
{ignore}     { }

"\n"         { ++lineNumber; }

.            { return TK_ERRO; }


%%
int yywrap() {
    return 1;
}

int get_line_number() {
    return lineNumber;
}