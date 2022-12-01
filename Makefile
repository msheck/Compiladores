etapa1: lex.yy.c
	gcc lex.yy.c main.c -o etapa1


lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm -rf etapa1 lex.yy.c
