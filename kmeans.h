#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>

#define EXPECTED_ARGUMENTS 2
#define ADDITIONAL_COLUMNS 2
#define ENTITY_MAX_DIMENSION 2
#define MAX_DIM (ENTITY_MAX_DIMENSION + ADDITIONAL_COLUMNS)
#define TRUE 1 
#define FALSE 0
#define SPACE ' '
#define MAX_ITERATIONS 100
#define OUTPUT "output/kmeans.out"

void  initArray(int a[], int n, int value);
int   getNumberOfEntities(char *filename);
int   getNumberOfDimensions(char *filename);
void  loadMatrizFromFile(float MATRIZ[][MAX_DIM], int n, int m, char *filename);
void  listarMatriz(float MATRIZ[][MAX_DIM], int n, int m);
void  listarArray(float a[], int n);

void  calculateNorm(float MATRIZ[][MAX_DIM], int n, int m);
void  initClusterAssociation(float MATRIZ[][MAX_DIM], int n, int m);

float distance(float v1[], float v2[], int n);
float norm(float v1[], int n);

void  initClusters(int a[], int n);
void  listarCluster(float MATRIZ[][MAX_DIM], int n, int m, int clusters[], int centroid);

void  initCentroids(float MATRIZ [][MAX_DIM], int n, int m, float centroids[][MAX_DIM], int k);
float calcCentroid(float MATRIZ[][MAX_DIM], int m, int clusters[], int n, int pos);
void  calcCentroids(float MATRIZ[][MAX_DIM], int n, int m, float centroids[][MAX_DIM], int k);
void  copyMatrix(float a[][MAX_DIM], float b[][MAX_DIM], int n, int m);
int   getClosetsCentroid(float attributes[], int m, float centroids[][MAX_DIM], int n);
void  listCentroids(float centroids[][MAX_DIM], int n, int m);
float calculateDelta(float oldCentroids[][MAX_DIM], float centroids[][MAX_DIM], int k, int m);

void  sampleMean(float MATRIZ[][MAX_DIM], float mean[MAX_DIM], int n, int m);

void  kmeans(float MATRIZ[][MAX_DIM], int CLUSTERS[], int n, int m, int k, int e);
void  save(float MATRIZ[][MAX_DIM], int n, int m, char *filename);

#endif
