/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "ASTExpressions.h"

OpList* switch_logic_labels(OpList* code){
    if(code != NULL) {
        OpList* current = code;
        char* buffer;
        while(current != NULL) {
            if((current->value->operation == OP_LABEL)
            &&(strcmp(current->value->arg0,"label_true") == 0)){
                buffer = current->next->value->arg0;
                current->next->value->arg0 = current->next->next->next->next->value->arg0;
                current->next->next->next->next->value->arg0 = buffer;
            }
            current = current->next;
        }
    }
}

void remove_logic_labels(OpList* code){ 
    if(code != NULL) {
        OpList* current = code;
        while(current->next != NULL) {
            if((current->next->value->operation == OP_CBR)
            &&(strcmp(current->next->value->arg2,"label_true") == 0)){
                opList_free(current->next);
                current->next = NULL;
                break;
            }
            current = current->next;
        }
    }
}

OpList* set_logic_labels(OpList* code, char* temp){
    code = opList_pushLeft(code, op_new(OP_LABEL, "label_next_command", NULL, NULL, NULL));
    code = opList_pushLeft(code, op_new(OP_LOADI, "0", NULL, temp, NULL));
    code = opList_pushLeft(code, op_new(OP_LABEL, "label_false", NULL, NULL, NULL));
    code = opList_pushLeft(code, op_new(OP_JUMPI, NULL, NULL, "label_next_command", NULL));
    code = opList_pushLeft(code, op_new(OP_LOADI, "1", NULL, temp, NULL));
    code = opList_pushLeft(code, op_new(OP_LABEL, "label_true", NULL, NULL, NULL));
    code = opList_pushLeft(code, op_new(OP_CBR, temp, NULL, "label_true", "label_false"));     
    return code;
}

ASTree* resolve_unary_expr(ASTree* operator_node, ASTree *expr){
    operator_node->temp = get_temp();
    switch (operator_node->data.label[0]) {
        case 33: //'!'
            operator_node->code = switch_logic_labels(operator_node->code);
            break;
        case 45: //'-'
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_RSUBI, expr->temp, "0", operator_node->temp, NULL));
            break;
        default:
            printf("\n\033[1;4;31mINVALID OPERATION!\033[0;31m Unary operator unidentified!\033[0m\n");
            exit(ERR_INVALID_EXPR);
    }
    operator_node->code = opList_concatLeft(operator_node->code, expr->code);
    return operator_node;
}

ASTree* resolve_binary_expr(ASTree* expr1, ASTree* operator_node, ASTree* expr2){
    operator_node->temp = get_temp();
    switch (operator_node->data.label[0]+operator_node->data.label[1]){
        //ARIT EXPR
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
        //LOGIC EXPR
        case 123: //'>='
            remove_logic_labels(expr2->code);
            remove_logic_labels(expr1->code);
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_GE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 121: //'<='
            remove_logic_labels(expr2->code);
            remove_logic_labels(expr1->code);
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_LE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 62: //'>'
            remove_logic_labels(expr2->code);
            remove_logic_labels(expr1->code);
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_GT, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 60: //'<'
            remove_logic_labels(expr2->code);
            remove_logic_labels(expr1->code);
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_LT, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 94: //'!='
            remove_logic_labels(expr2->code);
            remove_logic_labels(expr1->code);
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_NE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 122: //'=='
            remove_logic_labels(expr2->code);
            remove_logic_labels(expr1->code);
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_EQ, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 76: //"&&"
            remove_logic_labels(expr2->code);
            remove_logic_labels(expr1->code);
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_AND, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 248: //"||"
            remove_logic_labels(expr2->code);
            remove_logic_labels(expr1->code);
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
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
        operator_node = resolve_binary_expr(expr1, operator_node, expr2);
    }
    else
        operator_node = resolve_unary_expr(operator_node, expr2);
    return operator_node;
}