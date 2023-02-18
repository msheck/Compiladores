#INF01147 - Compiladores 2022/2
#Professor: 
#	-Lucas Schnorr
#
#Desenvolvido pelos alunos:
#	-Mateus Severgnini Heck 
#	-Vinicius Meirelles Pereira

etapa = 3

.PHONY: zip unzip compile_test test clean flex etapa$(etapa)

$(etapa): flex bison compile

debug: flex bison_debug compile

compile:
	gcc lex.yy.c AbstractSyntaxTree.c parser.tab.c main.c -o etapa$(etapa)

flex: scanner.l
	flex scanner.l

bison: parser.y
	bison -d parser.y

bison_debug:
	bison -d -v parser.y

clean:
	rm -rf entrega | rm -rf etapa$(etapa) lex.yy.c test.o *.tgz parser.tab.* *.output *.dot *.png

test: flex bison_debug compile_test
	./test.o

compile_test:
	gcc lex.yy.c AbstractSyntaxTree.c parser.tab.c test.c -o test.o

run_input: debug 
	./etapa$(etapa) < input

graph: run_input	
	dot saida.dot -Tpng -o grafo.png

valgrind: debug
	valgrind -s ./etapa$(etapa) < input --leak-check=full

zip: clean
	tar --exclude-vcs-ignores --exclude='.git*' --exclude='.vscode*' --exclude='input' -cvzf etapa$(etapa).tgz .

unzip:
	mkdir entrega | tar xf etapa$(etapa).tgz --directory entrega

table:
	gcc Tabela.c -o table_test