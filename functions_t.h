/*
    Headers file for additional functions for implementation with threads
*/
#ifndef KMEANS_T_H
#define KMEANS_T_H

#include <pthread.h>

#define MAX_THREADS 50


void initCentroidsMatrix_t();
void *initCentroid_t(void *param);
void updateClusterAssociation_t();
void *updateCluster_t(void *param);
void setNewCluster_t();

#endif
