/******************************************
 * Marco Martins                          *
 * projeto final de curso Eng.Informática *
 * algoritmo na pag. 344                  *
 ******************************************/

/*
PLANO :
    a estrutura é um ponteiro para array de floats e
        mais alguns foalts para a media dos atributos 
        a variacao dos atributos e mais alguma coisa que me lembre
    
    
    Implementar todas as funcoes da analise univariavel
    escrever o output num ficheiro
    mostrar um grafico com octave
    O dicheiro de entrada tem de entrar por argumento

cc kmeans.c dataset.c -o kmeans

*/


/* 
https://www.tutorialspoint.com/c_standard_library/limits_h.htm 
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_DIM 3

/************************************************************************************/
/* HEADERS */

int getNumberOfEntities(char *filename);
int getNumberOfDimensions(char *filename);
void loadMatrizFromFile(float MATRIZ[][MAX_DIM], int n, int m, char *filename);
void listarMatriz(float MATRIZ[][MAX_DIM], int n, int m);
void listarArray(float a[], int n);
void initClusters(int a[], int n);
void listarCluster(float MATRIZ[][MAX_DIM], int n, int m, int clusters[], int centroid);

void sampleMean(float MATRIZ[][MAX_DIM], float mean[MAX_DIM], int n, int m);

void kmeans(float MATRIZ[][MAX_DIM], int n, int m, int k, int e);

/************************************************************************************/
/* MAIN FUNCTION */
int main(int argc, char *argv[]) {

    if (argc<2) {
        printf("Sintaxe: \n%s <nome_ficheiro>\n", argv[0]);
        exit(1);
    }

    //lines
    int n = getNumberOfEntities(argv[0]);
    //dimensions
    int d = getNumberOfDimensions(argv[0]);
    //columns
    int m = d + 1;

    float MATRIZ[n][m];
    float mean[d];

    int k = 2;
    int e = 0;

  
    loadMatrizFromFile(MATRIZ, n, d, argv[1]);
    sampleMean(MATRIZ, mean, n, d);

    //puts("Listar todos");
    listarMatriz(MATRIZ, n, d);
    listarArray(mean, d);

    kmeans(MATRIZ, n, d, k, e);

    return 0;
}

/************************************************************************************/
/* FUNCTIONS IMPLEMENTATION */

int getNumberOfEntities(char *filename) {
    return 9;
}

int getNumberOfDimensions(char *filename) {
    return 2;
}

void loadMatrizFromFile(float MATRIZ[][MAX_DIM], int n, int m, char *filename) {
    FILE *datafile=NULL;
    datafile=fopen(filename, "r");
    for(int i=0; i<n; i++) {
        for(int j=0;j<m; j++) {
            if( !fscanf(datafile, "%f", &MATRIZ[i][j]) ) {
                break;
            }
        }
    }
    //printf("Numero= %f", attributes[0]);
    fclose(datafile);
}

void listarMatriz(float MATRIZ[][MAX_DIM], int n, int m) {
    for(int i=0; i<n; i++) {
        for(int j=0;j<m; j++) {
            printf("%f ", MATRIZ[i][j]);
        }
        //puts("\n");
        printf("\n");
    }
}

void listarArray(float a[], int n) {
    for(int i=0; i<n; i++) {
        printf("%f\n", a[i]);
    }
}

void initClusters(int a[], int n) {
    for (int i=0; i<n; i++) {
        a[i]=-1;
    }
}

void sampleMean(float MATRIZ[][MAX_DIM], float mean[], int n, int m) {
    float s = 0;
    for(int j=0; j<m; j++) {
        for(int i=0;i<n; i++) {
            s += MATRIZ[i][j];
        }
        //printf("S=%f N=%d M=%f\n", s, n, s/(float)n);
        mean[j]= s / (float)n;
        //printf("M=%f\n", mean[j]);
        s = 0;
    }
}

/*****************************************************************************/
/*****************************************************************************/

/*
    Empirical Cumulative Distribution Function
*/
float CDF(float MATRIZ [][MAX_DIM], int n, int m, int x) {
    int soma = 0;
    for(int i=0; i<n; i++) {
        if(MATRIZ[i][0] <= x) {
            soma++;
        }
    }
    return (float)soma / (float) n;
}


/*
    Inverse Cumulative Distribution Function or quantile function (pag.43)
    F^{−1} (q) = min{x | F (x) ≥ q}   for q ∈ [0, 1]
*/
int quantileFunction(float MATRIZ [][MAX_DIM], int n, int m, float q) {
    int x;
    int i=0;
    x=MATRIZ[0][0];
    for(int i=0; i<n; i++) {
        if(CDF(MATRIZ, n, m, x) >= q) {
            if(MATRIZ[i][0] < x ) {
                x=MATRIZ[i][0];
            }
        }
    }
    return x;
}


/*
    funcao para inicializar o vetor de centroids
*/
void initCentroids(float MATRIZ [][MAX_DIM], int n, int m, float centroids[], int k) {
    float value = 2;
    for(int i=0; i<k; i++) {
        centroids[i] = value;
        value *= 2;
    }

    /* testes */
    /***************************/
/*
    float quartil1;
    float quartil3;
    quartil1 = quantileFunction(MATRIZ, n, m, 0.25);
    quartil3 = quantileFunction(MATRIZ, n, m, 0.75);
    printf("quartil1= %f quartil3= %f\n", quartil1, quartil3);
    centroids[0] = quartil1;
    centroids[1] = quartil3;
*/
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
void listCentroids(float centroids[], int n) {
    printf("Lista de centroids: ");
    for(int i=0; i<n; i++) {
        printf("%f ", centroids[i]);
    }
    printf("\n");
}

/*
    recebe o atributo da entidade, o vetor de centroides, o tamanho desse vetor
    Devolve a posicao do centroid mais proximo
*/
int getClosetsCentroid(float attributes[], int m, float centroids[], int n) {
    float minDistance;
    int colsestCentroid;
    float distance;
    for(int i=0; i<n; i++) {

        distance = (attributes[0]<centroids[i]) ? centroids[i] - attributes[0] : attributes[0] - centroids[i];

        if(i==0) {
            minDistance = distance;
            colsestCentroid = i;
        } 
        else {
            if(distance < minDistance) {
                minDistance = distance;
                colsestCentroid = i;
            }
        }
        printf("ATTR:%f %f %f %f %d\n",attributes[0], centroids[i], distance, minDistance, colsestCentroid);
    }
    return colsestCentroid;
}

/* 
    Calc new centroid
*/
float calcCentroid(float MATRIZ[][MAX_DIM], int m, int clusters[], int n, int pos) {
    float soma = 0;
    float total =0;
    for (int i=0; i<n; i++) {
        if(clusters[i]==pos) {
            soma+=MATRIZ[i][0];
            total++;
        }
        printf("soma=%f total=%f C=%d A=%f P=%d\n", soma, total, clusters[i], MATRIZ[i][0], pos);
    }

    return soma / total;
}

void listarCluster(float MATRIZ[][MAX_DIM], int n, int m, int clusters[], int centroid) {
    for (int i=0; i<n; i++) {
        if(clusters[i]==centroid) {
            printf("%f ", MATRIZ[i][0]);
        }
    }
    printf("\n");
}

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


