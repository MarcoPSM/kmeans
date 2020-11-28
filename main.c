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

cc main.c general.c datasetMatrix.c centroidsMatrix.c datasetFile.c functions_t.c -lm -pthread -o kmeans

*/


/* 
https://www.tutorialspoint.com/c_standard_library/limits_h.htm 
pointers:
    https://codeforwin.org/2017/12/access-two-dimensional-array-using-pointers-c-programming.html
Threads:
    https://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/
*/

#include "datasetMatrix.h"
#include "general.h"
#include "centroidsMatrix.h"
#include "datasetFile.h"
#include "functions_t.h"


// Matrix for dataset
float **dataset;
// Number of lines
int nLines;
// Number of columns
int nColumns;
// Number of dimensions
int nDimensions;

float *totals;
// Number of clusters
int k = DEFAULT_CLUSTERS_NUMBER;
// Max Error
float e = DEFAULT_MAX_CONVERGENCE;
// Matrix for centroids
float **centroids;
// Flag for threads
int with_threads = FALSE;

void kmeans();

/* MAIN FUNCTION */
int main(int argc, char *argv[]) {
    char *inputfile;
    inputfile = NULL;

    if (argc % 2 == 0) {
        printf("Erro numero de argumentos.\n");
        exit(1);
    }
    argv++; //salta o nome do programa
    while (*argv != NULL) {
        if (strcmp(*argv, "-f") == 0) {
            argv++;
            inputfile = *argv;
        }
        else if (strcmp(*argv, "-k") == 0) {
            argv++;
            k = atoi(*argv);
        }
        else if (strcmp(*argv, "-e") == 0) {
            argv++;
            e = atof(*argv);
        }
        else {
            printf("Erro nos argumentos.\n");
            exit(1);
        }
        argv++;
    }
    printf("f=%s; k=%d, e=%f\n", inputfile,k,e);
    if (inputfile == NULL) {
        printf("Erro nos argumentos. Nao tem o nome do file.\n");
        exit(1);
    }

    // Output filename
    char *outfilename = OUTPUT;

    nLines = getNumberOfEntities(inputfile);
    nDimensions = getNumberOfDimensions(inputfile);

    nColumns = nDimensions + ADDITIONAL_COLUMNS;

    if (nDimensions > ENTITY_MAX_DIMENSION) {
        printf("O numero de dimensoes tem de ser no maximo %d", ENTITY_MAX_DIMENSION);
        exit(1);
    }

    alocateDataset();
    loadDataset(inputfile);
    //listDataset();
    calculateEntitiesNorm();
    //listFullDatasetMatrix();
    kmeans();
    saveDataset(outfilename);

    return 0;
}

void kmeans() {
    /*iterador*/
    int t=0;
    float aux;
    float delta;


    alocateCentroids();
    alocateOldCentroids();
    initCentroidsMatrixV2();

    /*
    printf("????????????\n");
    if (with_threads == TRUE) {
        initCentroidsMatrix_t();
    }
    else {
        initCentroidsMatrix();
    }
    */
    
    
    char cbuf[30];
    char buf[30];
    do {
        listCentroidsMatrix();

        t++;
        if (with_threads == TRUE) {
            updateClusterAssociation_t();
        }
        else {
            updateClusterAssociation();
        }
        
        //debug
        
        snprintf(cbuf, 30, "output/centroids_%d.out", t);
        saveCentroids(cbuf);

        backupCentroids();
        updateCentroids();
        delta = getDelta();
        //printf("Delta=%f\n", delta);

        //debug
        
        snprintf(buf, 30, "output/kmeans_%d.out", t);
        saveDataset(buf);


    } while( delta > e && t < MAX_ITERATIONS);  
    printf("Iteracaoes: %d\n", t);  
    //listCentroidsMatrix();
}