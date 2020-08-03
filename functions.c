#include "kmeans.h"

/* FUNCTIONS IMPLEMENTATION */


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
float CDF(float MATRIZ [][MAX_DIM], int n, int m, float x[]) {
    int soma = 0;
    for(int i=0; i<n; i++) {
        if(MATRIZ[i][0] <= x[0]) {
            soma++;
        }
    }
    return (float)soma / (float) n;
}


/*
    Inverse Cumulative Distribution Function or quantile function (pag.43)
    F^{−1} (q) = min{x | F (x) ≥ q}   for q ∈ [0, 1]
*/
float quantileFunction(float MATRIZ [][MAX_DIM], int n, int m, float q) {
    float *x;
    x=MATRIZ[0];
    for(int i=0; i<n; i++) {
        if(CDF(MATRIZ, n, m, x) >= q) {
            if(MATRIZ[i][0] < x[0] ) {
                x=MATRIZ[i];
            }
        }
    }
    return x[0];
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

    float quartil1;
    float quartil3;
    quartil1 = quantileFunction(MATRIZ, n, m, 0.25);
    quartil3 = quantileFunction(MATRIZ, n, m, 0.75);
    printf("quartil1= %f quartil3= %f\n", quartil1, quartil3);
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

void listarCluster(float MATRIZ[][MAX_DIM], int n, int m, int clusters[], int centroid) {
    for (int i=0; i<n; i++) {
        if(clusters[i]==centroid) {
            printf("%f ", MATRIZ[i][0]);
        }
    }
    printf("\n");
}

void save(float MATRIZ[][MAX_DIM], int CLUSTERS[], int n, int m, char *filename) {
    FILE *fp=NULL;
    fp=fopen(filename, "w");
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            fprintf(fp, "%f ", MATRIZ[i][j]);
        }
        fprintf(fp, "%d\n", CLUSTERS[i]);
    }
    fclose(fp);
    return;
}