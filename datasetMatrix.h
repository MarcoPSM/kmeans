#ifndef DM_H
#define DM_H

#include "general.h"

void alocateDataset();
void loadDataset(char *filename);
void listDataset();
void listFullDatasetMatrix();
void calculateEntitiesNorm();
void saveDataset(char *filename);
float* getDatasetQuantileEntity(float q);
float cumulativeDistribution(float norm);
void updateClusterAssociation();

#endif