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
int nThreads; 


int iterador = 0;
// semaforo de exclusao multipla
pthread_mutex_t sExMut;

/*
    funcao para inicializar o vetor de centroids utilizando threads
*/
void initCentroidsMatrix_t() {

	if (iterador != 0) {
		printf("%s\n", "Iterador nao esta a zero. Isto nao devia acontecer...");
        exit(1);
	}

	nThreads = (k<nThreads) ? k : nThreads;
	// array para threads
	pthread_t tid[nThreads];
	// Abrir o semaforo
	pthread_mutex_init(&sExMut, NULL);

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
    iterador = 0;

}

void *initCentroid_t(void *param) {
	//printf("DENTRO DA FUNCAO DA THREAD...\n");

	int centroid;
    float *aux;
    float quantil;
    int quantiles = k+1;

	for (int i = 0; i < k; i++) {

		pthread_mutex_lock(&sExMut);
		if (iterador<k) {
			centroid = iterador;
			iterador++;
			pthread_mutex_unlock(&sExMut);
		} 
		else {
			pthread_mutex_unlock(&sExMut);
			break;
		}

	    quantil = 1.0/ (float)quantiles * (float)(centroid+1);

	    //printf("%f\n", quantil);

		aux = getDatasetQuantileEntity(quantil);

	    for (int j=0; j<nDimensions;j++) {
	        *(*(centroids + centroid) + j)=aux[j];
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