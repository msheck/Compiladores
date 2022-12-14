etapa = 1

.PHONY: entrega compile_test test clean flex etapa$(etapa)

$(etapa): flex
	gcc lex.yy.c main.c -o etapa$(etapa)

flex: scanner.l
	flex scanner.l

clean:
	rm -rf etapa$(etapa) lex.yy.c test.o *.tgz

test: flex compile_test
	./test.o

compile_test:
	gcc lex.yy.c test.c -o test.o

entrega: clean
	tar cvzf etapa$(etapa).tgz .