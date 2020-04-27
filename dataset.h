#ifndef DATASET_H
#define DATASET_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Entity {
    float *attribute;
    struct Entity *next;
} ENTITY; 

typedef ENTITY *DATASET;

void inic(DATASET *dataset);
void inserir(DATASET *dataset, float *value, int columns);
void apagar(DATASET *dataset);
void listar(DATASET dataset);
//void loadDataset(DATASET *dataset, FILE *f);
void loadDatasetFromFile(DATASET *dataset, char *fileName);
/*
int media(DATASET *dataset);
int amplitude(DATASET *dataset);
float CDF(DATASET *dataset, int x);
float PMF(DATASET *dataset, int x);
int quantileFunction(DATASET *dataset, float q);
float variancia(DATASET *dataset);
float mediana(DATASET *dataset);
int moda(DATASET *dataset);
*/

#endif
