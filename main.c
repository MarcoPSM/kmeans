/******************************************
 * Marco Martins (1100619)                *
 * projeto final de curso Eng.Informática *
 * Ano: 2019/2020                         *
 ******************************************/

/*
Compilar:
cc main.c general.c datasetMatrix.c centroidsMatrix.c datasetFile.c functions_t.c -lm -pthread -o kmeans
Executar:
./kmeans -f datafiles/ds3d1milhoes.txt -k 8 -e 0.001 -t 4
*/


#include "datasetMatrix.h"
#include "general.h"
#include "centroidsMatrix.h"
#include "datasetFile.h"
#include "functions_t.h"


// Matrix for dataset
double **dataset;
// Number of lines
int nLines;
// Number of columns
int nColumns;
// Number of dimensions
int nDimensions;

double *totals;
// Number of clusters
int k = DEFAULT_CLUSTERS_NUMBER;
// Max Error
double e = DEFAULT_MAX_CONVERGENCE;
// Matrix for centroids
double **centroids;
// Flag for threads
int with_threads = FALSE;
int nThreads = MAX_THREADS;

// definir metodo utilizado em multithread
int active_method = METHOD_B;

// Set debig mode
int DEBUG = FALSE;


void kmeans();


/* MAIN FUNCTION */
int main(int argc, char *argv[]) {

    char *inputfile;
    inputfile = NULL;
    /*
    if (argc % 2 == 0) {
        printf("Erro numero de argumentos.\n");
        exit(1);
    }
    */
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
        else if (strcmp(*argv, "-t") == 0) {
            argv++;
            nThreads = atoi(*argv);
            with_threads = TRUE;
        }
        else if (strcmp(*argv, "-d") == 0) {
            DEBUG = TRUE;
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
    char *outCentroidsFile = OUTPUT_C;

    nLines = getNumberOfEntities(inputfile);
    nDimensions = getNumberOfDimensions(inputfile);

    nColumns = nDimensions + ADDITIONAL_COLUMNS;

    if (nDimensions > ENTITY_MAX_DIMENSION) {
        printf("O numero de dimensoes tem de ser no maximo %d", ENTITY_MAX_DIMENSION);
        exit(1);
    }

    // Start timer 
    time_t secounds = time(NULL);

    alocateDataset();
    loadDataset(inputfile);

    kmeans();
    printf("Execution time(secounds): %ld\n", time(NULL) - secounds);

    saveDataset(outfilename);
    saveCentroids(outCentroidsFile);

    return 0;
}

void kmeans() {
    /*iterador*/
    int t=0;
    double aux;
    double delta;


    alocateCentroids();
    alocateNewCentroids();
    resetNewCentroids();


    if (with_threads == TRUE) {
        initCentroidsMatrix_t();      
    }
    else {
        initCentroidsMatrix();
    }
    
    
    do {

        copyCentroidsFromNewCentroids();
        resetNewCentroids();

        t++;

        if (with_threads == TRUE) {
            updateClusterAssociation_t();
        }
        else {
            updateClusterAssociation();
        }

        delta = getDelta();
        //printf("Delta=%f\n", delta);

    } while( delta > e && t < MAX_ITERATIONS);  
    
    printf("Iteracaoes: %d\n", t);  
    if (DEBUG == TRUE) {
        listCentroidsMatrix();
    }
}