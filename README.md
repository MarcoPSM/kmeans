# kmeans
K-Means Clustering Algorithm Implementation in C

cc main.c kmeans.c functions.c -lm -o kmeans

cc -pthread main.c kmeans.c functions.c functions_t.c -lm -o kmeans

./kmeans datafiles/manaus.txt
