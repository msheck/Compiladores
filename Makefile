#INF01147 - Compiladores 2022/2
#Professor: 
#	-Lucas Schnorr
#
#Desenvolvido pelos alunos:
#	-Mateus Severgnini Heck 
#	-Vinicius Meirelles Pereira

ETAPA = 4
SOURCES := lex.yy.c parser.tab.c src/AbstractSyntaxTree.c src/Errors.c src/ASTExpressions.c src/IntList.c src/Content.c src/ContentList.c src/Table.c src/TablePrint.c

.PHONY: zip unzip compile_test test clean flex etapa$(ETAPA)

$(ETAPA): flex bison compile

debug: flex bison_debug compile

compile:
	gcc $(SOURCES) main.c -o etapa$(ETAPA)

flex: scanner.l
	flex scanner.l

bison: parser.y
	bison -d parser.y

bison_debug:
	bison -d -v parser.y

clean:
	rm -rf entrega | rm -rf etapa* debug table_test lex.yy.c test.o *.tgz parser.tab.* *.output *.dot *.png

test: flex bison_debug compile_test
	./test.o

compile_test:
	gcc $(SOURCES) test.c -o test.o

run_input: debug 
	./etapa$(ETAPA) < input

graph: run_input	
	dot saida.dot -Tpng -o grafo.png

valgrind: debug
	valgrind -s ./etapa$(ETAPA) < input

valgrind_details: debug
	valgrind -s --leak-check=full ./etapa$(ETAPA) < input

zip: clean
	tar --exclude-vcs-ignores --exclude='.git*' --exclude='.vscode*' --exclude='input' -cvzf etapa$(ETAPA).tgz .

unzip:
	mkdir entrega | tar xf etapa$(ETAPA).tgz --directory entrega

table:
	gcc $(SOURCES) main.c -o table_test