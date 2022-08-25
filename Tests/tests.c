#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matematicas/utilidades.c"

int main() {

    // struct vec3 v1 = {1, 2, 3};
    // struct vec3 v2 = {10, 20, 30};
    // sum2(&v1, &v2);
    // printf("%lf %lf %lf\n", v1.x, v1.y, v1.z);
    // printf("%lf %lf %lf\n", v2.x, v2.y, v2.z);

    // struct vec3 v1 = {1, 2, 3};
    // struct vec3 v2 = {10, 20, 30};
    // struct vec3 v3;
    // restar(&v3, &v1, &v2);
    // printf("%lf %lf %lf\n", v1.x, v1.y, v1.z);
    // printf("%lf %lf %lf\n", v2.x, v2.y, v2.z);
    // printf("%lf %lf %lf\n", v3.x, v3.y, v3.z);

    // struct vec3 v1 = {1, 2, 3};
    // struct vec3 v2 = {10, 20, 30};
    // restar2(&v1, &v2);
    // printf("%lf %lf %lf\n", v1.x, v1.y, v1.z);
    // printf("%lf %lf %lf\n", v2.x, v2.y, v2.z);

    // struct vec3 v = {1, 2, 3};
    // struct vec3 vNuevo;
    // escalar(&vNuevo, &v, 3);
    // printf("%lf %lf %lf\n", vNuevo.x, vNuevo.y, vNuevo.z);
    // printf("%lf %lf %lf\n", v.x, v.y, v.z);

    // struct vec3 v = {1, 2, 3};
    // escalar2(&v, 3);
    // printf("%lf %lf %lf\n", v.x, v.y, v.z);

    // struct vec3 v = {1, 2, 3};
    // float r = magnitud(&v);
    // printf("%lf\n", r);

    // struct vec3 v1 = {1, 2, 3};
    // struct vec3 v2 = {10, 20, 30}; 
    // float r = distancia(&v1, &v2);
    // printf("%lf\n", r);

    // struct vec3 v1 = {2, 2, 3};
    // struct vec3 v2 = {10, 20, 30}; 
    // float r = punto(&v1, &v2);
    // printf("%lf\n", r);

    // struct vec3 v1 = {1, 5, 7};
    // struct vec3 v2 = {10, 20, 30};
    // struct vec3 v3;
    // cruz(&v3, &v1, &v2);
    // printf("%lf %lf %lf\n", v1.x, v1.y, v1.z);
    // printf("%lf %lf %lf\n", v2.x, v2.y, v2.z);
    // printf("%lf %lf %lf\n", v3.x, v3.y, v3.z);

    // struct vec3 v1 = {1, 5, 7};
    // struct vec3 v2 = {10, 20, 30};
    // cruz2(&v1, &v2);
    // printf("%lf %lf %lf\n", v1.x, v1.y, v1.z);
    // printf("%lf %lf %lf\n", v2.x, v2.y, v2.z);

    // struct vec3 v1 = {1, 5, 7};
    // struct vec3 v2;
    // rotarZ(&v2, &v1, 1.5);
    // printf("%lf %lf %lf\n", v1.x, v1.y, v1.z);
    // printf("%lf %lf %lf\n", v2.x, v2.y, v2.z);

    struct vec3 v1 = {1, 5, 7};
    rotarZ2(&v1, 1.5);
    printf("%lf %lf %lf\n", v1.x, v1.y, v1.z);

    return 0;
}

