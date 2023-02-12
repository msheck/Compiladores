# Trabalho de Compiladores - INF01147 - UFRGS
_Desenvolvido por Mateus Severgnini Heck e Vinicius Meirelles Pereira_

[Repositório do Projeto](https://github.com/msheck/Compiladores)

## Etapa 1 - Análise Léxica

Foi desenvolvido o código dos arquivos _**scanner.l**_ e _**Makefile**_, os arquivos _**main.c**_ e _**token.h**_ foram fornecidos pelo professor e não foram modificados.

Adicionalmente, foi acrescentado o arquivo _**test.c**_ que realiza diversos testes para verificar o funcionamento do projeto. Ele pode ser executado pela chamada:
```
make test
```


## Etapa 2 - Análise Sintática

O arquivo _**token.h**_ foi deletado e substituido pelo _**parser.y**_. Neste arquivo, também foram escritas as regras de linguagem a serem usadas pelo _**bison**_. Também foi modidicado o _**scanner.l**_ para se adequar as regras declaradas.

O arquivo _**test.c**_ foi modificado para refletir as alterações da etapa e auxiliar no desenvolvimento.


## Etapa 3 - Árvore Sintática Abstrata

Adapatando o código fornecido na aula de laboratório, desenvolvemos os arquivos _**AbstractSyntaxTree.h**_, onde temos as estruturas de árvore e valor léxico, juntamente com a declaração das funções, e _**AbstractSyntaxTree.c**_, onde são implementadas as funções para a construção da árvore sintática.

Modificamos o arquivo _**parser.y**_ e _**scanner.l**_ para que contenham blocos de código _c_ onde manipulamos dados de cada um dos nodos. 

Após o _make_ pode-se executar _./etapa3_, escrever um código seguindo as regras da gramática e acionar _Ctrl+D_ para finalizar a execução.Finalmente a árvore é exportata para _**saida.dot**_ e liberada da memória.

#### _Geração de grafos:_
Com o resultado da execução correta, pode-se gerar um grafo que representa a árvore através de:
```
dot saida.dot -Tpng -o grafo.png
```
Ou, alternativamente, pode-se utilizar o comando implementado no _**Makefile**_ que irá executar o programa utilizando o arquivo _**input**_ como entrada:
```
make graph
```

#### _Controle de memória:_
Para checar a memória utilizada por uma entrada válida utilizamos o comando:
```
valgrind ./etapa$(etapa) < input
```
Ou, de forma similar ao grafo, usando a implementação no _**Makefile**_:
```
make valgrind
```

---

## Makefile commands

```
make
make debug
make clean
make test
make run_input
make graph
make valgrind
make zip
make unzip
```