/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "ASTExpressions.h"

ASTree* resolve_unary_expr(ASTree* operator_node, ASTree *expr, int node_type){
    switch (operator_node->data.label[0])
    {
    case 33: //'!'

    case 45: //'-'

    default:
        printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Unary operator unidentified!\033[0m\n");
        exit(ERR_INVALID_EXPR);
    }
}

ASTree* resolve_binary_expr(ASTree* expr1, ASTree* operator_node, ASTree* expr2, int node_type){
    operator_node->temp = get_temp();
    switch (operator_node->data.label[0]+operator_node->data.label[1]){
        case 37: //'%'
            break;
        case 47: //'/'
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_DIV, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;     
        case 42: //'*'
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_MULT, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 45: //'-'
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_SUB, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 43: //'+'
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_ADD, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 123: //'>='
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_GE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 121: //'<='
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_LE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 62: //'>'
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_GT, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 60: //'<'
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_LT, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 94: //'!='
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_NE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 122: //'=='
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_EQ, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 76: //"&&"
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_AND, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 248: //"||"
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_OR, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        default:
            printf("\n\033[1;4;31mINVALID CONVERTION!\033[0;31m Binary operator unidentified!\033[0m\n");
            exit(ERR_INVALID_EXPR);
    }
    operator_node->code = opList_concatLeft(operator_node->code, expr2->code);
    operator_node->code = opList_concatLeft(operator_node->code, expr1->code);
    return operator_node;
}

ASTree* ast_expr_node(ASTree *expr1, lexValue operator, ASTree *expr2){
    int node_type = expr2->node_type;
    ASTree* operator_node = ast_new_node(operator, node_type);
    if(expr1->data.label != NULL){
        if(expr1->node_type != expr2->node_type){
            if(expr1->node_type != 2 && node_type != 2){
                if(((expr1->node_type == 1) && (expr2->node_type == 3))
                || ((expr1->node_type == 3) && (expr2->node_type == 1)))
                    node_type = 3;
                else 
                if(((expr1->node_type == 1) && (expr2->node_type == 4))
                || ((expr1->node_type == 4) && (expr2->node_type == 1)))
                    node_type = 4;
                else 
                if(((expr1->node_type == 3) && (expr2->node_type == 4))
                || ((expr1->node_type == 4) && (expr2->node_type == 3)))
                    node_type = 4;
            }        
            else if((expr1->node_type == 2) || (expr2->node_type == 2)){
                printf("\n\033[1;4;31mINVALID EXPRESSION ON %d!\033[0;31m Can't coerce char type with different types!\033[0m\n", operator.line_number);
                exit(ERR_INVALID_EXPR);
            }
            else{
                printf("\n\033[1;4;31mINVALID EXPRESSION ON %d!\033[0;31m Type was not clearly defined!\033[0m\n", operator.line_number);
                exit(ERR_INVALID_EXPR);
            }
        }
        operator_node = resolve_binary_expr(expr1, operator_node, expr2, node_type);
    }
    else
        operator_node = resolve_unary_expr(operator_node, expr2, node_type);
    return operator_node;
}