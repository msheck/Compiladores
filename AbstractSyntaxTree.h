#ifndef _ARVORE_H_
#define _ARVORE_H_

#define TYPE_SPEC_CHAR 0
#define TYPE_KEY_WORD 1
#define TYPE_COMP_OP 2
#define TYPE_IDENT 3
#define TYPE_LIT 4

typedef struct lexical_value{
  int line_number;
  int type;
  char* value;
} lexValue;

typedef struct AbstractSyntaxTree {
  lexValue value;
  int number_of_children;
  struct AbstractSyntaxTree **children;
} ASTree;

ASTree *ast_new_node(lexValue value);

void ast_free(ASTree *tree);

void ast_add_child(ASTree *tree, ASTree *child);

void ast_print(ASTree *tree);

void ast_print_graphviz (ASTree *tree);

ASTree *ast_get_leaf(ASTree *root);


#endif