/*
    Headers file for additional functions for implementation with threads
*/
#ifndef KMEANS_T_H
#define KMEANS_T_H

#include <pthread.h>

#define MAX_THREADS 5




void initCentroids_t(float MATRIZ [][MAX_DIM], int n, int m, float centroids[][MAX_DIM], int k);
void *initCentroid_t(void *param);

#endif