etapa = etapa1

.PHONY: compile_test test clean flex $(etapa)

$(etapa): flex
	gcc lex.yy.c main.c -o $(etapa)

flex: scanner.l
	flex scanner.l

clean:
	rm -rf $(etapa) lex.yy.c test.o

test: flex compile_test
	./test.o

compile_test:
	gcc lex.yy.c test.c -o test.o
