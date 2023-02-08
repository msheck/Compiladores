#INF01147 - Compiladores 2022/2
#Professor: 
#	-Lucas Schnorr
#
#Desenvolvido pelos alunos:
#	-Mateus Severgnini Heck 
#	-Vinicius Meirelles Pereira

etapa = 3

.PHONY: zip unzip compile_test test clean flex etapa$(etapa)

$(etapa): flex bison
	gcc lex.yy.c AbstractSyntaxTree.c parser.tab.c main.c -o etapa$(etapa)

debug: flex bison_debug
	gcc lex.yy.c AbstractSyntaxTree.c parser.tab.c main.c -o etapa$(etapa)

flex: scanner.l
	flex scanner.l

bison: parser.y
	bison -d parser.y

bison_debug:
	bison -d -v parser.y

clean:
	rm -rf entrega | rm -rf etapa$(etapa) lex.yy.c test.o *.tgz parser.tab.* *.output

test: flex bison compile_test
	./test.o

compile_test:
	gcc lex.yy.c parser.tab.c test.c -o test.o

zip: clean
	tar --exclude-vcs-ignores --exclude='.git*' --exclude='.vscode*' --exclude='input' -cvzf etapa$(etapa).tgz .

unzip:
	mkdir entrega | tar xf etapa$(etapa).tgz --directory entrega