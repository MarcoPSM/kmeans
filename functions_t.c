/*
    Additional functions especific for implementation with threads
*/

#include "general.h"
#include "functions_t.h"

int iterador = 0;
// semaforo de exclusao multipla
pthread_mutex_t sExMut;

/*
    funcao para inicializar o vetor de centroids utilizando threads
*/
void initCentroids_t(float MATRIZ [][MAX_DIM], int n, int m, float centroids[][MAX_DIM], int k) {
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

	void *params[5];
	printf("%d | %d | %d\n", n,m,k);
	params[0] = (void **)MATRIZ;
    params[1] = &n;
    params[2] = &m;
    params[3] = (void **)centroids;
    params[4] = &k;

	for (int i = 0; i < nThreads; i++) {
		//if (pthread_create(&tid[i], 0, MATRIZ, n, m, initCentroid_t, centroids, k) != 0) {
		if (pthread_create(&tid[i], NULL, initCentroid_t, params) != 0) {
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

void *initCentroid_t(void *param) {
	printf("DENTRO DA FUNCAO DA THREAD...\n");
	int *nt;
	nt = (int *)(param+1);
	printf("nt=%d\n", *nt);

	int n = *(int *)(param+1);
	printf("n=%d\n", n);
	int m = *(int *)(param+2);
	printf("m=%d\n", m);
	float (*matrix)[m] = param;
	printf("...\n");
	float (*centroids)[m] = param+3;
	printf("...\n");
	int k = *(int *)(param+4);
	printf("k=%d\n", k);

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
		float matriz[n][m];
	    float *aux;
	    int quantiles = k+1;
	    float quantil = 1.0/ (float)quantiles * (float)(centroid+1);

	    
	    printf("%d\n", n);
	    printf("%d\n", m);
	    printf("%f\n", quantil);
        
        printf("Press Any Key to Continue\n");  
        getchar();  

		aux = NULL;//quantileFunction(matriz , n, m, quantil);

	    for (int j=0; j<m;j++) {
	        *(*(centroids + centroid) + j)=aux[j];
	    }
	}


}