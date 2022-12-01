.PHONY: compile_test test clean flex etapa1

etapa1: flex
	gcc lex.yy.c main.c -o etapa1

flex: scanner.l
	flex scanner.l

clean:
	rm -rf etapa1 lex.yy.c test.o

test: flex compile_test
	./test.o

compile_test:
	gcc lex.yy.c test.c -o test.o
