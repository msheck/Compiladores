/*
INF01147 - Compiladores 2022/2
Professor: 
	-Lucas Schnorr

Desenvolvido pelos alunos:
	-Mateus Severgnini Heck 
	-Vinicius Meirelles Pereira
*/

#include "ASTExpressions.h"

char* resolve_unary_expr(char operator, ASTree *expr, int node_type){
    int expr_intValue;
    float expr_floatValue;
    char expr_charValue;
    char* buffer= malloc(sizeof(char*)*(strlen(expr->data.value)+1));
    switch (node_type){
        case 1: //bool
        case 3: //int
            expr_intValue = atoi(expr->data.value);
            break;
        case 4: //float
            expr_floatValue = atof(expr->data.value);
            break;
        case 2: // char
            expr_charValue = expr->data.value[0];
            break;
        default:
            printf("\n\033[1;4;31mINVALID PARAMETER IN UNARY EXPRESSION IN LINE %d!\033[0;31m Couldn't convert %s to appropriate type!\033[0m\n", expr->data.line_number, expr->data.label);
            return NULL;
    }
    switch (operator)
    {
    case 33: //'!'
        if(expr->data.value == 0){ //false
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
            expr1_intValue = atoi(expr1->data.value);
            expr2_intValue = atoi(expr2->data.value);
            break;
        case 4: //float
            expr1_floatValue = atof(expr1->data.value);
            expr2_floatValue = atof(expr2->data.value);
            break;
        case 2: // char
            expr1_charValue = expr1->data.value[0];
            expr2_charValue = expr2->data.value[0];
            break;
        default:
            printf("\n\033[1;4;31mINVALID PARAMETER IN BINARY EXPRESSION!\033[0;31m Couldn't convert %s %s %s to appropriate type!\033[0m\n", expr1->data.label, operator, expr2->data.label);
            return NULL;
    }
    switch (operator[0]+operator[1]){
        case 37: //'%'
            switch (node_type){
                case 1:
                case 3: //int or bool
                    buffer = malloc(sizeof(char*)*abs((strlen(expr2->data.value)+1)));
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
                    buffer= malloc(sizeof(char*)*abs((strlen(expr1->data.value)-strlen(expr2->data.value))));
                    value_i = expr1_intValue / expr2_intValue;
                    sprintf(buffer, "%d", value_i);
                    return buffer;
                case 4: //float
                    buffer= malloc(sizeof(char*)*abs((strlen(expr1->data.value)-strlen(expr2->data.value))));
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
                    buffer= malloc(sizeof(char*)*abs((strlen(expr1->data.value)+strlen(expr2->data.value))));
                    value_i = expr1_intValue * expr2_intValue;
                    sprintf(buffer, "%d", value_i);
                    return buffer;
                case 4: //float
                    buffer= malloc(sizeof(char*)*abs((strlen(expr1->data.value)+strlen(expr2->data.value))));
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
                    if(strlen(expr1->data.value)>strlen(expr2->data.value))
                        buffer= malloc(sizeof(char*)*(strlen(expr1->data.value)));
                    else
                        buffer= malloc(sizeof(char*)*(strlen(expr2->data.value)));
                    value_i = expr1_intValue - expr2_intValue;
                    sprintf(buffer, "%d", value_i);
                    return buffer;
                case 4: //float
                    if(strlen(expr1->data.value)>strlen(expr2->data.value))
                        buffer= malloc(sizeof(char*)*(strlen(expr1->data.value)));
                    else
                        buffer= malloc(sizeof(char*)*(strlen(expr2->data.value)));
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
                    if(strlen(expr1->data.value)>strlen(expr2->data.value))
                        buffer= malloc(sizeof(char*)*(strlen(expr1->data.value)+1));
                    else
                        buffer= malloc(sizeof(char*)*(strlen(expr2->data.value)+1));
                    value_i = expr1_intValue + expr2_intValue;
                    sprintf(buffer, "%d", value_i);
                    return buffer;
                case 4: //float
                    if(strlen(expr1->data.value)>strlen(expr2->data.value))
                        buffer= malloc(sizeof(char*)*(strlen(expr1->data.value)+1));
                    else
                        buffer= malloc(sizeof(char*)*(strlen(expr2->data.value)+1));
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
                    buffer= malloc(sizeof(char*)*abs((strlen(expr1->data.value)-strlen(expr2->data.value))));
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

ASTree* ast_expr_node(ASTree *expr1, lexValue operator, ASTree *expr2){
    int node_type = expr2->node_type;
    char* expr_result;
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
    ASTree* expr_node = ast_new_node(operator, node_type);
    expr_node->data.value = strdup(expr_result);
    return expr_node;
}