#include <stdio.h>

int main(int argc, int *argv[]) {
    if (argc < 2) {
        printf("Sintaxe: \n%s <nome_ficheiro>\n", argv[0]);
        exit(1);
    }
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

    return 0;
}
