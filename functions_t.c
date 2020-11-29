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



int iterador = 0;
// semaforo de exclusao multipla
pthread_mutex_t sExMut;
// variaveis para a inicializacao dos centroids
float *top;
float *bottom;
float *steps;

/*
    funcao para inicializar o vetor de centroids utilizando threads
*/
void initCentroidsMatrix_t() {
    float t[nDimensions];
    float b[nDimensions];
    float s[nDimensions];
    top = t;
    bottom = b;
    steps = s;

    printf("initCentroidsMatrix_t\n");
    for (int i=0; i<nDimensions; i++) {
        printf("%f ", top[i]);
    }
    printf("\n");

	if (iterador != 0) {
		printf("%s\n", "Iterador nao esta a zero. Isto nao devia acontecer...");
        exit(1);
	}

    // Definir min e max valores para cada dimensao

    nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[nThreads];
    // Inicializar o semaforo
    pthread_mutex_init(&sExMut, NULL);

    for (int i = 0; i < nThreads; i++) {
        if (pthread_create(&tid[i], NULL, setBottomTopDimensions_t, NULL) != 0) {
            printf("%s\n", "Erro na criação de tarefa.");
            exit(1);
        }
        else {
            //printf("%s%d\n", "Criada a tarefa ", (int)tid[i]);
        }
    }

    for (int i=0; i<nThreads; i++) {
        pthread_join(tid[i], NULL);
    }
    // repor o iterador
    iterador = 0;

    // Definir o step
    for (int j = 0; j < nDimensions; j++) {
        steps[j] = (top[j] - bottom[j]) / (k+1);
    }

	nThreads = (k<nThreads) ? k : nThreads;

	for (int i = 0; i < nThreads; i++) {
		if (pthread_create(&tid[i], NULL, initCentroid_t, NULL) != 0) {
            printf("%s\n", "Erro na criação de tarefa.");
            exit(1);
        }
        else {
            //printf("%s%d\n", "Criada a tarefa ", (int)tid[i]);
        }
	}

    for (int i=0; i<nThreads; i++) {
        pthread_join(tid[i], NULL);
    }
    // Fechar o semaforo
	pthread_mutex_destroy(&sExMut);
    // repor o iterador
    iterador = 0;

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
/***************************************************************************/

void updateClusterAssociation_t() {

    if (iterador != 0) {
        printf("%s\n", "Iterador nao esta a zero. Isto nao devia acontecer...");
        exit(1);
    }

    nThreads = (nLines < nThreads) ? nLines : nThreads;
    // array para threads
    pthread_t tid[nThreads];
    // Abrir o semaforo
    pthread_mutex_init(&sExMut, NULL);

    for (int i = 0; i < nThreads; i++) {
        if (pthread_create(&tid[i], NULL, updateCluster_t, NULL) != 0) {
            printf("%s\n", "Erro na criação de tarefa.");
            exit(1);
        }
        else {
            //printf("%s%d\n", "Criada a tarefa ", (int)tid[i]);
        }
    }

    for (int i=0; i<nThreads; i++) {
        pthread_join(tid[i], NULL);
    }
    // Fechar o semaforo
    pthread_mutex_destroy(&sExMut);
    iterador = 0;

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