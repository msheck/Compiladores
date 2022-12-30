# Trabalho de Compiladores - INF01147 - UFRGS
_Desenvolvido por Mateus Severgnini Heck e Vinicius Meirelles Pereira_

[Repositório do Projeto](https://github.com/msheck/Compiladores)

## Etapa 1 - Análise Léxica

Foi desenvolvido o código dos arquivos _**scanner.l**_ e _**Makefile**_, os arquivos _**main.c**_ e _**token.h**_ foram fornecidos pelo professor e não foram modificados.

Adicionalmente, foi acrescentado o arquivo _**test.c**_ que realiza diversos testes para verificar o funcionamento do projeto. Ele pode ser executado pela chamada 'make test'.

## Etapa 2 - Análise Sintática

O arquivo _**token.h**_ foi deletado e substituido pelo _**parser.y**_. Neste arquivo, também foram escritas as regras de linguagem a serem usadas pelo _**bison**_. Também foi modidicado o _**scanner.l**_ para se adequar as regras declaradas.
