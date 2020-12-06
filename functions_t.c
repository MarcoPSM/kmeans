/*
    Additional functions especific for implementation with threads
*/

#include "general.h"
#include "functions_t.h"
#include "datasetMatrix.h"
#include "centroidsMatrix.h"


extern float **dataset;
extern int nLines;
extern int nDimensions;
extern int k;
extern float **centroids;
extern int nThreads; 
extern int DEBUG;


int iterador = 0;
// semaforo de exclusao multipla
pthread_mutex_t sExMut;
// variaveis para a inicializacao dos centroids
float *top;
float *bottom;
float *steps;

int distant_entity;

/*
    funcao para inicializar os centroids utilizando metodo KATSAVOUNIDIS
    Em bloco
*/
void initCentroidsMatrix_t() {

    int local_nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[local_nThreads];
    // Inicializar o semaforo
    pthread_mutex_init(&sExMut, NULL);
    // Definir numero de entidades que cada thread vai processar (ceil)
    int blockSize = ( nLines / local_nThreads) + ((nLines % local_nThreads) != 0);
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
            *(*(centroids + c) + j) = *(*(dataset + distant_entity) + j);
        }
        printf("Entidade: %d\n", distant_entity);
    }

    // Fechar o semaforo
    pthread_mutex_destroy(&sExMut);

}


void *initCentroid_t(void *param) {
    //printf("DENTRO DA FUNCAO DA THREAD...initCentroid_t\n");
    int positionForNorm = nDimensions;
    float minDistance, d;

    int *tArgs = (int *)param;
    int index=tArgs[0]; 
    int blockSize = tArgs[1];
    int cluster = tArgs[2];
    //printf("I:%d B:%d K:%d\n", index, blockSize, k);

    for (int i = index*blockSize; i < index*blockSize + blockSize && i < nLines; i++) {

            minDistance = FLT_MAX;
            for (int ic=0; ic < cluster; ic++) {
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
                pthread_mutex_lock(&sExMut);
                if (*(*(dataset + i) + positionForNorm) > *(*(dataset + distant_entity) + positionForNorm)) {
                    distant_entity = i;
                }
                pthread_mutex_unlock(&sExMut);
            }
    }
    return NULL;

}


/***************************************************************************/

void updateClusterAssociation_t() {

    if (iterador != 0) {
        printf("%s\n", "Iterador nao esta a zero. Isto nao devia acontecer...");
        exit(1);
    }

    int local_nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[local_nThreads];
    // Abrir o semaforo
    pthread_mutex_init(&sExMut, NULL);

    for (int i = 0; i < local_nThreads; i++) {
        if (pthread_create(&tid[i], NULL, updateCluster_t, NULL) != 0) {
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
    // Fechar o semaforo
    pthread_mutex_destroy(&sExMut);
    iterador = 0;

}

void updateClusterAssociation_v2_t() {

    if (iterador != 0) {
        printf("%s\n", "Iterador nao esta a zero. Isto nao devia acontecer...");
        exit(1);
    }

    int local_nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[local_nThreads];
    // Abrir o semaforo
    pthread_mutex_init(&sExMut, NULL);
    // Definir numero de entidades que cada thread vai processar (ceil)
    int blockSize = ( nLines / local_nThreads) + ((nLines % local_nThreads) != 0);
    int tArgs[local_nThreads][2];

    for (int i = 0; i < local_nThreads; i++) {
        tArgs[i][0] = i;
        tArgs[i][1] = blockSize;
        if (pthread_create(&tid[i], NULL, updateCluster_v2_t, (void *)&tArgs[i]) != 0) {
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
    // Fechar o semaforo
    pthread_mutex_destroy(&sExMut);


}

void *updateCluster_t(void *param) {
    int index;
    int positionForClusters = nDimensions + 1;
    int cluster;

    for (int i = 0; i < nLines; i++) {

        pthread_mutex_lock(&sExMut);
        if (iterador<nLines) {
            index = iterador;
            iterador++;
            pthread_mutex_unlock(&sExMut);
        } 
        else {
            pthread_mutex_unlock(&sExMut);
            break;
        }

        cluster = getClosetsCentroidPosition( *(dataset + index) );
        //printf("Entidade %d CLUSTER = %d\n", index, cluster);
        *(*(dataset + index) + positionForClusters) = cluster;

    }
    return NULL;
}

void *updateCluster_v2_t(void *param) {
    int positionForClusters = nDimensions + 1;
    int cluster;

    int *tArgs = (int *)param;
    int index=tArgs[0]; 
    int blockSize = tArgs[1];
    //printf("I:%d B:%d K:%d\n", index, blockSize, k);
    for (int i = index*blockSize; i < index*blockSize + blockSize && i < nLines; i++) {

        cluster = getClosetsCentroidPosition( *(dataset + i) );
        //printf("Entidade %d CLUSTER = %d\n", index, cluster);
        *(*(dataset + i) + positionForClusters) = cluster;

    }
    return NULL;
}
