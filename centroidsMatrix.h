#ifndef CM_H
#define CM_H 

void resetNewCentroids();
void alocateCentroids();
void alocateNewCentroids();
void copyCentroidsFromNewCentroids();
void initCentroidsMatrix();
void listCentroidsMatrix();
int getClosetsCentroidPosition(double *entity);
void updateCentroids();
void recalculateCentroid(int i);
double getDelta();
void saveCentroids(char *filename);

#endif
