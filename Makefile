#INF01147 - Compiladores 2022/2
#Professor: 
#	-Lucas Schnorr
#
#Desenvolvido pelos alunos:
#	-Mateus Severgnini Heck 
#	-Vinicius Meirelles Pereira

ETAPA = 5
DATA = 500
STACK = 1024
SOURCES := lex.yy.c parser.tab.c src/AbstractSyntaxTree.c src/Errors.c src/CodeGen.c src/OpList.c src/ASTExpressions.c src/IntList.c src/Content.c src/ContentList.c src/Table.c src/TablePrint.c src/Misc.c

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
	rm -rf entrega | rm -rf etapa* debug table_test lex.yy.c test.o *.tgz parser.tab.* *output* *.dot *.png

test: debug flex bison_debug compile_test
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
	valgrind -s --leak-check=full --show-leak-kinds=all ./etapa$(ETAPA) < input

iloc_output: debug
	./etapa$(ETAPA) <input >output.iloc

iloc: iloc_output
	./ilocsim.py -x -t -s --data $(DATA) --stack $(STACK) output.iloc

zip: clean
	tar --exclude-vcs-ignores --exclude='.git*' --exclude='.vscode*' --exclude='input' --exclude='aux' -cvzf etapa$(ETAPA).tgz .

unzip:
	mkdir entrega | tar xf etapa$(ETAPA).tgz --directory entrega