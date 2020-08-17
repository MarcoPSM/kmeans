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

float **oldCentroids;

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
    float *aux;
    int quantiles = k+1;
    float quantil;

    for (int i=0; i<k; i++) {
    	quantil = (1.0/quantiles) * (i+1);
        printf("quantil=%f\n", quantil);
        aux = getDatasetQuantileEntity(quantil);

        for (int j=0; j<nDimensions;j++) {
            *(*(centroids + i) + j) = aux[j];
        }
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

    //reset centroids
    for (int i=0; i<k; i++) {
        for (int j=0; j<nDimensions; j++) {
            *(*(centroids + i) + j) = 0;
        }
    }
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
        for (int j=0; j<nDimensions; j++) {
        	// calcula a media
            *(*(centroids + i) + j) /= cnt[i];
        }
    }
}

float getDelta() {
    float delta = 0;

    for(int i=0; i<k; i++) {
    	printf("DISTANCE = %f\n", distance(*(centroids + i), *(oldCentroids + i), nDimensions) );
        delta += distance(*(centroids + i), *(oldCentroids + i), nDimensions);
    }
    return delta;
}