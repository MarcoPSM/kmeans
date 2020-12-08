#ifndef GEN_H
#define GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FLT_MAX 340282346638528859811704183484516925440.0
#define EXPECTED_ARGUMENTS 2
#define ADDITIONAL_COLUMNS 2
#define ENTITY_MAX_DIMENSION 4
#define MAX_DIM (ENTITY_MAX_DIMENSION + ADDITIONAL_COLUMNS)
#define TRUE 1 
#define FALSE 0
#define SPACE ' '
#define MAX_ITERATIONS 500
#define OUTPUT "output/kmeans.out"
#define OUTPUT_C "output/centroids.out"
#define DEFAULT_CLUSTERS_NUMBER 4
#define DEFAULT_MAX_CONVERGENCE 0

#define METHOD_A 1 // iterador
#define METHOD_B 2 // bloco

double norm(double v1[], int n);
double distance(double v1[], double v2[], int n);
void  initArray(int a[], int n, int value);

#endif