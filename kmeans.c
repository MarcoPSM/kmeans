#include "kmeans.h"


void kmeans(float MATRIZ[][MAX_DIM], int CLUSTERS[], int n, int m, int k, int e) {
    /*iterador*/
    int t=0;
    /*lista de centroids*/
    float centroids[k];

    int pos;
    float aux;
    float delta;

    initCentroids(MATRIZ, n, m, centroids, k);
    listCentroids(centroids, k);

    do {
        t += 1;

        for(int i=0; i<k;i++) {
            initClusters(CLUSTERS, n);
        }

        /*atribuicao de clusters*/
        for (int i=0; i<n; i++) {
            pos = getClosetsCentroid(MATRIZ[i], m, centroids, k);
            CLUSTERS[i] = pos;
        }

        /*atualizar os centroids*/
        delta = 0;
        for(int i=0; i<k; i++) {
            aux = calcCentroid(MATRIZ, m, CLUSTERS, n, i);
            printf("d=%f aux=%f c=%f\n", delta, aux, centroids[i]);
            delta += (centroids[i] < aux) ? aux-centroids[i] : centroids[i] - aux;
            centroids[i] = aux;
        }
        
        printf("delta %f \n", delta);
    } while( delta > e );  
    printf("Iteracaoes: %d\n", t);  

            
    for(int i=0; i<k; i++) {
        printf("Listar cluster %d \n", i);
        listarCluster(MATRIZ, n, m, CLUSTERS, i);
        //printf("Media do cluster %d \n", media(&clusters[i]));
    }
}


