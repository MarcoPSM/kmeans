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

#include "kmeans.h"


/* MAIN FUNCTION */
int main(int argc, char *argv[]) {

    if (argc < EXPECTED_ARGUMENTS) {
        printf("Sintaxe: \n%s <nome_ficheiro>\n", argv[0]);
        exit(1);
    }
    // Output filename
    char outfilename[] = OUTPUT;
    //lines
    int n = getNumberOfEntities(argv[1]);
    //dimensions
    int d = getNumberOfDimensions(argv[1]);
    //columns 
    int m = d + ADDITIONAL_COLUMNS;

    if (d > ENTITY_MAX_DIMENSION) {
        printf("O numero de dimensoes tem de ser no maximo %d", ENTITY_MAX_DIMENSION);
        exit(1);
    }

    float matriz[n][m];
    float mean[d];
    /*lista de clusters
       Uma lista do tamanho do dataset, em cada posicao fica o cluster da 
       entidade correspondente a essa posicao no dataset */
    int CLUSTERS[n];

    // estas variaveis se calhar passam a vir por argumento
    int k = 4;
    int e = 1;

  
    loadMatrizFromFile(matriz, n, d, argv[1]);
    sampleMean(matriz, mean, n, d);
    calculateNorm(matriz, n, d);
    initClusterAssociation(matriz, n, m);


    //puts("Listar todos");
    listarMatriz(matriz, n, m);
    //listarArray(mean, d);
    printf("Nlinhas: %d\n", n);
    printf("NColunas: %d\n", d);
    for(int i=0; i<d; i++) {
        printf("Media da dimensao %d: %f\n", i+1, mean[i]);
    }

    kmeans(matriz, CLUSTERS, n, d, k, e);

    save(matriz, n, d, outfilename);

    return 0;
}
