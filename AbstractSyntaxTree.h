#ifndef _ARVORE_H_
#define _ARVORE_H_

typedef struct AbstractSyntaxTree {
  char *label;
  double value;
  int number_of_children;
  struct AbstractSyntaxTree **children;
} ASTree;

ASTree *astree_new_node(const char *label, double value);

void astree_free(ASTree *tree);

void astree_add_child(ASTree *tree, ASTree *child);

void astree_print(ASTree *tree);

void astree_print_graphviz (ASTree *tree);

#endif