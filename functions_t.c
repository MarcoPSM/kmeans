/*
    Additional functions especific for implementation with threads
*/


#include "kmeans_t.h"

int iterador = 0;
// semaforo de exclusao multipla
pthread_mutex_t sExMut;

/*
    funcao para inicializar o vetor de centroids utilizando threads
*/
void initCentroids(float MATRIZ [][MAX_DIM], int n, int m, float centroids[][MAX_DIM], int k) {
	int nThreads;
	//pthread_t tid[MAX_THREADS];

	if (iterador != 0) {
		printf("%s\n", "Iterador nao esta a zero. Isto nao devia acontecer...");
        exit(1);
	}

	nThreads = (k<MAX_THREADS) ? k : MAX_THREADS;
	// array para threads
	pthread_t tid[nThreads];
	// Abrir o semaforo
	pthread_mutex_init(&sExMut, NULL);

	for (int i = 0; i < nThreads; i++) {
		if (pthread_create(&tid[i], 0, MATRIZ, n, m, initCentroid, centroids, k) != 0) {
            printf("%s\n", "Erro na criação de tarefa.");
            exit(1);
        }
        else {
            printf("%s%d\n", "Criada a tarefa ", (int)tid[i]);
        }
	}

    for (int i=0; i<nThreads; i++) {
        pthread_join(tid[i], NULL);
    }
    // Fechar o semaforo
	pthread_mutex_destroy(&sExMut);
    iterador = 0;

}

void initCentroid(float MATRIZ [][MAX_DIM], int n, int m, float centroids[][MAX_DIM], int k) {
	int centroid;
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

	    float *aux;
	    int quantiles = k+1;
	    float quantil = 1.0/quantiles * (centroid+1);
	    aux = quantileFunction(MATRIZ, n, m, quantil);
	    for (int j=0; j<m;j++) {
	        centroids[centroid][j]=aux[j];
	    }
	}
}