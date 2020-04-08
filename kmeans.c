#include <stdio.h>
#include <stdlib.h>

/*
    cc kmeans.c -o kmeans
*/

/************************************************************************************/
/* DATA STRUCT */

typedef struct Entity {
    int attribute;
    struct Entity *next;
} ENTITY; 

typedef ENTITY *DATASET;


/* FUNCTIONS IMPLEMENTATION */

/*
    Iniciar DATASET
*/
void inic(DATASET *dataset) {
    *dataset=NULL;
}

/*
    Insere um novo registo no fim da dataset
*/
void inserir(DATASET *dataset, int value) {
    if(*dataset==NULL) {
        *dataset=(DATASET)malloc(sizeof(ENTITY));
        if(*dataset==NULL) {
            return;
        }
        (*dataset)->attribute=value;
        (**dataset).next=NULL;
    }
    else {
        inserir(& (**dataset).next, value);
    }
}

/*
    Apagar o primeiro elemento da dataset
*/
void apagar(DATASET *dataset) {
    ENTITY *tmp = *dataset;
    if(*dataset==NULL) {
        /*nao ha elementos*/
        return;
    }
    *dataset=(*dataset)->next;
    free(tmp);
}

/*
    Listar todos os elementos de forma recursiva
*/
void listar(DATASET dataset) {
    if (dataset==NULL) {
        /*nao ha elementos*/
        return;
    }
    printf("Valor: %d \n", dataset->attribute);
    listar(dataset->next);
}

int media(DATASET *dataset) {
    if (dataset==NULL) {
        /*nao ha elementos*/
        return 0;
    }
    ENTITY *tmp = *dataset;
    int soma = 0;
    int i=0;
    while(tmp != NULL) {
        soma += (int) tmp->attribute;
        i++;
        tmp = tmp->next;
    }
    return soma/i;
}

/************************************************************************************/

/* FUNCTIONS IMPLEMENTATION */

/*
    funcao para inicializar o vetor de centroids
*/
void initCentroids(int *centroids, int n) {
    int value = 2;
    for(int i=0; i<n; i++) {
        centroids[i] = value;
        value *= 2;
    }
}

/*
    recebe o atributo da entidade, o vetor de centroides, o tamanho desse vetor
    Devolve a posicao do centroid mais proximo
*/
int getClosetsCentroid(int attribute, int centroids[], int n) {
    int minDistance=-1;
    int colsestCentroid;
    int distance;
    for(int i=0; i<n; i++) {
        distance = (attribute<centroids[i]) ? centroids[i] - attribute : attribute - centroids[i];
        if(minDistance==-1) {
            minDistance = distance;
            colsestCentroid = i;
        } 
        else {
            if(distance < minDistance) {
                minDistance = distance;
                colsestCentroid = i;
            }
        }
    }
    return colsestCentroid;
}

void kmeans(DATASET *dataset, int k, int e) {
    /*iterador*/
    int t=0;
    /*lista de centroids*/
    int centroids[k];
    /*lista de clusters*/
    DATASET clusters[k];
    int aux;
    int soma_dist_novos_centros_aos_antigos;

    /*****   VER A CENA DE TROCAR INTEIROS  *****/
    initCentroids(centroids, k);
    /*output dos centroids*/
    for(int i=0; i<k; i++) {
        printf("%d \n", centroids[i]);
    }

    do {
        t += 1;

        for(int i=0; i<k;i++) {
            inic(&clusters[i]);
        }

        /*atribuicao de clusters*/
        ENTITY *tmp = *dataset;
        while(tmp != NULL) {
            printf("Valor: %d \n", tmp->attribute);
            aux = getClosetsCentroid(tmp->attribute, centroids, k);
            inserir(&clusters[aux], tmp->attribute);
            printf("cluster: %d \n", aux);

            tmp=tmp->next;
        }

        /*atualizar os centroids*/
        soma_dist_novos_centros_aos_antigos = 0;
        for(int i=0; i<k; i++) {
            printf("Listar cluster %d \n", i);
            listar(clusters[i]);
            aux = media(&clusters[i]);
            printf("Media do cluster %d \n", aux);
            soma_dist_novos_centros_aos_antigos += (centroids[i] < aux) ? aux-centroids[i] : centroids[i] - aux;
            centroids[i] = aux;
        }
        
        printf("soma_dist_novos_centros_aos_antigos %d \n", soma_dist_novos_centros_aos_antigos);
    } while( soma_dist_novos_centros_aos_antigos > e );    
    
}


/************************************************************************************/
/* MAIN FUNCTION */
int main(int argc, char *argv[]) {
    DATASET f;
    int k = 2;
    int e = 0;
    int testing = 0; // true=1 false=0
    
    inic(&f);
    inserir(&f, 2);
    inserir(&f, 3);
    inserir(&f, 4);
    inserir(&f, 10);
    inserir(&f, 11);
    inserir(&f, 12);
    inserir(&f, 20);
    inserir(&f, 25);
    inserir(&f, 30);
    puts("Listar todos");
    listar(f);
    printf("A media do dataset é: %d \n", media(&f));
    
    if (testing) {
        apagar(&f);
        puts("Listar menos 1");
        listar(f);
        apagar(&f);
        puts("Listar menos 2");
        listar(f);
        apagar(&f);
    }
    else {
        kmeans(&f, k, e);
        puts("Listar todos");
        listar(f);
    }

    return 0;

}


