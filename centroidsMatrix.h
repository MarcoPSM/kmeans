#ifndef CM_H
#define CM_H 

void resetCentroids();
void alocateCentroids();
void alocateOldCentroids();
void initCentroidsMatrix();
void initCentroidsMatrixV2();
void listCentroidsMatrix();
int getClosetsCentroidPosition(float *entity);
void backupCentroids();
void updateCentroids();
void recalculateCentroid(int i);
float getDelta();
void saveCentroids(char *filename);

#endif
