/*
    Headers file for additional functions for implementation with threads
*/
#ifndef KMEANS_T_H
#define KMEANS_T_H

#include <pthread.h>

#define MAX_THREADS 50


void initCentroidsMatrix_t();
void *setBottomTopDimensions_t(void *param);
void *setBottomTopDimensions_v2_t(void *param);
void *initCentroid_t(void *param);
void *initCentroid_v2_t(void *param);
void *initCentroid_v3_t(void *param);
void updateClusterAssociation_t();
void updateClusterAssociation_v2_t();
void *updateCluster_t(void *param);
void *updateCluster_v2_t(void *param);

#endif
