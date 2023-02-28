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
        ret->value = value;
        ret->number_of_children = 0;
        ret->children = NULL;
        ret->node_type = node_type;
    }
    return ret;
}

char* resolve_unary_expr(char operator, ASTree *expr, int node_type){
    int expr_intValue;
    float expr_floatValue;
    char expr_charValue;
    char* buffer= malloc(sizeof(char*)*(strlen(expr->value.value)+1));
    switch (node_type){
        case 1: //bool
        case 3: //int
            expr_intValue = atoi(expr->value.value);
            break;
        case 4: //float
            expr_floatValue = atof(expr->value.value);
            break;
        case 2: // char
            expr_charValue = expr->value.value[0];
            break;
        default:
            printf("\n\033[1;4;31mINVALID PARAMETER IN UNARY EXPRESSION IN LINE %d!\033[0;31m Couldn't convert %s to appropriate type!\033[0m\n", expr->value.line_number, expr->value.label);
            return NULL;
    }
    switch (operator)
    {
    case 33: //'!'
        if(expr->value.value == 0){ //false
            return strdup("1");
        }
        else{ //true
            return strdup("0");
        }
        printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Unary operator \"!\" couldn't complete the operation!\033[0m\n");
        return NULL;
    case 45: //'-'
        switch (node_type){
            case 1: //bool
                return resolve_unary_expr('!', expr, node_type);
            case 3: //int
                sprintf(buffer, "%d", -expr_intValue);
                return strdup(buffer);              
            case 4: //float
                sprintf(buffer, "%f", -expr_floatValue);
                return strdup(buffer);
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Unary operator \"-\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }        
    default:
        printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Unary operator unidentified!\033[0m\n");
        return NULL;
    }
}

char* resolve_binary_expr(ASTree *expr1, char* operator, ASTree *expr2, int node_type){
    int expr1_intValue, expr2_intValue;
    float expr1_floatValue, expr2_floatValue;
    char expr1_charValue, expr2_charValue;
    char* buffer;
    int value_i;
    float value_f;
    switch (node_type){
        case 1: //bool
        case 3: //int
            expr1_intValue = atoi(expr1->value.value);
            expr2_intValue = atoi(expr2->value.value);
            break;
        case 4: //float
            expr1_floatValue = atof(expr1->value.value);
            expr2_floatValue = atof(expr2->value.value);
            break;
        case 2: // char
            expr1_charValue = expr1->value.value[0];
            expr2_charValue = expr2->value.value[0];
            break;
        default:
            printf("\n\033[1;4;31mINVALID PARAMETER IN BINARY EXPRESSION!\033[0;31m Couldn't convert %s %s %s to appropriate type!\033[0m\n", expr1->value.label, operator, expr2->value.label);
            return NULL;
    }
    switch (operator[0]+operator[1])
    {
    case 37: //'%'
        switch (node_type){
            case 1:
            case 3: //int or bool
                buffer = malloc(sizeof(char*)*abs((strlen(expr2->value.value)+1)));
                value_i = expr1_intValue % expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;                                        
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"%%\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 47: //'/'
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*abs((strlen(expr1->value.value)-strlen(expr2->value.value))));
                value_i = expr1_intValue / expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*abs((strlen(expr1->value.value)-strlen(expr2->value.value))));
                value_f = expr1_floatValue / expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"//\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }        
    case 42: //'*'
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*abs((strlen(expr1->value.value)+strlen(expr2->value.value))));
                value_i = expr1_intValue * expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*abs((strlen(expr1->value.value)+strlen(expr2->value.value))));
                value_f = expr1_floatValue * expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"**\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 45: //'-'
        switch (node_type){
            case 1: //bool
            case 3: //int
                if(strlen(expr1->value.value)>strlen(expr2->value.value))
                    buffer= malloc(sizeof(char*)*(strlen(expr1->value.value)));
                else
                    buffer= malloc(sizeof(char*)*(strlen(expr2->value.value)));
                value_i = expr1_intValue - expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                if(strlen(expr1->value.value)>strlen(expr2->value.value))
                    buffer= malloc(sizeof(char*)*(strlen(expr1->value.value)));
                else
                    buffer= malloc(sizeof(char*)*(strlen(expr2->value.value)));
                value_f = expr1_floatValue - expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"-\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 43: //'+'
        switch (node_type){
            case 1: //bool
            case 3: //int
                if(strlen(expr1->value.value)>strlen(expr2->value.value))
                    buffer= malloc(sizeof(char*)*(strlen(expr1->value.value)+1));
                else
                    buffer= malloc(sizeof(char*)*(strlen(expr2->value.value)+1));
                value_i = expr1_intValue + expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                if(strlen(expr1->value.value)>strlen(expr2->value.value))
                    buffer= malloc(sizeof(char*)*(strlen(expr1->value.value)+1));
                else
                    buffer= malloc(sizeof(char*)*(strlen(expr2->value.value)+1));
                value_f = expr1_floatValue + expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"+\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 123: //'>='
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*2);
                value_i = expr1_intValue >= expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*2);
                value_f = expr1_floatValue >= expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \">=\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 121: //'<='
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*2);
                value_i = expr1_intValue <= expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*2);
                value_f = expr1_floatValue <= expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"<=\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 62: //'>'
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*2);
                value_i = expr1_intValue > expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*abs((strlen(expr1->value.value)-strlen(expr2->value.value))));
                value_f = expr1_floatValue > expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \">\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 60: //'<'
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*2);
                value_i = expr1_intValue < expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*2);
                value_f = expr1_floatValue < expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"<\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 94: //'!='
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*2);
                value_i = expr1_intValue != expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*2);
                value_f = expr1_floatValue != expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"!=\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 122: //'=='
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*2);
                value_i = expr1_intValue == expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*2);
                value_f = expr1_floatValue == expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"==\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 76: //"&&"
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*2);
                value_i = expr1_intValue && expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*2);
                value_f = expr1_floatValue && expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"&&\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    case 248: //"||"
        switch (node_type){
            case 1: //bool
            case 3: //int
                buffer= malloc(sizeof(char*)*2);
                value_i = expr1_intValue || expr2_intValue;
                sprintf(buffer, "%d", value_i);
                return buffer;
            case 4: //float
                buffer= malloc(sizeof(char*)*2);
                value_f = expr1_floatValue || expr2_floatValue;
                sprintf(buffer, "%f", value_f);
                return buffer;
            default:
                printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Binary operator \"||\" couldn't complete the operation!\033[0m\n");
                return NULL;
        }
    default:
        printf("\n\033[1;4;31mINVALID CONVERTION!\033[0;31m Binary operator unidentified!\033[0m\n");
        return NULL;
    }
}
//TODO: pegar o label como value se value for null, e pegar o value de fato depois
ASTree* ast_expr_node(ASTree *expr1, lexValue operator, ASTree *expr2){
    int node_type = expr2->node_type;
    char* expr_result;
    if(expr1->value.label != NULL){
        if(expr1->node_type != expr2->node_type){
            if(expr1->node_type != 2 && node_type != 2){
                if(((expr1->node_type == 1) && (expr2->node_type == 3))
                ||((expr1->node_type == 3) && (expr2->node_type == 1)))
                    node_type = 3;
                else 
                if(((expr1->node_type == 1) && (expr2->node_type == 4))
                ||((expr1->node_type == 4) && (expr2->node_type == 1)))
                    node_type = 4;
                else 
                if(((expr1->node_type == 3) && (expr2->node_type == 4))
                ||((expr1->node_type == 4) && (expr2->node_type == 3)))
                    node_type = 4;
            }        
            else if((expr1->node_type == 2) || (expr2->node_type == 2)){
                printf("\n\033[1;4;31mINVALID EXPRESSION ON %d!\033[0;31m Can't coerce char type with different types!\033[0m\n", operator.line_number);
                return NULL;
            }
            else{
                printf("\n\033[1;4;31mINVALID EXPRESSION ON %d!\033[0;31m Type was not clearly defined!\033[0m\n", operator.line_number);
                return NULL;
            }
        }
        expr_result = resolve_binary_expr(expr1, operator.label, expr2, node_type);
    }
    else
        expr_result = resolve_unary_expr(operator.label[0], expr2, node_type);
    ASTree* expr_node = ast_new_node(operator, node_type); //TODO: tirar um dos node_types //sera q da?
    expr_node->value.value = strdup(expr_result);
    return expr_node;
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
        free(tree->value.label);
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
    }
}

