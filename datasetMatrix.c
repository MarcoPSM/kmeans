#include "datasetMatrix.h"
#include "centroidsMatrix.h"

extern float **dataset;
extern int nLines;
extern int nColumns;
extern int nDimensions;

void alocateDataset() {
    dataset = (float **) malloc(sizeof (float *) * nLines);
    for (int i = 0; i < nLines; i++) {
        dataset[i] = (float *) malloc(sizeof (float) * nColumns);
    }
}

void loadDataset(char *filename) {
    FILE *datafile=NULL;
    datafile=fopen(filename, "r");
    for(int i=0; i<nLines; i++) {
        for(int j=0;j<nDimensions; j++) {
            if( !fscanf(datafile, "%f", &dataset[i][j]) ) {
                break;
            }
        }
    }
    //printf("Numero= %f", attributes[0]);
    fclose(datafile);
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
float* getDatasetQuantileEntity(float q) {
    float *x;
    int positionForNorm = nDimensions;
    x = *(dataset);
    int flag = FALSE;
    for(int i=0; i<nLines; i++) {
        if(cumulativeDistribution(*(*(dataset + i) + positionForNorm)) >= q) {
            if (flag == FALSE) {
                x = *(dataset + i);
                flag = TRUE;
            }
            else if (*(*(dataset + i) + positionForNorm) < x[positionForNorm] ) {
                x = *(dataset + i);
            }
        }
    }
    return x;
}

/*
    Empirical Cumulative Distribution Function
    using the dimensions vector norm saved in the penultimate position 
*/
float cumulativeDistribution(float norm) {
    int soma = 0;
    int positionForNorm = nDimensions;
    for(int i=0; i<nLines; i++) {
        if(*(*(dataset + i) + positionForNorm) <= norm) {
            soma++;
        }
    }
    //printf("Norma=%f Soma=%d n=%d CDF = %f \n", norm, soma, n, (float)soma / (float) n);
    return (float)soma / (float) nLines;
}

void updateClusterAssociation() {
    int positionForClusters = nDimensions + 1;
    int cluster;
    for (int i=0; i<nLines; i++) {
        cluster = getClosetsCentroidPosition( *(dataset + i) );
        *(*(dataset + i) + positionForClusters) = cluster;
    }
}