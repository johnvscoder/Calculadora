#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define DIM_MAX_FUN 6
#define NUM_MAX_ARGS 3
#define DIM_MAX_NUM 50

double NaN = 0.0 / 0.0;
int indentacion = -4;

bool esDigito(char caracter);
bool esLetra(char caracter);
double coc(double a, double b);
double res(double a, double b);
double expresionN(char *expresion, int lon);
double expresion0(char *expresion0, int lon);

void p();


/*Evalúa una expresión nivel n (tiene su definición formal) y devuelve
  su valor*/
double calc(char *expresion) {
    return expresionN(expresion, strlen(expresion));
}

/*Evalúa una expresión nivel n (tiene su definición formal) y devuelve
  su valor. En este método se necesita el argumento lon para la recursividad que ocurre en él*/
double expresionN(char *expresion, int lon) {
    indentacion += 4;
    bool enBusqueda = false;
    bool hallado = false;
    int cont;
    int pos;
    for(int i = 0; i < lon; i++) {
        if(!enBusqueda && (expresion[i] == '(' || expresion[i] == ',')) {
            enBusqueda = true;
            pos = i + 1;
            continue;
        }
        if(enBusqueda && !hallado) {
            if(esDigito(expresion[i]) || expresion[i] == '.')
                continue;
            if((expresion[i] == 'e' || expresion[i] == 'E') && (esDigito(expresion[i - 1]) || expresion[i - 1] == '.'))
                continue;
            if((expresion[i] == '+' || expresion[i] == '-') && (i == pos || expresion[i - 1] == 'e' || expresion[i - 1] == 'E'))
                continue;
            if((pos >= 2 && esLetra(expresion[pos - 2])) || expresion[pos - 1] == ',') {
                if(expresion[i] == '(' && i == pos)
                    continue;
                if(expresion[i] == ')' && (esDigito(expresion[i - 1]) || expresion[i - 1] == '.') &&
                expresion[pos] == '(')
                    continue;
                if((expresion[i] == '+' || expresion[i] == '-') && expresion[i - 1] == '(' && i == pos + 1)
                    continue;
            }
            if(expresion[i] == ')' || expresion[i] == ',') {
                enBusqueda = false;
                continue;
            }
            hallado = true;
            cont = 1;
            if(expresion[i] == '(')
                cont++;
            continue;
        }
        if(hallado) {
            if(cont == 1 && (expresion[i] == ')' || expresion[i] == ',')) {
                char *numero = (char*) calloc(DIM_MAX_NUM, 1);
                int lonSubExpresion = i - pos;
                int lonVieja = strlen(expresion + pos);
                //p(); printf("1. expresion=%s i=%i lon=%i\n", expresion, i, lon);
                sprintf(numero, "%lf", expresionN(expresion + pos, lonSubExpresion));
                int lonNueva = strlen(expresion + pos);
                i += lonNueva - lonVieja;
                lon += lonNueva - lonVieja;
                //p(); printf("2. expresion=%s i=%i lon=%i cambio=%i\n", expresion, i, lon, lonNueva - lonVieja);
                int lonNumero = strlen(numero);
                int lonExpresion2 = pos + lonNumero + strlen(expresion + i);
                int lonNuevaExpresion = pos + lonNumero + lon - i;
                char *expresion2 = (char*) calloc(lonExpresion2 + 1, 1);
                strncat(expresion2, expresion, pos);
                strcat(expresion2, numero);
                strcat(expresion2, expresion + i);
                strcpy(expresion, expresion2);
                free(numero);
                free(expresion2);
                enBusqueda = false;
                hallado = false;
                i = pos + lonNumero;
                lon = lonNuevaExpresion;
                //p(); printf("3. expresion=%s i=%i lon=%i\n", expresion, i, lon);
                continue;
            }
            if(expresion[i] == '(')
                cont++;
            if(expresion[i] == ')')
                cont--;
            continue;
        }
    }
    indentacion -= 4;
    return expresion0(expresion, lon);
}

/* Evalúa una expresión nivel 0 (tiene su definición formal) y devuelve
   su valor*/
