#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//  cc teste_distance.c -lm -o teste

float distance(float v1[], float v2[], int n);

int main(int argc, char const *argv[])
{
    float a[3] = {-1,2,-3};
    float b[3] = {2,-4,-6};
    float c[3] = {-1,-2,-3};
    float d[3] = {2,4,6};
    printf("A distancia entre A e B = %f\n", distance(b,a,3));
    printf("A distancia entre C e D = %f\n", distance(c,d,3));
    return 0;
}

float distance(float v1[], float v2[], int n) {
    float d = 0;
    for (int i=0; i<n; i++) {
        d += pow (v1[i]-v2[i], 2);
    }
    //return sqrt(d);
    return d;
}