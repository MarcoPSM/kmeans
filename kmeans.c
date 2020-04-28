#include "kmeans.h"


void kmeans(float MATRIZ[][MAX_DIM], int n, int m, int k, int e) {
    /*iterador*/
    int t=0;
    /*lista de centroids*/
    float centroids[k];
    /*lista de clusters*/
    /* Uma lista do tamanho do dataset, em cada posicao fica o luster da 
       entidade correspondente a essa posicao no dataset */
    int clusters[n];
    int pos;
    float aux;
    float delta;

    initCentroids(MATRIZ, n, m, centroids, k);
    listCentroids(centroids, k);

    do {
        t += 1;

        for(int i=0; i<k;i++) {
            initClusters(clusters, n);
        }

        /*atribuicao de clusters*/
        for (int i=0; i<n; i++) {
            pos = getClosetsCentroid(MATRIZ[i], m, centroids, k);
            clusters[i] = pos;
        }

        /*atualizar os centroids*/
        delta = 0;
        for(int i=0; i<k; i++) {
            aux = calcCentroid(MATRIZ, m, clusters, n, i);
            printf("d=%f aux=%f c=%f\n", delta, aux, centroids[i]);
            delta += (centroids[i] < aux) ? aux-centroids[i] : centroids[i] - aux;
            centroids[i] = aux;
        }
        
        printf("delta %f \n", delta);
    } while( delta > e );  
    printf("Iteracaoes: %d\n", t);  

            
    for(int i=0; i<k; i++) {
        printf("Listar cluster %d \n", i);
        listarCluster(MATRIZ, n, m, clusters, i);
        //printf("Media do cluster %d \n", media(&clusters[i]));
    }
}


