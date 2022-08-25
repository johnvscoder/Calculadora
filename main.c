#include <stdio.h>
#include <string.h>
#include "matematicas.c"
#include <stdlib.h>

#define MAX_LON 600

//Para probar las funciones
int main() {
    char *expresion = calloc(MAX_LON, 1);
    printf("Ingrese expresion: ");
    fgets(expresion, MAX_LON, stdin);
    if((strlen(expresion) > 0) && (expresion[strlen(expresion) - 1] == '\n'))
        expresion[strlen(expresion) - 1] = '\0';
    double resultado = calc(expresion);
    printf("El resultado: %lf\n", resultado);
    return 0;
}