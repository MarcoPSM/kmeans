#include "kmeans.h"


void kmeans(float MATRIZ[][MAX_DIM], int CLUSTERS[], int n, int m, int k, int e) {
    /*iterador*/
    int t=0;
    /*lista de centroids*/
    float centroids[k][m];
    float oldCentroids[k][m];

    int pos;
    float aux;
    float delta;

    initCentroids(MATRIZ, n, m, centroids, k);
    //listCentroids(centroids, k, m);

    do {
        t++;

        /*atribuicao de clusters*/
        for (int i=0; i<n; i++) {
            pos = getClosetsCentroid(MATRIZ[i], m, centroids, k);
            MATRIZ[i][m+1]= pos;
        }

        //printf("centroids\n"); 
        //listarMatriz(centroids, k, m);


        /*atualizar os centroids*/
        copyMatrix(centroids, oldCentroids, k, m);
        calcCentroids(MATRIZ, n, m, centroids, k);

        delta = calculateDelta(oldCentroids, centroids, k, m);

        printf("delta %f t=%d\n", delta, t);
        //printf("Press Any Key to Continue\n");  
        //getchar();   
    } while( delta > e && t < MAX_ITERATIONS);  
    printf("Iteracaoes: %d\n", t);  

            
    for(int i=0; i<k; i++) {
        printf("Listar cluster %d \n", i);
        //listarCluster(MATRIZ, n, m, CLUSTERS, i);
        //printf("Media do cluster %d \n", media(&clusters[i]));
    }
}


