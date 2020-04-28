#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_DIM 3

int getNumberOfEntities(char *filename);
int getNumberOfDimensions(char *filename);
void loadMatrizFromFile(float MATRIZ[][MAX_DIM], int n, int m, char *filename);
void listarMatriz(float MATRIZ[][MAX_DIM], int n, int m);
void listarArray(float a[], int n);
void initClusters(int a[], int n);
void listarCluster(float MATRIZ[][MAX_DIM], int n, int m, int clusters[], int centroid);
void initCentroids(float MATRIZ [][MAX_DIM], int n, int m, float centroids[], int k);
float calcCentroid(float MATRIZ[][MAX_DIM], int m, int clusters[], int n, int pos);
int getClosetsCentroid(float attributes[], int m, float centroids[], int n);
void listCentroids(float centroids[], int n);

void sampleMean(float MATRIZ[][MAX_DIM], float mean[MAX_DIM], int n, int m);

void kmeans(float MATRIZ[][MAX_DIM], int n, int m, int k, int e);

#endif
