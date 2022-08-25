#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "matematicas/matematicas.c"


void escribir(char const *texto, char const *ruta);
void ejecutarPrueba(char const *ruta, int estadisticas);

int main() {
    char *ruta = (char*) calloc(300, 1);

    printf("Ingrese la ruta del archivo: ");
    fgets(ruta, 100, stdin);
    if(strlen(ruta) != 0 && (ruta[strlen(ruta) - 1] == '\n'))
        ruta[strlen(ruta) - 1] = '\0';
    
    ejecutarPrueba(ruta, 1);

    free(ruta);
    
    return 0;
}


void escribir(char const *texto, char const *ruta) {
    FILE *file = fopen(ruta, "a+t");
    for(int i = 0; i < strlen(texto); i++) {
        fputc(texto[i], file);
    }
    fclose(file);
}

//Este metodo no libera la memoria apuntada por ruta
void ejecutarPrueba(char const *ruta, int estadisticas) {
    //estadisticas = 0 no muestra estadisticas, cualquier otro valor si las muestra

    int contBuenas = 0;
    int contMalas = 0;

    FILE *file = fopen(ruta, "r");
    if(file == NULL) {
        printf("Error de lectura\n");
    }
    else {
        char character;
        int dondeGuardar = 0; //0: expresion, 1: numero
        char *expresion = (char*) calloc(300, 1);
        char *numero = (char*) calloc(300, 1);
        int i = 0;

        bool espacioEncontrado = false;
        bool saltoLineaEncontrado = false;

        char *nuevaRuta = (char*) calloc(300, 1);
        strcpy(nuevaRuta, "resultado__");
        strcat(nuevaRuta, ruta);

        while((character = fgetc(file)) != EOF) {
            if(character == ' ') {
                if(espacioEncontrado) {
                    escribir(" ", nuevaRuta);
                    continue;
                }
                espacioEncontrado = true;
                dondeGuardar = 1;
                i = 0;
                continue;
            }
            if(character == '\n') {
                if(saltoLineaEncontrado) {
                    escribir("\n", nuevaRuta);
                    continue;
                }
                saltoLineaEncontrado = true;
                //Aqui ocurre la comparacion entre los resultados correctos y los que da el metodo calc
                double resultadoCalc = calc(expresion);
                double resultadoCorrecto = atof(numero);
                escribir(expresion, nuevaRuta);
                escribir(" ", nuevaRuta);
                escribir(numero, nuevaRuta);
                if((!isfinite(resultadoCalc) && strcmp(numero, "nan") == 0) ||
                (isfinite(resultadoCalc) && (strcmp(numero, "nan") != 0) &&
                 (fabs(resultadoCalc - resultadoCorrecto) <= 0.001))) {
                    escribir(" si\n", nuevaRuta);
                    contBuenas++;
                }
                else {
                    escribir(" no\n", nuevaRuta);
                    contMalas++;
                }

                memset(expresion, 0, 300);
                memset(numero, 0, 300);
                dondeGuardar = 0;
                i = 0;
                continue;
            }

            espacioEncontrado = false;
            saltoLineaEncontrado = false;

            if(dondeGuardar == 0)
                expresion[i] = character;
            else
                numero[i] = character;
            i++;
        }

        //Aqui ocurre la comparacion entre los resultados correctos y los que da el metodo calc
        double resultadoCalc = calc(expresion);
        double resultadoCorrecto = atof(numero);
        escribir(expresion, nuevaRuta);
        escribir(" ", nuevaRuta);
        escribir(numero, nuevaRuta);
        if((!isfinite(resultadoCalc) && strcmp(numero, "nan") == 0) ||
                (isfinite(resultadoCalc) && (strcmp(numero, "nan") != 0) &&
                 (fabs(resultadoCalc - resultadoCorrecto) <= 0.001))) {
            escribir(" si\n", nuevaRuta);
            contBuenas++;
        }
        else {
            escribir(" no\n", nuevaRuta);
            contMalas++;
        }

        if(estadisticas != 0) {
            int contBuenas2 = contBuenas;
            int contMalas2 = contMalas;
            int contTotal2 = contBuenas + contMalas;
            if(contBuenas == 0)
                contBuenas2++;
            if(contMalas == 0)
                contMalas2++;
            if(contTotal2 == 0)
                contTotal2++;

            char *contBuenasTexto = (char*) calloc((int) (ceil(log10(contBuenas2) + 2) * sizeof(char)), 1);
            char *contMalasTexto = (char*) calloc((int) (ceil(log10(contMalas2) + 2) * sizeof(char)), 1);
            char *contTotal = (char*) calloc((int) (ceil(log10(contTotal2) + 2) * sizeof(char)), 1);
            sprintf(contBuenasTexto, "%d", contBuenas);
            sprintf(contMalasTexto, "%d", contMalas);
            sprintf(contTotal, "%d", contBuenas + contMalas);
            
            escribir("\nEstadisticas\n", nuevaRuta);
            escribir("Cantidad de pruebas positivas: ", nuevaRuta);
            escribir(contBuenasTexto, nuevaRuta);
            escribir("\n", nuevaRuta);
            escribir("Cantidad de pruebas negativas: ", nuevaRuta);
            escribir(contMalasTexto, nuevaRuta);
            escribir("\n", nuevaRuta);
            escribir("Cantidad de pruebas total: ", nuevaRuta);
            escribir(contTotal, nuevaRuta);
            escribir("\n", nuevaRuta);

            free(contBuenasTexto);
            free(contMalasTexto);
            free(contTotal);
        }

        free(expresion);
        free(numero);
        free(nuevaRuta);
    }

    fclose(file);
}