double expresion0(char *expresion0, int lon) {
    double resultado = 0;
    double sumando = 1;
    double operando;
    double base;

    bool signoSuma = true; //true = +, false = -
    char operador = '*';
    int inicioOperando = 0;
    int inicioFuncion = 0;

    bool enFuncion = false;
    bool enPotencia = false;
    char funcion[DIM_MAX_FUN];
    memset(funcion, '\0', DIM_MAX_FUN);
    double argumentosFuncion[NUM_MAX_ARGS];
    int lonArgumentos = 0;

    for(int i = 0; i < lon; i++) {
        switch(expresion0[i]) {
            case '+':
            case '-':
                if(i == 0) {
                    signoSuma = expresion0[i] == '+';
                    continue;
                }
                if(expresion0[i - 1] == '(' || expresion0[i - 1] == ',') {
                    inicioOperando = i;
                    continue;
                }
                if(expresion0[i - 1] == 'e' || expresion0[i - 1] == 'E')
                    continue;
                if(signoSuma)
                    resultado += sumando;
                else
                    resultado -= sumando;
                if(!isfinite(resultado))
                    return NaN;
                signoSuma = expresion0[i] == '+';
                sumando = 1;
                operador = '*';
                break;
            case '*':
            case '/':
            case '%':
            case '#':
                if(operador != '*' && operando == 0)
                    return NaN;
                if(operador == '*')
                    sumando *= operando;
                if(operador == '/')
                    sumando /= operando;
                if(operador == '#')
                    sumando = coc(sumando, operando);
                if(operador == '%')
                    sumando = res(sumando, operando);
                if(!isfinite(sumando))
                    return NaN;
                operador = expresion0[i];
                break;
            case '^':
                if(enPotencia) {
                    if((base == 0 && operando < 0) || (base < 0 && operando != floor(operando)))
                        return NaN;
                    base = pow(base, operando);
                    if(!isfinite(base))
                        return NaN;
                }
                else {
                    base = operando;
                    enPotencia = true;
                }
                break;
            case '(':
                if(i != 0 && esLetra(expresion0[i - 1])) {
                    memset(funcion, '\0', DIM_MAX_FUN);
                    strncpy(funcion, expresion0 + inicioFuncion, i - inicioFuncion);
                }
                break;
            case ')':
                if(enFuncion) {
                    if(i != lon - 1 && (expresion0[i + 1] == ')' || expresion0[i + 1] == ',')) {
                        argumentosFuncion[lonArgumentos] = atof(expresion0 + inicioOperando);
                        if(!isfinite(argumentosFuncion[lonArgumentos]))
                            return NaN;
                        lonArgumentos++;
                        continue;
                    }
                    enFuncion = false;
                    if(strcmp(funcion, "abs") == 0)
                        operando = fabs(argumentosFuncion[0]);
                    if(strcmp(funcion, "floor") == 0)
                        operando = floor(argumentosFuncion[0]);
                    if(strcmp(funcion, "ceil") == 0)
                        operando = ceil(argumentosFuncion[0]);
                    if(strcmp(funcion, "sqrt") == 0) {
                        if(argumentosFuncion[0] < 0)
                            return NaN;
                        operando = sqrt(argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "root") == 0) {
                        if(argumentosFuncion[0] == 0)
                            return NaN;
                        if(argumentosFuncion[1] < 0 && 
                        argumentosFuncion[0] == floor(argumentosFuncion[0])) {
                            if(res(argumentosFuncion[0],2) == 1) {
                                double b = 1 / argumentosFuncion[0];
                                if(!isfinite(b))
                                    return NaN;
                                operando = -pow(-argumentosFuncion[1], b);
                            }
                            else
                                return NaN;
                        }
                        else {
                            double a = argumentosFuncion[1], b = 1 / argumentosFuncion[0];
                            if(!isfinite(b))
                                return NaN;
                            if(a < 0 && res(b, 2) == 0)
                                return NaN;
                            if((a == 0 && b < 0) || (a < 0 && b != floor(b)))
                                return NaN;
                            operando = pow(a, b);
                        }
                    }
                    if(strcmp(funcion, "sin") == 0)
                        operando = sin(argumentosFuncion[0]);
                    if(strcmp(funcion, "cos") == 0)
                        operando = cos(argumentosFuncion[0]);
                    if(strcmp(funcion, "tan") == 0) {
                        if(res(argumentosFuncion[0], M_PI) == M_PI / 2)
                            return NaN;
                        operando = tan(argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "csc") == 0) {
                        if(res(argumentosFuncion[0], M_PI) == 0)
                            return NaN;
                        operando = 1 / sin(argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "sec") == 0) {
                        if(res(argumentosFuncion[0], M_PI) == M_PI / 2)
                            return NaN;
                        operando = 1 / cos(argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "cot") == 0) {
                        if(res(argumentosFuncion[0], M_PI) == 0)
                            return NaN;
                        operando = cos(argumentosFuncion[0]) / sin(argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "asin") == 0) {
                        if(argumentosFuncion[0] < -1 || argumentosFuncion[0] > 1)
                            return NaN;
                        operando = asin(argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "acos") == 0) {
                        if(argumentosFuncion[0] < -1 || argumentosFuncion[0] > 1)
                            return NaN;
                        operando = acos(argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "atan") == 0)
                        operando = atan(argumentosFuncion[0]);
                    if(strcmp(funcion, "acsc") == 0) {
                        if(-1 < argumentosFuncion[0] && argumentosFuncion[0] < 1)
                            return NaN;
                        if(argumentosFuncion[0] <= -1)
                            operando = -1.5 * M_PI + acos(1 / argumentosFuncion[0]);
                        else
                            operando = M_PI / 2 - acos(1 / argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "asec") == 0) {
                        if(-1 < argumentosFuncion[0] && argumentosFuncion[0] < 1)
                            return NaN;
                        if(argumentosFuncion[0] <= -1)
                            operando = 2 * M_PI - acos(1 / argumentosFuncion[0]);
                        else
                            operando = acos(1 / argumentosFuncion[0]);
                    }
                    if(strcmp(funcion, "acot") == 0) 
                        operando = M_PI / 2 - atan(argumentosFuncion[0]);
                    if(strcmp(funcion, "exp") == 0) 
                        operando = exp(argumentosFuncion[0]);
                    if(strcmp(funcion, "log") == 0) {
                        if(lonArgumentos == 1) {
                            if(argumentosFuncion[0] <= 0)
                                return NaN;
                            operando = log(argumentosFuncion[0]);
                        }
                        else {
                            if(argumentosFuncion[0] <= 0 || argumentosFuncion[0] == 1 || argumentosFuncion[1] <= 0)
                                return NaN;
                            operando = log(argumentosFuncion[1]) / log(argumentosFuncion[0]);
                        }
                    }
                    if(!isfinite(operando))
                        return NaN;
                    if(enPotencia && (i == lon - 1 || expresion0[i + 1] != '^')) {
                        if((base == 0 && operando < 0) || (base < 0 && operando != floor(operando)))
                            return NaN;
                        operando = pow(base, operando);
                        if(!isfinite(operando))
                            return NaN;
                        enPotencia = false;
                    }
                    if(!enPotencia && (i == lon - 1 || expresion0[i + 1] == '+' || expresion0[i + 1] == '-')) {
                        if(operador != '*' && operando == 0)
                            return NaN;
                        if(operador == '*') {
                            sumando *= operando;
                        }
                        if(operador == '/')
                            sumando /= operando;
                        if(operador == '%')
                            sumando = res(sumando, operando);
                        if(operador == '#')
                            sumando = coc(sumando, operando);
                        if(!isfinite(sumando))
                            return NaN;
                    }
                    if(i == lon - 1) {
                        if(signoSuma)
                            resultado += sumando;
                        else
                            resultado -= sumando;
                        if(!isfinite(resultado))
                            return NaN;
                        return resultado;
                    }
                }
                else {
                    operando = atof(expresion0 + inicioOperando);
                    if(!isfinite(operando))
                        return NaN;
                    if(enPotencia && (i == lon - 1 || expresion0[i + 1] != '^')) {
                        if((base == 0 && operando < 0) || (base < 0 && operando != floor(operando)))
                            return NaN;
                        operando = pow(base, operando);
                        if(!isfinite(operando))
                            return NaN;
                        enPotencia = false;
                    }
                    if(!enPotencia && (i == lon - 1 || expresion0[i + 1] == '+' || expresion0[i + 1] == '-')) {
                        if(operador != '*' && operando == 0)
                            return NaN;
                        if(operador == '*')
                            sumando *= operando;
                        if(operador == '/')
                            sumando /= operando;
                        if(operador == '%')
                            sumando = res(sumando, operando);
                        if(operador == '#')
                            sumando = coc(sumando, operando);
                        if(!isfinite(sumando))
                            return NaN;
                    }
                    if(i == lon - 1) {
                        if(signoSuma)
                            resultado += sumando;
                        else
                            resultado -= sumando;
                        if(!isfinite(resultado))
                            return NaN;
                        return resultado;
                    }
                }
               break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':
                if(i == 1 && (expresion0[0] == '+' || expresion0[0] == '-'))
                    inicioOperando = 1;
                if(i == 0)
                    inicioOperando = 0;
                if(i >= 2 && (expresion0[i - 1] == '+' || expresion0[i - 1] == '-') && expresion0[i - 2] != '(' && expresion0[i - 2] != ',' && expresion0[i - 2] != 'e' && expresion0[i - 2] != 'E')
                    inicioOperando = i;
                if(i != 0 && (expresion0[i - 1] == '*' || expresion0[i - 1] == '/' || 
                expresion0[i - 1] == '%' || expresion0[i - 1] == '#' || expresion0[i - 1] == '^'))
                    inicioOperando = i;
                if(i != 0 && (expresion0[i - 1] == '(' || expresion0[i - 1] == ','))
                    inicioOperando = i;
                if(enFuncion && (expresion0[i + 1] == ',' ||
                 (expresion0[i + 1] == ')' && (i + 2 > lon - 1 || 
                 (expresion0[i + 2] != ')' && expresion0[i + 2] != ','))))) {
                    argumentosFuncion[lonArgumentos] = atof(expresion0 + inicioOperando);
                    if(!isfinite(argumentosFuncion[lonArgumentos]))
                        return NaN;
                    lonArgumentos++;
                    continue;
                }
                if(i == lon - 1 || expresion0[i + 1] == '^' || expresion0[i + 1] == '*' ||
                 expresion0[i + 1] == '/' || expresion0[i + 1] == '%' || expresion0[i + 1] == '#' 
                 || expresion0[i + 1] == '+' || expresion0[i + 1] == '-') {
                    operando = atof(expresion0 + inicioOperando);
                    if(!isfinite(operando))
                        return NaN;
                    if(enPotencia && (i == lon - 1 || expresion0[i + 1] != '^')) {
                        if((base == 0 && operando < 0) || (base < 0 && operando != floor(operando)))
                            return NaN;
                        operando = pow(base, operando);
                        if(!isfinite(operando))
                            return NaN;
                        enPotencia = false;
                    }
                    if(!enPotencia && (i == lon - 1 || expresion0[i + 1] == '+' || expresion0[i + 1] == '-')) {
                        if(operador != '*' && operando == 0)
                            return NaN;
                        if(operador == '*')
                            sumando *= operando;
                        if(operador == '/')
                            sumando /= operando;
                        if(operador == '%')
                            sumando = res(sumando, operando);
                        if(operador == '#')
                            sumando = coc(sumando, operando);
                        if(!isfinite(sumando))
                            return NaN;
                    }
                    if(i == lon - 1) {
                        if(signoSuma)
                            resultado += sumando;
                        else
                            resultado -= sumando;
                        if(!isfinite(resultado))
                            return NaN;
                        return resultado;
                    }
                }   
                break;
            default:
                if(expresion0[i] == ',')
                    continue;
                if(i != 0 && (expresion0[i] == 'e' || expresion0[i] == 'E') && (esDigito(expresion0[i - 1]) || expresion0[i - 1] == '.'))
                    continue;
                if(i == 0 || !esLetra(expresion0[i - 1])) {
                    inicioFuncion = i;
                    enFuncion = true;
                    lonArgumentos = 0;
                }
        }
    }
    return 0;
} //Fin del método expresion0

//Método ya probado
bool esDigito(char caracter) {
    return 48 <= caracter && caracter <= 57;
}

//Método ya probado
bool esLetra(char caracter) {
    return (65 <= caracter && caracter <= 90) || (97 <= caracter && caracter <= 122);
}

//Método ya probado
double coc(double a, double b) {
    if(b == 0)
        return NaN;
    else
        return b > 0 ? floor(a / b) : ceil(a / b);
}

//Método ya probado
double res(double a, double b) {
    if(b == 0)
        return NaN;
    else
        return a - b * coc(a, b);
}

void p() {
    for(int i = 0; i < indentacion; i++)
        printf(" ");
}