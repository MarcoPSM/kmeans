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

    if (argc<2) {
        printf("Sintaxe: \n%s <nome_ficheiro>\n", argv[0]);
        exit(1);
    }
    // Output filename
    char outfilename[] = "output/kmeans.out";
    //lines
    int n = getNumberOfEntities(argv[1]);
    //dimensions
    int d = getNumberOfDimensions(argv[1]);
    //columns
    int m = d + 1;

    float MATRIZ[n][m];
    float mean[d];
    /*lista de clusters
       Uma lista do tamanho do dataset, em cada posicao fica o luster da 
       entidade correspondente a essa posicao no dataset */
    int CLUSTERS[n];

    int k = 2;
    int e = 0;

  
    loadMatrizFromFile(MATRIZ, n, d, argv[1]);
    sampleMean(MATRIZ, mean, n, d);

    //puts("Listar todos");
    listarMatriz(MATRIZ, n, d);
    listarArray(mean, d);

    kmeans(MATRIZ, CLUSTERS, n, d, k, e);

    save(MATRIZ, CLUSTERS, n, d, outfilename);

    return 0;
}