static void ast_print_label (FILE *foutput, ASTree *tree)
{
    if (tree != NULL){
        fprintf(foutput, "\t%ld [ label=\"%s\" ]\n", (long)tree, tree->value.label);
        printf("\t%p [ label=\"%s\" ]\n", tree, tree->value.label);
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
    if(node1->node_type == node2->node_type)
        return;
    if(node2->node_type == NODE_TYPE_CHAR) {
        if(node1->node_type == NODE_TYPE_INT){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para INT.\033[0m",
            node1->value.line_number);
            exit(ERR_CHAR_TO_INT);
        }
        if(node1->node_type == NODE_TYPE_FLOAT){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para FLOAT.\033[0m",
            node1->value.line_number);
            exit(ERR_CHAR_TO_FLOAT);
        }
        if(node1->node_type == NODE_TYPE_BOOL){
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de CHAR para BOOL.\033[0m",
            node1->value.line_number);
            exit(ERR_CHAR_TO_BOOL);
        }
        else {
            printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Nodo %s sem tipo atribuido.\033[0m",
            node1->value.line_number, node1->value.value);
            exit(-1);
        }
    }
    if(node2->node_type == NODE_TYPE_CHAR){
        if(node1->node_type == NODE_TYPE_BOOL)
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de BOOL para CHAR.\033[0m",
            node1->value.line_number);
        if(node1->node_type == NODE_TYPE_FLOAT)
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de FLOAT para CHAR.\033[0m",
            node1->value.line_number);
        if(node1->node_type == NODE_TYPE_INT)
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Conversao implicita de INT para CHAR.\033[0m",
            node1->value.line_number);
        else
        printf("\n\033[1;4;31mERRO na linha %d:\033[0;31m Nodo %s sem tipo atribuido.\033[0m",
            node1->value.line_number, node1->value.value);
        exit(ERR_X_TO_CHAR);
  }
}