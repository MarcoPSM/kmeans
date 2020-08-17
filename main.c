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

// Matrix for dataset
float **dataset;
// Number of lines
int nLines;
// Number of columns
int nColumns;
// Number of dimensions
int nDimensions;
// Number of clusters
int k = DEFAULT_CLUSTERS_NUMBER;
// Max Error
float e = DEFAULT_MAX_CONVERGENCE;
// Matrix for centroids
float **centroids;

void kmeans();

/* MAIN FUNCTION */
int main(int argc, char *argv[]) {

    if (argc < EXPECTED_ARGUMENTS) {
        printf("Sintaxe: \n%s <nome_ficheiro>\n", argv[0]);
        exit(1);
    }
    // Output filename
    char outfilename[] = OUTPUT;

    nLines = getNumberOfEntities(argv[1]);
    nDimensions = getNumberOfDimensions(argv[1]);
    nColumns = nDimensions + ADDITIONAL_COLUMNS;

    if (nDimensions > ENTITY_MAX_DIMENSION) {
        printf("O numero de dimensoes tem de ser no maximo %d", ENTITY_MAX_DIMENSION);
        exit(1);
    }

    alocateDataset();
    loadDataset(argv[1]);
    listDataset();
    calculateEntitiesNorm();
    listFullDatasetMatrix();
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
    initCentroidsMatrix();
    listCentroidsMatrix();

    e=0;

    do {
        t++;
        updateClusterAssociation();
        backupCentroids();
        updateCentroids();
        delta = getDelta();
        printf("Delta=%f\n", delta);

    } while( delta > e && t < MAX_ITERATIONS);  
    printf("Iteracaoes: %d\n", t);  

}