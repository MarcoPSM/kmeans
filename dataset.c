#include "dataset.h"
/*
    Iniciar DATASET
*/
void inic(DATASET *dataset) {
    *dataset=NULL;
}

/*
    Insere um novo registo no fim da dataset
*/
void inserir(DATASET *dataset, float *value, int columns) {
    if(*dataset==NULL) {
        *dataset=(DATASET)malloc(sizeof(ENTITY));
        if(*dataset==NULL) {
            return;
        }
        float new_attributes[columns];
        for(int i=0; i<columns; i++) {
            new_attributes[i] = value[i];
        }
        (*dataset)->attribute=new_attributes;
        (**dataset).next=NULL;
    }
    else {
        inserir(& (**dataset).next, value, columns);
    }
}

/*
    Apagar o primeiro elemento da dataset
*/
void apagar(DATASET *dataset) {
    ENTITY *tmp = *dataset;
    if(*dataset==NULL) {
        /*nao ha elementos*/
        return;
    }
    *dataset=(*dataset)->next;
    free(tmp);
}

/*
    Listar todos os elementos de forma recursiva
*/
void listar(DATASET dataset) {
    if (dataset==NULL) {
        /*nao ha elementos*/
        return;
    }
    printf("Valor: %f \n", dataset->attribute[0]);
    listar(dataset->next);
}

/*
    carregar o dataset do ficheiro
*/
void loadDatasetFromFile(DATASET *dataset, char *fileName) {
    FILE *datafile=NULL;
    datafile=fopen(fileName, "r");
    int columns=1;
    float attributes[columns];
    while(fscanf(datafile,"%f",attributes) !=EOF ) {
        printf("attr= %f\n", attributes[0]);
        inserir(dataset, attributes, columns);    
    }
    fclose(datafile);
}

