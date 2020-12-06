/*
  Funtions related to centroids matrix
  Marco Martins marcopaulomartins@hotmail.com
  2020/08/16
*/
#include "centroidsMatrix.h"
#include "datasetMatrix.h"
#include "general.h"

extern float **centroids;
extern int k;
extern int nDimensions;
extern float **dataset;
extern int nLines;
extern float *totals;
extern int DEBUG;

float **oldCentroids;

void resetCentroids() {
    for (int i=0; i<k; i++) {
        for (int j=0; j<nDimensions; j++) {
            *(*(centroids + i) + j) = 0;
        }
    }
}

void alocateCentroids() {
    centroids = (float **) malloc(sizeof (float *) * k);
    for (int i = 0; i < k; i++) {
        centroids[i] = (float *) malloc(sizeof (float) * nDimensions);
    }
}
void alocateOldCentroids() {
    oldCentroids = (float **) malloc(sizeof (float *) * k);
    for (int i = 0; i < k; i++) {
        oldCentroids[i] = (float *) malloc(sizeof (float) * nDimensions);
    }
}

void initCentroidsMatrix() {
    int positionForNorm = nDimensions;
    int distant_entity;
    float minDistance, d;

    for (int c=0; c<k; c++) {
        distant_entity = 0;
        for (int i = 0; i < nLines; i++) {
            minDistance = FLT_MAX;
            for (int ic=0; ic < c; ic++) {
                d = distance(*(dataset + i), *(centroids + ic), nDimensions);
                if (d < minDistance){
                    minDistance = d;
                }
            }
            if (minDistance == -1) {
                *(*(dataset + i) + positionForNorm) = norm(*(dataset + i), nDimensions);
            }
            else {
                *(*(dataset + i) + positionForNorm) = minDistance;
            }
            if (*(*(dataset + i) + positionForNorm) > *(*(dataset + distant_entity) + positionForNorm)) {
                distant_entity = i;
            }
        }

        // copy most distant entity values into centroid
        for (int j=0; j<nDimensions; j++) {
            *(*(centroids + c) + j) = *(*(dataset + distant_entity) + j);
        }
        printf("Entidade: %d\n", distant_entity);
    }
}


void listCentroidsMatrix() {
    printf("LISTAR CENTROIDS MATRIX\n");
    for(int i=0; i<k; i++) {
        for(int j=0;j<nDimensions; j++) {
            //printf("%f ", dataset[i][j]);
            printf("%f ", *(*(centroids + i) + j));
        }
        //puts("\n");
        printf("\n");
    }
}

int getClosetsCentroidPosition(float *entity) {
    float minDistance;
    int colsestCentroid;
    float d;

    for(int i=0; i<k; i++) {
        d = distance(entity, *(centroids + i), nDimensions);

        if(i==0) {
            minDistance = d;
            colsestCentroid = i;
        } 
        else {
            if(d < minDistance) {
                minDistance = d;
                colsestCentroid = i;
            }
        }
    }
    return colsestCentroid;
}

void backupCentroids() {
    for (int i=0; i<k; i++) {
        for (int j=0; j<nDimensions; j++) {
            *(*(oldCentroids + i) + j) = *(*(centroids + i) + j);
        }
    }
}

void updateCentroids() {
    int cnt[k];
    initArray(cnt, k, 0);
    int positionForClusters = nDimensions + 1;

    resetCentroids();

    // Recalcular centroids fazendo media
    for (int i=0; i<nLines; i++) {
        for (int j=0; j<nDimensions; j++) {
        	// soma os valores
        	int line = (int) *(*(dataset + i) + positionForClusters);
        	*(*(centroids + line) + j) +=  *(*(dataset + i) + j);
        }
        // contabiliza quantos foram somadas em cada cluster
        cnt[(int) *(*(dataset + i) + positionForClusters)] ++;
    }
    for (int i=0; i<k; i++) {
        if (cnt[i]==0) {
            recalculateCentroid(i);
            continue;
        }
        for (int j=0; j<nDimensions; j++) {
        	// calcula a media
            *(*(centroids + i) + j) /= cnt[i];
        }
    }
    if (DEBUG == TRUE) {
        //debug
        //printf("-------------------\n");
        //listCentroidsMatrix();
        //printf("-------------------\n");
        for (int i=0; i<k; i++) {
            printf("Contador: %d ", cnt[i]);
        }
        printf("\n");
    }
}

void recalculateCentroid(int i) {
    float *aux;
    aux = getClosestEntity( *(oldCentroids + i) );
    if (DEBUG == TRUE) {
        for (int l = 0; l < nDimensions; l++) {
            printf("a%d: %f",l, *(aux + l));
        }
        printf("\n");
    }
    for (int j=0; j<nDimensions; j++) {
        *(*(centroids + i) + j) = *(aux + j);
    }
}

float getDelta() {
    float delta = 0;
    for(int i=0; i<k; i++) {
        if (DEBUG == TRUE) {
            printf("DISTANCE = %f ", distance(*(centroids + i), *(oldCentroids + i), nDimensions) );
        }
        delta += distance(*(centroids + i), *(oldCentroids + i), nDimensions);
    }
    if (DEBUG == TRUE) {
        printf("\n");
    }
    return delta;
}

void saveCentroids(char *filename) {
    FILE *fp=NULL;
    fp=fopen(filename, "w");
    for (int i=0; i<k; i++) {
        for (int j=0; j<nDimensions; j++) {
            fprintf(fp, "%f ", *(*(centroids + i) + j));
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return;
}
