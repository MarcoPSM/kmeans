#ifndef DATASET_H
#define DATASET_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Entity {
    int attribute;
    struct Entity *next;
} ENTITY; 

typedef ENTITY *DATASET;

void inic(DATASET *dataset);
void inserir(DATASET *dataset, int value);
void apagar(DATASET *dataset);
void listar(DATASET dataset);
void loadDataset(DATASET *dataset, FILE *f);
void loadDatasetFromFile(DATASET *dataset, char *fileName);

int media(DATASET *dataset);
int amplitude(DATASET *dataset);
float CDF(DATASET *dataset, int x);
int quantileFunction(DATASET *dataset, float q);
float variancia(DATASET *dataset);

#endif
