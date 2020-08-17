/*
  General functions, global to entire project.
  Marco Martins marcopaulomartins@hotmail.com
  2020/08/16
*/

#include "general.h"
#include <math.h>

/*
  Calculate Euclidean norm of a vector.
  Args: 
    v1 Pointer for vector 
    n  Vector dimension
*/
float norm(float v1[], int n) {
    float d = 0;
    for (int i=0; i<n; i++) {
        d += pow (v1[i], 2);
    }
    return sqrt(d);
}
/*
  Calculate Euclidean distance between 2 vectores.
  Args: 
    v1 Pointer for vector 1
    v2 Pointer for vector 2
    n  Vectores dimensions
*/
float distance(float v1[], float v2[], int n) {
    /*
    printf("Listar os vectores para a Distancia\n"); 
    listarArray(v1, n);
    printf("Listar os vectores para a Distancia\n"); 
    listarArray(v2, n);
    */

    float d = 0;
    for (int i=0; i<n; i++) {
        //printf("v1=%.2f v2=%.2f pow=%f \n", v1[i], v2[i], pow (v1[i]-v2[i], 2));
        //printf("v1=%.2f v2=%.2f pow=%f \n", *v1, *v2, pow (*v1-*v2, 2));
        //printf("v1=%.2f v2=%.2f pow=%f \n", v1[i], v2[i], pow (v1[i]-v2[i], 2));
        //d += pow (*v1-*v2, 2);
        d += pow (v1[i]-v2[i], 2);
        //v1++;
        //v2++;
    }
    //printf("Distancia=%f\n", sqrt(d));
    return sqrt(d);
}

void  initArray(int a[], int n, int value) {
    for (int i=0; i<n; i++) {
        a[i]=value;
    }
}