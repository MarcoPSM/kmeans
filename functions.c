#include "kmeans.h"
#include <math.h>

/* FUNCTIONS IMPLEMENTATION */


void  initArray(int a[], int n, int value) {
    for (int i=0; i<n; i++) {
        a[i]=value;
    }
}

int getNumberOfEntities(char *filename) {
    int nLines = 0;
    char c, last = SPACE;

    FILE *fp=NULL;
    fp=fopen(filename, "r");
    // Check if file exists 
    if (fp == NULL) 
    { 
        printf("Could not open file %s", filename); 
        return 0; 
    } 

    // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) {

        if (c == '\n') {
            // Increment nLines if this character is newline 
            nLines++;
            
            if (c == last) {
                //Decrement nLines, is an empty line
                nLines--;
            }
        } 
        else {
            if (c == '#') {
                if (last == '\n') {
                    //Decrement nLines, is a comment line
                    nLines--;
                }
            }
        }
        last = c;
    }

    printf("The file %s has %d lines\n ", filename, nLines); 

    fclose(fp);
    return nLines;
}

int getNumberOfDimensions(char *filename) {
    int nLines = 0, nAttributes = 0, nAttributesToWork = 0;
    char c, last = SPACE, lastNonSpace;
    int inAttr = FALSE;

    FILE *fp=NULL;
    fp=fopen(filename, "r");
    // Check if file exists 
    if (fp == NULL) 
    { 
        printf("Could not open file %s", filename); 
        return 0; 
    } 

    // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) {

        if (c == '\n') {
            // Increment nLines if this character is newline 
            nAttributes++;
            break;
        } 
        else {
            if (c == '#') {
                break;
            }
            else {
                if (  c == SPACE ) {
                    inAttr = FALSE;
                    nAttributes++;
                }
                else {
                    inAttr = TRUE; 
                }
            }
        }
    }
    printf("The file %s has %d nAttributesToWork\n ", filename, nAttributes);

    fclose(fp);

    return nAttributes;
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

/*
    Calculate Euclidean norm of all entities
    is registered in the position after the last dimension
*/
void calculateNorm(float MATRIZ[][MAX_DIM], int n, int m) {
    for(int i=0; i<n; i++) {
        MATRIZ[i][m] = norm(MATRIZ[i], m);
    }

}

/*
    Initial Cluster Association is registered in the last position 
*/
void initClusterAssociation(float MATRIZ[][MAX_DIM], int n, int m) {
    for (int i=0; i<n; i++) {
        MATRIZ[i][m-1] = -1;
    }
}

void initClusters(int a[], int n) {
    for (int i=0; i<n; i++) {
        a[i]=-1;
    }
}

/*
  Calculate Euclidean distance between 2 vectores.
  Args: 
    v1 Pointer for vector 1
    v2 Pointer for vector 2
    n  Vectores dimensions
*/
float distance(float v1[], float v2[], int n) {
    /*
    printf("Listar os vectores para a Distancia\n"); 
    listarArray(v1, n);
    printf("Listar os vectores para a Distancia\n"); 
    listarArray(v2, n);
    */

    float d = 0;
    for (int i=0; i<n; i++) {
        //printf("v1=%.2f v2=%.2f pow=%f \n", v1[i], v2[i], pow (v1[i]-v2[i], 2));
        //printf("v1=%.2f v2=%.2f pow=%f \n", *v1, *v2, pow (*v1-*v2, 2));
        //printf("v1=%.2f v2=%.2f pow=%f \n", v1[i], v2[i], pow (v1[i]-v2[i], 2));
        //d += pow (*v1-*v2, 2);
        d += pow (v1[i]-v2[i], 2);
        //v1++;
        //v2++;
    }
    //printf("Distancia=%f\n", sqrt(d));
    return sqrt(d);
}

