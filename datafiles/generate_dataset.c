#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[]) {
    FILE *fp4;
    FILE *fp3;
    FILE *fp2;
    FILE *fp1;


    srand((unsigned int)time(NULL));

    float topa = 3.0;
    float topb = 7.0;
    float topc = 150.0;
    float topd = 100.0;
    float a, b, c, d;

    fp4 = fopen("ds4d1milhoes.txt", "w+");
    fp3 = fopen("ds3d1milhoes.txt", "w+");
    fp2 = fopen("ds2d1milhoes.txt", "w+");
    fp1 = fopen("ds1d1milhoes.txt", "w+");

    for (int i=0; i<1000000; i++) {
        a = (float)rand() / (float)(RAND_MAX) * topa + 12;
        b = (float)rand() / (float)(RAND_MAX) * topb - 15;
        c = (float)rand() / (float)(RAND_MAX) * topc + 50;
        d = (float)rand() / (float)(RAND_MAX) * topd + 450;
        fprintf(fp4, "%.2f %.2f %.2f %.2f\n", a, b, c, d);
        fprintf(fp3, "%.2f %.2f %.2f\n", a, b, c);
        fprintf(fp2, "%.2f %.2f\n", a, b);
        fprintf(fp1, "%.2f\n", a);
    }
    return 0;

}