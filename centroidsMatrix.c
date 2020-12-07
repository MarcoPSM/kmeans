/*
  Funtions related to centroids matrix
  Marco Martins marcopaulomartins@hotmail.com
  2020/08/16
*/
#include "centroidsMatrix.h"
#include "datasetMatrix.h"
#include "general.h"

extern double **centroids;
extern int k;
extern int nDimensions;
extern double **dataset;
extern int nLines;
extern double *totals;
extern int DEBUG;

double **newCentroids;

void resetNewCentroids() {
    for (int i=0; i<k; i++) {
        for (int j=0; j<nDimensions; j++) {
            *(*(newCentroids + i) + j) = 0;
        }
    }
}

void alocateCentroids() {
    centroids = (double **) malloc(sizeof (double *) * k);
    for (int i = 0; i < k; i++) {
        centroids[i] = (double *) malloc(sizeof (double) * nDimensions);
    }
}
void alocateNewCentroids() {
    newCentroids = (double **) malloc(sizeof (double *) * k);
    for (int i = 0; i < k; i++) {
        newCentroids[i] = (double *) malloc(sizeof (double) * nDimensions);
    }
}

void copyCentroidsFromNewCentroids() {
    for (int i=0; i<k; i++) {
        for (int j=0; j<nDimensions; j++) {
            *(*(centroids + i) + j) = *(*(newCentroids + i) + j);
        }
    }
}

void initCentroidsMatrix() {
    int positionForNorm = nDimensions;
    int distant_entity;
    double minDistance, d;

    for (int c=0; c<k; c++) {
        distant_entity = 0;
        for (int i = 0; i < nLines; i++) {
            minDistance = FLT_MAX;
            for (int ic=0; ic < c; ic++) {
                d = distance(*(dataset + i), *(newCentroids + ic), nDimensions);
                if (d < minDistance){
                    minDistance = d;
                }
            }

            if (minDistance == FLT_MAX) {
                *(*(dataset + i) + positionForNorm) = norm(*(dataset + i), nDimensions);
            }
            else {
                *(*(dataset + i) + positionForNorm) = minDistance;
            }

            if (*(*(dataset + i) + positionForNorm) > *(*(dataset + distant_entity) + positionForNorm)) {
                distant_entity = i;
            }
            if (*(*(dataset + distant_entity) + positionForNorm) < 0)
            {
                printf("NUNCA PODE ENTRAR AQUI:::\n");
            }
        }

        // copy most distant entity values into centroid
        for (int j=0; j<nDimensions; j++) {
            *(*(newCentroids + c) + j) = *(*(dataset + distant_entity) + j);
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

int getClosetsCentroidPosition(double *entity) {
    double minDistance;
    int colsestCentroid;
    double d;

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


void updateCentroids() {
    int cnt[k];
    initArray(cnt, k, 0);
    int positionForClusters = nDimensions + 1;

    //resetCentroids();

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
    double *aux;
    aux = getClosestEntity( *(centroids + i) );

    for (int j=0; j<nDimensions; j++) {
        *(*(newCentroids + i) + j) = *(aux + j);
    }
}

double getDelta() {
    double delta = 0;
    for(int i=0; i<k; i++) {
        if (DEBUG == TRUE) {
            printf("DISTANCE = %f ", distance(*(centroids + i), *(newCentroids + i), nDimensions) );
        }
        delta += distance(*(centroids + i), *(newCentroids + i), nDimensions);
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
