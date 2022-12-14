etapa = 2

.PHONY: zip unzip compile_test test clean flex etapa$(etapa)

$(etapa): flex bison
	gcc lex.yy.c parser.tab.c main.c -o etapa$(etapa)

debug: flex bison_debug
	gcc lex.yy.c parser.tab.c main.c -o etapa$(etapa)

flex: scanner.l
	flex scanner.l

bison: parser.y
	bison -d parser.y

bison_debug:
	bison -d -v parser.y

clean:
	rm -rf entrega | rm -rf etapa$(etapa) lex.yy.c test.o *.tgz parser.tab.* *.output

zip: clean
	tar --exclude-vcs-ignores --exclude='.git*' -cvzf etapa$(etapa).tgz .

unzip:
	mkdir entrega | tar xf etapa$(etapa).tgz --directory entrega