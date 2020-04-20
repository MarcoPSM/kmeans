/******************************************
 * Marco Martins                          *
 * projeto final de curso Eng.Informática *
 * algoritmo na pag. 344                  *
 ******************************************/

/*
TAREFAS PARA 3-Feira:
    Implementar todas as funcoes da analise univariavel
    escrever o output num ficheiro
    mostrar um grafico com octave
    O dicheiro de entrada tem de entrar por argumento
*/

#include "dataset.h"

/* 
https://www.tutorialspoint.com/c_standard_library/limits_h.htm 
*/


/************************************************************************************/
/* HEADERS */

void initCentroids(DATASET *dataset, int *centroids, int n);
void listCentroids(int *centroids, int n);
int getClosetsCentroid(int attribute, int centroids[], int n);
void kmeans(DATASET *dataset, int k, int e);

/************************************************************************************/
/* MAIN FUNCTION */
int main(int argc, char *argv[]) {
    DATASET f;
    int k = 2;
    int e = 0;

    if (argc<2) {
        printf("Sintaxe: \n%s <nome_ficheiro>\n", argv[0]);
        exit(1);
    }

    inic(&f);    
    loadDatasetFromFile(&f, argv[1]);
//    loadDatasetFromFile(&f, "datafileA.txt");
//    loadDatasetFromFile(&f, "datafile.txt");
//    loadDatasetFromFile(&f, "dataset_img21.txt");

    //puts("Listar todos");
    //listar(f);
    printf("A media do dataset é: %d \n", media(&f));
    printf("A amplitude do dataset é: %d \n", amplitude(&f));
    printf("A variancia do dataset é: %f \n", variancia(&f));
    printf("A mediana do dataset é: %f \n", mediana(&f));

    kmeans(&f, k, e);

    

    return 0;
}

/************************************************************************************/
/* FUNCTIONS IMPLEMENTATION */

/*
    funcao para inicializar o vetor de centroids
*/
void initCentroids(DATASET *dataset, int *centroids, int n) {
    int value = 2;
    for(int i=0; i<n; i++) {
        centroids[i] = value;
        value *= 2;
    }
    /* testes */
    /***************************/
    int quartil1;
    int quartil3;
    quartil1 = quantileFunction(dataset, 0.25);
    quartil3 = quantileFunction(dataset, 0.75);
    printf("quartil1= %d quartil3= %d\n", quartil1, quartil3);
    centroids[0] = quartil1;
    centroids[1] = quartil3;
    /***************************/
    /* dividir o dataset em n+1 partes 
        (e se quiser mais de 100 centroids ou se o dataset tiver menos elementos que os centroids pretendidos) */
/*
    float parte;
    parte = 100 / (n+1) ;
    for(int i=0; i<n; i++) {
        centroids[i] = quantileFunction(dataset, parte);
        parte+=parte;
    }
*/
}

/*
    Listar centroids
*/
void listCentroids(int *centroids, int n) {
    printf("Lista de centroids: ");
    for(int i=0; i<n; i++) {
        printf("%d ", centroids[i]);
    }
    printf("\n");
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
    int delta;

    initCentroids(dataset, centroids, k);
    listCentroids(centroids, k);

    do {
        t += 1;

        for(int i=0; i<k;i++) {
            inic(&clusters[i]);
        }

        /*atribuicao de clusters*/
        ENTITY *tmp = *dataset;
        while(tmp != NULL) {
            aux = getClosetsCentroid(tmp->attribute, centroids, k);
            inserir(&clusters[aux], tmp->attribute);
            tmp=tmp->next;
        }

        /*atualizar os centroids*/
        delta = 0;
        for(int i=0; i<k; i++) {
            aux = media(&clusters[i]);
            delta += (centroids[i] < aux) ? aux-centroids[i] : centroids[i] - aux;
            centroids[i] = aux;
        }
        
        printf("delta %d \n", delta);
    } while( delta > e );  
    printf("Iteracaoes: %d\n", t);  

            
    for(int i=0; i<k; i++) {
        printf("Listar cluster %d \n", i);
        //listar(clusters[i]);
        printf("Media do cluster %d \n", media(&clusters[i]));
    }
}

