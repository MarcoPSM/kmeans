#include <stdio.h>
#include <stdlib.h>

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
    printf("%d \n", dataset->attribute);
    listar(dataset->next);
}

void media(DATASET *dataset) {
    if (dataset==NULL) {
        /*nao ha elementos*/
        return;
    }
    ENTITY *tmp = *dataset;
    int soma = 0;
    int i=0;
    while(tmp != NULL) {
        soma += (int) tmp->attribute;
        i++;
        tmp = tmp->next;
    }
    printf("%d \n", soma/i);
}

/************************************************************************************/

/* FUNCTIONS IMPLEMENTATION */

void initCentroids(int *centroids, int n) {
    int value = 2;
    for(int i=0; i<n; i++) {
        *centroids = value;
        centroids++;
        value *= 2;
    }
}

void kmeans(DATASET *d, int k, float e) {
    /*iterador*/
    int t=0;
    /*lista de centroids*/
    int centroids[k];
    /*lista de clusters*/
    int *clusters[k];

    /*****   VER A CENA DE TROCAR INTEIROS  *****/
    initCentroids(centroids, k);
    /*output dos centroids*/
    for(int i=0; i<k; i++) {
        printf("%d \n", &centroids);
        centroids++;
    }

    do {
        t += 1;
/*        inicializar_os_clusters(clusters[], k);
*/
        /*atribuicao de clusters*/
        for (int i=0; i<0 /*tamanho_do_d*/; i++) {
/*            associar_d[i]_ao_cluster_pela_distancia_ao_centroid();
            append_do_d[i]_a_lista_de_clusters();
*/
        }
        /*atualizar os centroids*/
        for(int i=0; i<k; i++) {
/*            centroi[i]=media_do_pontos_no_cluster[i];
*/
        }
        
    } while( t<e /*soma_distancias_dos_novos_centroids_aos_antigos < e*/ );    
    
}


/************************************************************************************/
/* MAIN FUNCTION */
int main(int argc, char *argv[]) {
    DATASET f;
    int k = 2;
    int e = 0;
    int testing = 0; // true=1 false=0
    
    if (testing) {
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
        puts("Calcular a media");
        media(&f);
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
    }

    return 0;

}


