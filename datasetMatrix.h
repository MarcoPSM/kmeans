#ifndef DM_H
#define DM_H

#include "general.h"

void alocateTotals();
void loadTotals();
void alocateDataset();
void loadDataset(char *filename);
void listDataset();
void listFullDatasetMatrix();
void calculateEntitiesNorm();
float* getClosestEntity(float *centroid);
void saveDataset(char *filename);
float* getDatasetQuantileEntity(float q);
float cumulativeDistribution(float norm);
void updateClusterAssociation();

#endif