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
