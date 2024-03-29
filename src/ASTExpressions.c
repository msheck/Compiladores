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
            &&(current->next->value->operation == OP_LOADI)
            &&(current->next->next->next->next->value->operation == OP_LOADI)
            &&(current->next->next->next->next->next->next == NULL)) {
                buffer = current->next->value->arg0;
                current->next->value->arg0 = current->next->next->next->next->value->arg0;
                current->next->next->next->next->value->arg0 = buffer;
                break;
            }
            current = current->next;
        }
    }
    return code;
}

OpList* set_logic_labels(OpList* code, char* temp){
    char* label_true = get_label();
    char* label_false = get_label();
    char* label_next_command = get_label();
    code = opList_pushLeft(code, op_new(OP_LABEL, label_next_command, NULL, NULL, NULL));
    code = opList_pushLeft(code, op_new(OP_LOADI, "0", NULL, temp, NULL));
    code = opList_pushLeft(code, op_new(OP_LABEL, label_false, NULL, NULL, NULL));
    code = opList_pushLeft(code, op_new(OP_JUMPI, NULL, NULL, label_next_command, NULL));
    code = opList_pushLeft(code, op_new(OP_LOADI, "1", NULL, temp, NULL));
    code = opList_pushLeft(code, op_new(OP_LABEL, label_true, NULL, NULL, NULL));
    code = opList_pushLeft(code, op_new(OP_CBR, temp, NULL, label_true, label_false));     
    return code;
}

ASTree* resolve_unary_expr(ASTree* operator_node, ASTree *expr){
    switch (operator_node->data.label[0]) {
        case 33: //'!'
            if(expr->node_type == NODE_TYPE_BOOL){
                operator_node->temp = expr->temp;
                expr->code = switch_logic_labels(expr->code);
                break;
            }
        case 45: //'-'
            operator_node->temp = get_temp();
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_RSUBI, expr->temp, "0", operator_node->temp, NULL));
            break;
        default:
            printf("\n\033[1;4;31mOPERADOR INVALIDO!\033[0;31m Operador unario nao identificado!\033[0m\n");
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
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_GE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 121: //'<='
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_LE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 62: //'>'
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_GT, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 60: //'<'
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_LT, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 94: //'!='
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_NE, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 122: //'=='
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_CMP_EQ, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 76: //"&&"
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_AND, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        case 248: //"||"
            operator_node->code = set_logic_labels(operator_node->code, operator_node->temp);
            operator_node->code = opList_pushLeft(operator_node->code, op_new(OP_OR, expr1->temp, expr2->temp, operator_node->temp, NULL));
            break;
        default:
            printf("\n\033[1;4;31mOPERADOR INVALIDO!\033[0;31m Operador binario nao identificado!\033[0m\n");
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
                printf("\n\033[1;4;31mEXPRESSAO INVALIDA EM %d!\033[0;31m Tipo char nao pode realizar expressoes com tipos diferentes!\033[0m\n", operator.line_number);
                exit(ERR_INVALID_EXPR);
            }
            else{
                printf("\n\033[1;4;31mEXPRESSAO INVALIDA EM %d!\033[0;31m Tipo da expressao nao foi bem definido!\033[0m\n", operator.line_number);
                exit(ERR_INVALID_EXPR);
            }
        }
        operator_node = resolve_binary_expr(expr1, operator_node, expr2);
    }
    else
        operator_node = resolve_unary_expr(operator_node, expr2);
    return operator_node;
}