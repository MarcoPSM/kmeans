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
void inserir(DATASET *dataset, int value) {
    if(*dataset==NULL) {
        *dataset=(DATASET)malloc(sizeof(ENTITY));
        if(*dataset==NULL) {
            return;
        }
        (*dataset)->attribute=value;
        (**dataset).next=NULL;
    }
    else {
        inserir(& (**dataset).next, value);
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
    printf("Valor: %d \n", dataset->attribute);
    listar(dataset->next);
}

/*
    carregar o dataset do ficheiro
*/
void loadDatasetFromFile(DATASET *dataset, char *fileName) {
    FILE *datafile=NULL;
    datafile=fopen(fileName, "r");
    int v;
    while(fscanf(datafile,"%d",&v)!=EOF) {
        inserir(dataset, v);    
    }
    fclose(datafile);
}

int media(DATASET *dataset) {
    if (dataset==NULL) {
        /*nao ha elementos*/
        return 0;
    }
    ENTITY *tmp = *dataset;
    int soma = 0;
    int i=0;
    while(tmp != NULL) {
        soma += (int) tmp->attribute;
        i++;
        tmp = tmp->next;
    }
    return soma/i;
}

/*
    Calcular a amplitude do dataset
*/
int amplitude(DATASET *dataset) {
    if (dataset==NULL) {
        /*nao ha elementos*/
        return 0;
    }
    ENTITY *tmp = *dataset;
    int min, max;
    min = max= tmp->attribute;
    for (int i =0; tmp != NULL; i++) {
        if(tmp->attribute < min) {
            min = (int) tmp->attribute;
        }
        if(tmp->attribute > max) {
            max = (int) tmp->attribute;
        }
        tmp = tmp->next;
    }
    return max - min;
}

/*
    carregar o dataset do ficheiro
*/
void loadDataset(DATASET *dataset, FILE *f) {
    int v;
    while(fscanf(f,"%d",&v)!=EOF) {
        inserir(dataset, v);    
    }

}

/*
    Empirical Cumulative Distribution Function
*/
float CDF(DATASET *dataset, int x) {
    if (dataset==NULL) {
        /*nao ha elementos*/
        printf("Nao ha elementos!");
        return 0;
    }
    ENTITY *tmp = *dataset;
    int soma = 0;
    int n=0;
    while(tmp != NULL) {
        if(tmp->attribute <= x) {
            soma++;
        }
        n++;
        tmp = tmp->next;
    }

    return (float)soma / (float) n;
}

/*
    Inverse Cumulative Distribution Function or quantile function (pag.43)
    F^{−1} (q) = min{x | F (x) ≥ q}   for q ∈ [0, 1]
*/
int quantileFunction(DATASET *dataset, float q) {
    if (dataset==NULL) {
        /*nao ha elementos*/
        return 0;
    }
    ENTITY *tmp = *dataset;
    int x;
    int i=0;
    while(tmp != NULL) {
        if(CDF(dataset, tmp->attribute) >= q) {
            if(i==0) {
                x=tmp->attribute;
                i++;
            }
            else {
                if (tmp->attribute < x) {
                    x=tmp->attribute;
                }
            }
        }
        tmp = tmp->next;
    }
    return x;
}


float variancia(DATASET *dataset) {
    return (float) media(dataset);
}

float mediana(DATASET *dataset) {
    return (float) quantileFunction(dataset, 0.5);
}