/*
  Calculate Euclidean norm of a vector.
  Args: 
    v1 Pointer for vector 
    n  Vector dimension
*/
float norm(float v1[], int n) {
    float d = 0;
    for (int i=0; i<n; i++) {
        d += pow (v1[i], 2);
    }
    return sqrt(d);
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
    using the dimensions vector norm saved in the penultimate position 
*/
float CDF(float MATRIZ [][MAX_DIM], int n, int m, float norm) {
    int soma = 0;
    int normPosition = m;
    for(int i=0; i<n; i++) {
        if(MATRIZ[i][normPosition] <= norm) {
            soma++;
        }
    }
    //printf("Norma=%f Soma=%d n=%d CDF = %f \n", norm, soma, n, (float)soma / (float) n);
    return (float)soma / (float) n;
}


/*
    Inverse Cumulative Distribution Function or quantile function (pag.43)
    F^{−1} (q) = min{x | F (x) ≥ q}   for q ∈ [0, 1]
    
    Norm of all entities is registered in the penultimate position 

    Return entity array
*/
float* quantileFunction(float MATRIZ [][MAX_DIM], int n, int m, float q) {
    float *x;
    int normPosition = m;
    x=MATRIZ[0];
    int flag = FALSE;
    for(int i=0; i<n; i++) {
        if(CDF(MATRIZ, n, m, MATRIZ[i][normPosition]) >= q) {
            if (flag == FALSE) {
                x=MATRIZ[i];
                flag = TRUE;
            }
            else if (MATRIZ[i][normPosition] < x[normPosition] ) {
                x=MATRIZ[i];
            }
        }
    }
    return x;
}


/*
    funcao para inicializar o vetor de centroids
*/
void initCentroids(float MATRIZ [][MAX_DIM], int n, int m, float centroids[][MAX_DIM], int k) {
    float *aux;
    int quantiles = k+1;
    float quantil = 1.0/quantiles;

    //printf("Dims = %d \n", m);

    for (int i=0; i<k; i++) {
        //printf("quartil = %f \n", quantil);
        aux = quantileFunction(MATRIZ, n, m, quantil);
        for (int j=0; j<m;j++) {
            centroids[i][j]=aux[j];
        }
        quantil += quantil;
        //printf("quartil%d= %f \n", i, centroids[i]);
    }

}

/*
    Listar centroids
*/
void listCentroids(float centroids[][MAX_DIM], int n, int m) {
    printf("Lista de centroids: \n");
    for(int i=0; i<n; i++) {
        for(int j=0;j<m; j++) {
            printf("%f ", centroids[i][j]);
        }
        printf("\n");
    }
    printf("\n");

}

float calculateDelta(float oldCentroids[][MAX_DIM], float centroids[][MAX_DIM], int k, int m) {
    float delta = 0;

    for(int i=0; i<k; i++) {
        delta += distance(&oldCentroids[i][0], &centroids[i][0], m);
        //printf("***DISTANCE=%f\n", distance(&oldCentroids[i][0], &centroids[i][0], m));
    }
    //printf("***delta=%f\n", delta);
    return delta;
}

/*
    recebe o atributo da entidade, o vetor de centroides, o tamanho desse vetor
    Devolve a posicao do centroid mais proximo
*/
int getClosetsCentroid(float attributes[], int m, float centroids[][MAX_DIM], int n) {
    float minDistance;
    int colsestCentroid;
    float d;
    for(int i=0; i<n; i++) {

        //distance = (attributes[0]<centroids[i]) ? centroids[i] - attributes[0] : attributes[0] - centroids[i];
        d = distance(attributes, centroids[i], m);

        if(i==0) {
            minDistance = d;
            colsestCentroid = i;
        } 
        else {
            if(d < minDistance) {
                minDistance = d;
                colsestCentroid = i;
            }
        }
        //printf("ATTR:%f %f %f %f %d\n",attributes[0], centroids[i], distance, minDistance, colsestCentroid);
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
        //printf("soma=%f total=%f C=%d A=%f P=%d\n", soma, total, clusters[i], MATRIZ[i][0], pos);
    }

    return soma / total;
}

/*
    Copy matrix a into matrix b.
*/
void  copyMatrix(float a[][MAX_DIM], float b[][MAX_DIM], int n, int m) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            b[i][j]=a[i][j];
        }
    }
}


/*
    calculate the centroids array
*/
void calcCentroids(float MATRIZ[][MAX_DIM], int n, int m, float centroids[][MAX_DIM], int k) {
    int cnt[k];
    initArray(cnt, k, 0);

    //reset centroids
    for (int i=0; i<k; i++) {
        for (int j=0; j<m; j++) {
            centroids[i][j]=0;
        }
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            centroids[ (int) MATRIZ[i][m+1] ][j]+=MATRIZ[i][j];
        }
        cnt[(int)MATRIZ[i][m+1]]+=1;
    }
    for (int i=0; i<k; i++) {
        for (int j=0; j<m; j++) {
            centroids[i][j] /= cnt[i];
        }
    }
}


void listarCluster(float MATRIZ[][MAX_DIM], int n, int m, int clusters[], int centroid) {
    for (int i=0; i<n; i++) {
        if(clusters[i]==centroid) {
            printf("%f ", MATRIZ[i][0]);
        }
    }
    printf("\n");
}

/*
    Write the output into a file
    The output is the data plus cluster identification.
    The cluster identification is saved on the last position of dimensions vector.
*/
void save(float MATRIZ[][MAX_DIM], int n, int m, char *filename) {
    int clustersPosition = m+1;
    FILE *fp=NULL;
    fp=fopen(filename, "w");
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            fprintf(fp, "%f ", MATRIZ[i][j]);
        }
        fprintf(fp, "%d\n", (int) MATRIZ[i][clustersPosition]);
    }
    fclose(fp);
    return;
}