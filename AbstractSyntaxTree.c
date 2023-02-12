#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "AbstractSyntaxTree.h"
#define ARQUIVO_SAIDA "saida.dot"

ASTree *ast_new_node(lexValue value)
{
  ASTree *ret = NULL;
  ret = calloc(1, sizeof(ASTree));
  if (ret != NULL){
    ret->value = value;
    ret->number_of_children = 0;
    ret->children = NULL;
  }
  return ret;
}

void ast_free(ASTree *tree)
{
  if (tree != NULL){
    int i;
    for (i = 0; i < tree->number_of_children; i++){
      ast_free(tree->children[i]);
    }
    free(tree->children);
    free(tree->value.value);
    free(tree);
  }
}

void ast_add_child(ASTree *tree, ASTree *child)
{
  if (child==NULL){}
  else if (tree != NULL){
    tree->number_of_children++;
    tree->children = realloc(tree->children, tree->number_of_children * sizeof(ASTree*));
    tree->children[tree->number_of_children-1] = child;
  }else{
    printf("Erro: %s recebeu parâmetro tree = %p / %p.\n", __FUNCTION__, tree, child);
  }
}

static void _ast_print (FILE *foutput, ASTree *tree, int profundidade)
{
  int i;
  if (tree != NULL){
    fprintf(foutput, "%d%*s: Nó '%s' tem %d filhos:\n", profundidade, profundidade*2, "", tree->value.value, tree->number_of_children);
    for (i = 0; i < tree->number_of_children; i++){
      _ast_print(foutput, tree->children[i], profundidade+1);
    }
  }else{
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void ast_print(ASTree *tree)
{
  FILE *foutput = stderr;
  if (tree != NULL){
    _ast_print(foutput, tree, 0);
  }else{
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

static void _ast_print_graphviz (FILE *foutput, ASTree *tree)
{
  int i;
  if (tree != NULL){
    fprintf(foutput, "  %ld [ label=\"%s\" ]\n", (long)tree, tree->value.value);
    for (i = 0; i < tree->number_of_children; i++){
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _ast_print_graphviz(foutput, tree->children[i]);
    }
  }
}

void ast_print_graphviz(ASTree *tree)
{
  FILE *foutput = fopen(ARQUIVO_SAIDA, "w+");
  if(foutput == NULL){
    printf("Erro: %s não pude abrir o arquivo [%s] para escrita.\n", __FUNCTION__, ARQUIVO_SAIDA);
  }
  fprintf(foutput, "digraph grafo {\n");
  _ast_print_graphviz(foutput, tree);
  fprintf(foutput, "}\n");
  fclose(foutput);
}

ASTree *ast_get_leaf(ASTree *root){
  ASTree *leaf = root;
  while(leaf->number_of_children > 0){
    leaf = leaf->children[0];
  }
  return leaf;
}
