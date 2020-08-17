#ifndef CM_H
#define CM_H 

void alocateCentroids();
void alocateOldCentroids();
void initCentroidsMatrix();
void listCentroidsMatrix();
int getClosetsCentroidPosition(float *entity);
void backupCentroids();
void updateCentroids();
float getDelta();

#endif
