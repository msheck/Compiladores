/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Adaptado pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
A partir do código fornecido pelo professor.
*/

#include "AbstractSyntaxTree.h"
#define ARQUIVO_SAIDA "saida.dot"

ASTree* ast_new_node(lexValue value, int node_type)
{
    ASTree *ret = NULL;
    ret = calloc(1, sizeof(ASTree));
    if (ret != NULL){
        ret->data = value;
        ret->number_of_children = 0;
        ret->children = NULL;
        ret->node_type = node_type;
    }
    return ret;
}

ASTree* ast_get_node(ASTree *tree){
    if ((tree != NULL) && (tree->number_of_children > 2))
        return ast_get_node(tree->children[tree->number_of_children-1]);
    return tree;
}

void ast_free(ASTree *tree)
{
    if (tree != NULL){
        int i;
        for (i = 0; i < tree->number_of_children; i++){
            ast_free(tree->children[i]);
        }
        free(tree->children);
        free(tree->data.label);
        free(tree->data.value);
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
    }
}

static void ast_print_label (FILE *foutput, ASTree *tree)
{
    if (tree != NULL){
        fprintf(foutput, "\t%ld [ label=\"%s\" ]\n", (long)tree, tree->data.label);
        printf("\t%p [ label=\"%s\" ]\n", tree, tree->data.label);
        if (tree->number_of_children != 0){
            for (int i = 0; i < tree->number_of_children; i++){                
                ast_print_label(foutput, tree->children[i]);
            }
        }
    }
}

static void ast_print_children (FILE *foutput, ASTree *tree)
{
    if ((tree != NULL) && (tree->number_of_children != 0)){
        for (int i = 0; i < tree->number_of_children; i++){
            fprintf(foutput, "\t%ld -> %ld;\n", (long)tree, (long)tree->children[i]);
            printf("\t%p -> %p;\n", tree, tree->children[i]);
            ast_print_children(foutput, tree->children[i]);
        }
    }
}

void ast_print(ASTree *tree)
{
    if(tree != NULL){
        FILE *foutput = fopen(ARQUIVO_SAIDA, "w+");
        if(foutput == NULL){
            printf("Erro: %s não pude abrir o arquivo [%s] para escrita.\n", __FUNCTION__, ARQUIVO_SAIDA);
        }
        fprintf(foutput, "digraph {\n");
        printf("\ndigraph {\n");
        ast_print_children(foutput, tree);
        ast_print_label(foutput, tree);
        fprintf(foutput, "}\n");
        printf("}\n");
        fclose(foutput);
    }
}

void ast_check_type(ASTree* node1, ASTree* node2) {
    // printf("\nChecking!");
    // printf("\nNode1:");
    // printf("\n  node_type: %d", node1->node_type);
    // printf("\n  data_type: %d", node1->data.type);
    // printf("\n  data_label: %s", node1->data.label);
    // printf("\n  data_value: %s", node1->data.value);
    // printf("\nNode2:");
    // printf("\n  node_type: %d", node2->node_type);
    // printf("\n  data_type: %d", node2->data.type);
    // printf("\n  data_label: %s", node2->data.label);
    // printf("\n  data_value: %s", node2->data.value);
    if(node1->node_type == node2->node_type)
        return;
    if(node2->node_type == NODE_TYPE_CHAR) {
        if(node1->node_type == NODE_TYPE_INT){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para INT.\033[0m",
            node1->data.line_number);
            exit(ERR_CHAR_TO_INT);
        }
        if(node1->node_type == NODE_TYPE_FLOAT){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para FLOAT.\033[0m",
            node1->data.line_number);
            exit(ERR_CHAR_TO_FLOAT);
        }
        if(node1->node_type == NODE_TYPE_BOOL){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para BOOL.\033[0m",
            node1->data.line_number);
            exit(ERR_CHAR_TO_BOOL);
        }
        if(node1->node_type <= NODE_TYPE_UNDECLARED || node1->node_type > NODE_TYPE_FLOAT){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Nodo %s sem tipo atribuido.\033[0m",
            node1->data.line_number, node1->data.value);
            exit(-1);
        }
    }
    if(node1->node_type == NODE_TYPE_CHAR){
        if(node2->node_type == NODE_TYPE_BOOL){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de BOOL para CHAR.\033[0m",
            node2->data.line_number);
            exit(ERR_X_TO_CHAR);
        }
        if(node2->node_type == NODE_TYPE_FLOAT){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de FLOAT para CHAR.\033[0m",
            node2->data.line_number);
            exit(ERR_X_TO_CHAR);
        }
        if(node2->node_type == NODE_TYPE_INT){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de INT para CHAR.\033[0m",
            node2->data.line_number);
            exit(ERR_X_TO_CHAR);
        }
        if(node1->node_type <= NODE_TYPE_UNDECLARED || node1->node_type > NODE_TYPE_FLOAT){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Nodo %s sem tipo atribuido.\033[0m",
            node2->data.line_number, node2->data.value);
            exit(ERR_X_TO_CHAR);
        }
  }
}