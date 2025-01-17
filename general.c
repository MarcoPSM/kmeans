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
double norm(double v1[], int n) {
    double d = 0;
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
double distance(double v1[], double v2[], int n) {
    double d = 0;
    for (int i=0; i<n; i++) {
        d += pow (v1[i]-v2[i], 2);
    }
    //printf("Distancia=%f\n", sqrt(d));
    //return sqrt(d);
    return d;
}

/**
  Array initialization with a specific value
*/
void  initArray(int a[], int n, int value) {
    for (int i=0; i<n; i++) {
        a[i]=value;
    }
}