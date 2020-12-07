#include "datasetMatrix.h"
#include "centroidsMatrix.h"

extern double **dataset;
extern int nLines;
extern int nColumns;
extern int nDimensions;
extern double *totals;
extern int k;
extern double **newCentroids;

void alocateTotals() {
    totals = (double*) malloc(sizeof(double) * nDimensions);
}

void loadTotals() {
    for(int j=0;j<nDimensions; j++) {
        *(totals + j) = 0.0;
    }

    for(int i=0; i<nLines; i++) {
        for(int j=0;j<nDimensions; j++) {
            *(totals + j) += *(*(dataset + i) + j);
        }
    }
}

void alocateDataset() {
    dataset = (double **) malloc(sizeof (double *) * nLines);
    for (int i = 0; i < nLines; i++) {
        dataset[i] = (double *) malloc(sizeof (double) * nColumns);
    }
    alocateTotals();
}

void loadDataset(char *filename) {
    FILE *datafile=NULL;
    datafile=fopen(filename, "r");
    for(int i=0; i<nLines; i++) {
        for(int j=0;j<nDimensions; j++) {
            if( !fscanf(datafile, "%lf", &dataset[i][j]) ) {
                break;
            }
        }
    }
    //printf("Numero= %f", attributes[0]);
    fclose(datafile);
    loadTotals();
}

void listDataset() {
    printf("LISTAR A MATRIZ DE POINTEIROS\n");
    for(int i=0; i<nLines; i++) {
        for(int j=0;j<nDimensions; j++) {
            //printf("%f ", dataset[i][j]);
            printf("%f ", *(*(dataset + i) + j));
        }
        //puts("\n");
        printf("\n");
    }
}

void listFullDatasetMatrix() {
    for(int i=0; i<nLines; i++) {
        for(int j=0;j<nColumns; j++) {
            //printf("%f ", dataset[i][j]);
            printf("%f ", *(*(dataset + i) + j));
        }
        //puts("\n");
        printf("\n");
    }
}

void calculateEntitiesNorm() {
    int positionForNorm = nDimensions;
    for(int i=0; i<nLines; i++) {
        *(*(dataset + i) + positionForNorm) = norm(*(dataset + i), nDimensions);
    }
}

double* getClosestEntity(double *centroid) {
    double minDistance;
    int colsestEntity;
    double d;
    for (int i = 0; i < nLines; i++) {
        d = distance( *(dataset + i), centroid, nDimensions);
        if(i==0) {
            minDistance = d;
            colsestEntity = i;
        } 
        else {
            if(d < minDistance) {
                minDistance = d;
                colsestEntity = i;
            }
        }
    }
    return *(dataset + colsestEntity);
}

void saveDataset(char *filename) {
    int positionForClusters = nDimensions + 1;

    FILE *fp=NULL;
    fp=fopen(filename, "w");
    for (int i=0; i<nLines; i++) {
        for (int j=0; j<nDimensions; j++) {
            fprintf(fp, "%f ", *(*(dataset + i) + j));
        }
        fprintf(fp, "%d\n", (int) *(*(dataset + i) + positionForClusters));
    }
    fclose(fp);
    return;
}

/*
    Inverse Cumulative Distribution Function or quantile function (pag.43)
    F^{−1} (q) = min{x | F (x) ≥ q}   for q ∈ [0, 1]
    
    Norm of all entities is registered in the penultimate position 

    Return entity array
*/
double* getDatasetQuantileEntity(double q) {
    double *x = NULL;
    int positionForNorm = nDimensions;
    int flag = FALSE;
    double lowNorm = -1;
    double highNorm = -1;

    for(int i=0; i<nLines; i++) {
        if (*(*(dataset + i) + positionForNorm) < lowNorm
            || ( x != NULL
                &&  *(*(dataset + i) + positionForNorm) > x[positionForNorm])) {
            continue;
        }

        if(cumulativeDistribution(*(*(dataset + i) + positionForNorm)) >= q) {
            // A primeira vez que apaha um aceita
            if (flag == FALSE) {
                x = *(dataset + i);
                flag = TRUE;
                highNorm = *(*(dataset + i) + positionForNorm);
            }
            else if (*(*(dataset + i) + positionForNorm) < x[positionForNorm]) {
                x = *(dataset + i);
                highNorm = *(*(dataset + i) + positionForNorm);
            }
        }
        else {
            if (*(*(dataset + i) + positionForNorm) > lowNorm) {
                lowNorm = *(*(dataset + i) + positionForNorm);
            }
        }
    }
    return x;
}

/*
    Empirical Cumulative Distribution Function
    using the dimensions vector norm saved in the penultimate position 
*/
double cumulativeDistribution(double norm) {
    //printf("===cumulativeDistribution===\n");
    int soma = 0;
    int positionForNorm = nDimensions;
    for(int i=0; i<nLines; i++) {
        if(*(*(dataset + i) + positionForNorm) <= norm) {
            soma++;
        }
    }
    //printf("Norma=%f Soma=%d n=%d CDF = %f \n", norm, soma, n, (double)soma / (double) n);
    return (double)soma / (double) nLines;
}

void updateClusterAssociation() {
    int cnt[k];
    initArray(cnt, k, 0);
    int positionForClusters = nDimensions + 1;
    int cluster;

    for (int i=0; i<nLines; i++) {
        cluster = getClosetsCentroidPosition( *(dataset + i) );
        *(*(dataset + i) + positionForClusters) = cluster;

        for (int j=0; j<nDimensions; j++) {
            // soma os valores
            *(*(newCentroids + cluster) + j) +=  *(*(dataset + i) + j);
        }
        // contabiliza quantos foram somadas em cada cluster
        cnt[cluster] ++;
    }

    for (int i=0; i<k; i++) {
        if (cnt[i]==0) {
            recalculateCentroid(i);
            continue;
        }
        //printf("C:%d T:%d\n", i, cnt[i]);
        for (int j=0; j<nDimensions; j++) {
            // calcula a media
            *(*(newCentroids + i) + j) /= cnt[i];
        }
    }
}