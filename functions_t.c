/*
    Additional functions especific for implementation with threads
*/

#include "general.h"
#include "functions_t.h"
#include "datasetMatrix.h"
#include "centroidsMatrix.h"


extern double **dataset;
extern int nLines;
extern int nDimensions;
extern int k;
extern double **centroids;
extern double **newCentroids;
extern int nThreads; 
extern int DEBUG;


int iterador = 0;
// semaforo de exclusao multipla
pthread_mutex_t sExMut;

int distant_entity;
//aux for average calculation
int *avg_aux;


/*
    funcao para inicializar os centroids utilizando metodo KATSAVOUNIDIS
    com threads a processar blocos do dataset
*/
void initCentroidsMatrix_t() {

    int local_nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[local_nThreads];
    // Inicializar o semaforo
    pthread_mutex_init(&sExMut, NULL);
    // Definir numero de entidades que cada thread vai processar (ceil)
    int blockSize = ( nLines / local_nThreads) + ((nLines % local_nThreads) != 0);
    // array de argumentos para threads
    int tArgs[local_nThreads][3];
    
    for (int c=0; c<k; c++) {
        distant_entity = 0;

        for (int i = 0; i < local_nThreads; i++) {
            tArgs[i][0] = i;
            tArgs[i][1] = blockSize;
            tArgs[i][2] = c;
            if (pthread_create(&tid[i], NULL, initCentroid_t, (void *)&tArgs[i]) != 0) {
                printf("%s\n", "Erro na criação de tarefa.");
                exit(1);
            }
            else {
                //printf("%s%d\n", "Criada a tarefa ", (int)tid[i]);
            }
        }
        for (int i=0; i<local_nThreads; i++) {
            pthread_join(tid[i], NULL);
        }

        // copy most distant entity values into centroid
        for (int j=0; j<nDimensions; j++) {
            *(*(newCentroids + c) + j) = *(*(dataset + distant_entity) + j);
        }
        //printf("Entidade: %d\n", distant_entity);
    }

    // Fechar o semaforo
    pthread_mutex_destroy(&sExMut);

}


void *initCentroid_t(void *param) {
    int positionForNorm = nDimensions;
    double minDistance, d;

    int *tArgs = (int *)param;
    int index=tArgs[0]; 
    int blockSize = tArgs[1];
    int cluster = tArgs[2];

    for (int i = index*blockSize; i < index*blockSize + blockSize && i < nLines; i++) {

        minDistance = FLT_MAX;
        for (int ic=0; ic < cluster; ic++) {
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
        pthread_mutex_lock(&sExMut);
        if (*(*(dataset + i) + positionForNorm) > *(*(dataset + distant_entity) + positionForNorm)) {
            distant_entity = i;
        }
        if (*(*(dataset + i) + positionForNorm) == *(*(dataset + distant_entity) + positionForNorm)
            && i < distant_entity) {
            distant_entity = i;
        }
        pthread_mutex_unlock(&sExMut);
    }
    return NULL;

}


/***************************************************************************/

void updateClusterAssociation_t() {

    int local_nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[local_nThreads];
    // Abrir o semaforo
    pthread_mutex_init(&sExMut, NULL);
    // Definir numero de entidades que cada thread vai processar (ceil)
    int blockSize = ( nLines / local_nThreads) + ((nLines % local_nThreads) != 0);
    // array de argumentos para threads
    int tArgs[local_nThreads][2];


    int average_aux[k];
    for (int i = 0; i < k; i++) {
        average_aux[i]=0;
    }
    avg_aux = average_aux;


    for (int i = 0; i < local_nThreads; i++) {
        tArgs[i][0] = i;
        tArgs[i][1] = blockSize;
        if (pthread_create(&tid[i], NULL, updateCluster_t, (void *)&tArgs[i]) != 0) {
            printf("%s\n", "Erro na criação de tarefa.");
            exit(1);
        }
        else {
            //printf("%s%d\n", "Criada a tarefa ", (int)tid[i]);
        }
    }

    for (int i=0; i<local_nThreads; i++) {
        pthread_join(tid[i], NULL);
    }

    for (int i=0; i<k; i++) {
        if (avg_aux[i]==0) {
            recalculateCentroid(i);
            continue;
        }
        for (int j=0; j<nDimensions; j++) {
            // calcula a media
            *(*(newCentroids + i) + j) /= avg_aux[i];
        }
    }

    // Fechar o semaforo
    pthread_mutex_destroy(&sExMut);
}

void *updateCluster_t(void *param) {
    int positionForClusters = nDimensions + 1;
    int cluster;

    int *tArgs = (int *)param;
    int index=tArgs[0]; 
    int blockSize = tArgs[1];


    double **tmpNewCentroids;
    int local_avg_aux[k];
    // alocate tmp centroid matriz for thread use
    tmpNewCentroids = (double **) malloc(sizeof (double *) * k);

    for (int i = 0; i < k; i++) {
        tmpNewCentroids[i] = (double *) malloc(sizeof (double) * nDimensions );
        for (int j=0; j<nDimensions; j++) {
            tmpNewCentroids[i][j] = 0;
        }
    }
    for (int j=0; j<k; j++) {
        local_avg_aux[j] = 0;
    }


    for (int i = index*blockSize; i < index*blockSize + blockSize && i < nLines; i++) {

        cluster = getClosetsCentroidPosition( *(dataset + i) );
        *(*(dataset + i) + positionForClusters) = cluster;

        for (int j=0; j<nDimensions; j++) {
            // soma os valores
            *(*(tmpNewCentroids + cluster) + j) +=  *(*(dataset + i) + j);
        }

        local_avg_aux[cluster] += 1;

    }
    
    pthread_mutex_lock(&sExMut);
    for (int i=0; i<k; i++) {
        for (int j=0; j<nDimensions; j++) {
            *(*(newCentroids + i) + j) += *(*(tmpNewCentroids + i) + j);
        }
    }
    for (int j=0; j<k; j++) {
        avg_aux[j] += local_avg_aux[j];
    }
    pthread_mutex_unlock(&sExMut);

    return NULL;
}
