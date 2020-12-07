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
double* getClosestEntity(double *centroid);
void saveDataset(char *filename);
double* getDatasetQuantileEntity(double q);
double cumulativeDistribution(double norm);
void updateClusterAssociation();

#endif