/*
    Additional functions especific for implementation with threads
*/

#include "general.h"
#include "functions_t.h"
#include "datasetMatrix.h"
#include "centroidsMatrix.h"
#include <float.h>

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

/*
    funcao para inicializar o vetor de centroids utilizando threads
    Metodo 1 = Iterador
*/
void initCentroidsMatrix_t() {
    float t[nDimensions];
    float b[nDimensions];
    float s[nDimensions];
    top = t;
    bottom = b;
    steps = s;
    if (DEBUG == TRUE) {
        printf("initCentroidsMatrix_t\n");
        for (int i=0; i<nDimensions; i++) {
            printf("%f ", top[i]);
        }
        printf("\n");
    }
    
	if (iterador != 0) {
		printf("%s\n", "Iterador nao esta a zero. Isto nao devia acontecer...");
        exit(1);
	}

    // Definir min e max valores para cada dimensao

    int local_nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[local_nThreads];
    // Inicializar o semaforo
    pthread_mutex_init(&sExMut, NULL);

    for (int i = 0; i < local_nThreads; i++) {
        if (pthread_create(&tid[i], NULL, setBottomTopDimensions_t, NULL) != 0) {
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
    // repor o iterador
    iterador = 0;

    // Definir o step
    for (int j = 0; j < nDimensions; j++) {
        steps[j] = (top[j] - bottom[j]) / (k+1);
        if (DEBUG == TRUE) {
            printf("Step:%f ", steps[j]);
        }
    }
    if (DEBUG == TRUE) {
        printf("\n");
    }

	local_nThreads = (k<nThreads) ? k : nThreads;

	for (int i = 0; i < local_nThreads; i++) {
		if (pthread_create(&tid[i], NULL, initCentroid_t, NULL) != 0) {
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
    // repor o iterador
    iterador = 0;

}


/*
    funcao para inicializar o vetor de centroids utilizando threads
    Metodo 2 = Em bloco
*/
void initCentroidsMatrix_v2_t() {
    if (DEBUG == TRUE) {
        printf("initCentroidsMatrix_v2_t\n");
    }
    float t[nDimensions];
    float b[nDimensions];
    float s[nDimensions];

    for (int i=0; i<nDimensions; i++) {
        t[i]=-FLT_MAX;
        b[i]=FLT_MAX;
    }

    top = t;
    bottom = b;
    steps = s;

    // Definir min e max valores para cada dimensao

    int local_nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[local_nThreads];
    // Inicializar o semaforo
    pthread_mutex_init(&sExMut, NULL);
    // Definir numero de entidades que cada thread vai processar (ceil)
    int blockSize = ( nLines / local_nThreads) + ((nLines % local_nThreads) != 0);
    int tArgs[local_nThreads][2];

    for (int i = 0; i < local_nThreads; i++) {
        tArgs[i][0] = i;
        tArgs[i][1] = blockSize;
        if (pthread_create(&tid[i], NULL, setBottomTopDimensions_v2_t, (void *)&tArgs[i]) != 0) {
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

    // Definir o step
    for (int j = 0; j < nDimensions; j++) {
        steps[j] = (top[j] - bottom[j]) / (k+1);
        if (DEBUG == TRUE) {
            printf("Step:%f ", steps[j]);
        }
    }
    if (DEBUG == TRUE) {
        printf("\n");
    }

    local_nThreads = (k<nThreads) ? k : nThreads;
   // Definir numero de clusters que cada thread vai processar (ceil)
    blockSize = ( k / local_nThreads) + ((k % local_nThreads) != 0);
    if (DEBUG == TRUE) {
        printf("blockSize: %d\n", blockSize);
    }
    int tArgsK[local_nThreads][2];

    for (int i = 0; i < local_nThreads; i++) {
        tArgsK[i][0] = i;
        tArgsK[i][1] = blockSize;
        if (pthread_create(&tid[i], NULL, initCentroid_v2_t, (void *)tArgsK[i]) != 0) {
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


void *setBottomTopDimensions_t(void *param) {
    //printf("DENTRO DA FUNCAO DA THREAD...\n");

    int index;

    for (int i = 0; i < nLines; i++) {

        pthread_mutex_lock(&sExMut);
        if (iterador < nLines) {
            index = iterador;
            iterador++;
            pthread_mutex_unlock(&sExMut);
        } 
        else {
            pthread_mutex_unlock(&sExMut);
            break;
        }

        if (index==0) {
            for (int j = 0; j < nDimensions; j++) {
                top[j] = *(*(dataset + index) + j);
                bottom[j] = *(*(dataset + index) + j);
            }
        }
        else {
            for (int j = 0; j < nDimensions; j++) {
                if (top[j] < *(*(dataset + index) + j) ) {
                    top[j] = *(*(dataset + index) + j);
                }
                if (bottom[j] > *(*(dataset + index) + j) ) {
                    bottom[j] = *(*(dataset + index) + j);
                }
            }
        }

    }
    return NULL;

}

void *setBottomTopDimensions_v2_t(void *param) {
    //Desta forma as threads trabalham em bloco 
    //e nao pelo iterador
    int *tArgs = (int *)param;
    int index=tArgs[0]; 
    int blockSize = tArgs[1];
    //printf("#I:%d B:%d\n", index, blockSize);

    float localTop[nDimensions];
    float localBottom[nDimensions];

    for (int i = index*blockSize; i < index*blockSize + blockSize && i<nLines; i++) {

        if (i==index*blockSize) {
            for (int j = 0; j < nDimensions; j++) {
                localTop[j] = *(*(dataset + i) + j);
                localBottom[j] = *(*(dataset + i) + j);
            }
        }
        else {
            for (int j = 0; j < nDimensions; j++) {
                if (localTop[j] < *(*(dataset + i) + j) ) {
                    localTop[j] = *(*(dataset + i) + j);
                }
                if (localBottom[j] > *(*(dataset + i) + j) ) {
                    localBottom[j] = *(*(dataset + i) + j);
                }
            }
        }

    }

    pthread_mutex_lock(&sExMut);
    for (int j = 0; j < nDimensions; j++) {
        if (localTop[j] > top[j] ) {
            top[j] = localTop[j];
        }
        if (localBottom[j] < bottom[j]) {
            bottom[j] = localBottom[j];
        }
    }
    pthread_mutex_unlock(&sExMut);
    
    return NULL;

}


void *initCentroid_t(void *param) {
	//printf("DENTRO DA FUNCAO DA THREAD...\n");

	int centroid_pos;

	for (int i = 0; i < k; i++) {

		pthread_mutex_lock(&sExMut);
		if (iterador<k) {
			centroid_pos = iterador;
			iterador++;
			pthread_mutex_unlock(&sExMut);
		} 
		else {
			pthread_mutex_unlock(&sExMut);
			break;
		}

	    for (int j=0; j<nDimensions;j++) {
	        *(*(centroids + centroid_pos) + j) = bottom[j] + steps[j] * (centroid_pos+1);
	    }
	}
    return NULL;

}

void *initCentroid_v2_t(void *param) {
    //printf("DENTRO DA FUNCAO DA THREAD...initCentroid_v2_t\n");

    int *tArgs = (int *)param;
    int index=tArgs[0]; 
    int blockSize = tArgs[1];
    //printf("I:%d B:%d K:%d\n", index, blockSize, k);
    for (int i = index*blockSize; i < index*blockSize + blockSize && i < k; i++) {

        for (int j=0; j<nDimensions;j++) {
            //printf("I:%d J:%d B:%f S:%f\n", i,j,bottom[j], steps[j]);
            *(*(centroids + i) + j) = bottom[j] + steps[j] * (i+1);
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
