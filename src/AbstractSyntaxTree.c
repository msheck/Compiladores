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
        ret->temp = NULL;
        ret->code = NULL;
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
        // printf("\t%p [ label=\"%s\" ]\n", tree, tree->data.label);
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
            // printf("\t%p -> %p;\n", tree, tree->children[i]);
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
        // printf("\ndigraph {\n");
        ast_print_children(foutput, tree);
        ast_print_label(foutput, tree);
        fprintf(foutput, "}\n");
        // printf("}\n");
        fclose(foutput);
    }
}

void ast_check_node_type(ASTree* node1, int type) {
    // Caso checagem de undeclared seja permitida, ocorrerá segfault quando ast_check_type tentar acessar a label de node2
    if (type == NODE_TYPE_UNDECLARED) {
        printf("\n\033[1;4;31mUso incorreto da funcao ast_check_type (AbstractSyntaxTree.c: %d)\033[0m", __LINE__);
        exit(-1);
    }
    lexValue val;
    val.line_number = node1->data.line_number;
    val.label = NULL;
    val.value = NULL;
    ASTree* node2 = ast_new_node(val, type);
    ast_check_type(node1, node2);
    ast_free(node2);
}

void ast_check_type_node(int type, ASTree *node2) {
    // Caso checagem de undeclared seja permitida, ocorrerá segfault quando ast_check_type tentar acessar a label de node1
    if (type == NODE_TYPE_UNDECLARED) {
        printf("\n\033[1;4;31mUso incorreto da funcao ast_check_type (AbstractSyntaxTree.c: %d)\033[0m", __LINE__);
        exit(-1);
    }
    lexValue val;
    val.line_number = node2->data.line_number;
    val.label = NULL;
    val.value = NULL;
    ASTree* node1 = ast_new_node(val, type);
    ast_check_type(node1, node2);
    ast_free(node1);
}

void ast_check_type(ASTree* node1, ASTree* node2) {
    if (node1 == NULL || node2 == NULL)
        return;
    /*printf("\nChecking!");
    printf("\nline: %d", node1->data.line_number);
    printf("\nline: %d", node2->data.line_number);
    printf("\nNode1:");
    printf("\n  node_type: %d", node1->node_type);
    printf("\n  data_type: %d", node1->data.type);
    printf("\n  data_label: %s", node1->data.label);
    printf("\n  data_value: %s", node1->data.value);
    printf("\nNode2:");
    printf("\n  node_type: %d", node2->node_type);
    printf("\n  data_type: %d", node2->data.type);
    printf("\n  data_label: %s", node2->data.label);
    printf("\n  data_value: %s", node2->data.value);*/
    if(node1->node_type == node2->node_type)
        return;
    if(node2->node_type == NODE_TYPE_CHAR) {
        if(node1->node_type == NODE_TYPE_INT){
            emit_error(ERR_CHAR_TO_INT, node1->data.line_number, NULL, NULL);
        }
        if(node1->node_type == NODE_TYPE_FLOAT){
            emit_error(ERR_CHAR_TO_FLOAT, node1->data.line_number, NULL, NULL);
        }
        if(node1->node_type == NODE_TYPE_BOOL){
            emit_error(ERR_CHAR_TO_BOOL, node1->data.line_number, NULL, NULL);
        }
        if(node1->node_type <= NODE_TYPE_UNDECLARED || node1->node_type > NODE_TYPE_FLOAT){
            emit_error(ERR_UNDECLARED, node1->data.line_number, node1->data.label, NULL);
        }
    }
    if(node1->node_type == NODE_TYPE_CHAR){
        if(node2->node_type == NODE_TYPE_BOOL){
            emit_error(ERR_X_TO_CHAR, node2->data.line_number, NULL, "BOOL");
        }
        if(node2->node_type == NODE_TYPE_FLOAT){
            emit_error(ERR_X_TO_CHAR, node2->data.line_number, NULL, "FLOAT");
        }
        if(node2->node_type == NODE_TYPE_INT){
            emit_error(ERR_X_TO_CHAR, node2->data.line_number, NULL, "INT");
        }
        if(node1->node_type <= NODE_TYPE_UNDECLARED || node1->node_type > NODE_TYPE_FLOAT){
            emit_error(ERR_UNDECLARED, node2->data.line_number, node2->data.label, NULL);
        }
  }
}

void ast_check_not_char(ASTree *node, int expected_type) {
    if (expected_type == NODE_TYPE_CHAR)
        expected_type = NODE_TYPE_INT;
    if(node->node_type == NODE_TYPE_CHAR){
        switch (expected_type) {
            case NODE_TYPE_BOOL:
                emit_error(ERR_CHAR_TO_BOOL, node->data.line_number, node->data.label, NULL);
                break;
            case NODE_TYPE_FLOAT:
                emit_error(ERR_CHAR_TO_FLOAT, node->data.line_number, node->data.label, NULL);
                break;
            case NODE_TYPE_INT:
                emit_error(ERR_CHAR_TO_INT, node->data.line_number, node->data.label, NULL);
                break;
            default:
                break;
        }
    }
}
