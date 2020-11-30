#! /usr/bin/octave -qf
# Author: Marco Martins <marcopaulomartins@hotmail.com>
# Date: 10/05/2020

# sudo apt install epstool
# sudo apt install fig2dev
# sudo apt install pstoedit


dimensions = 2;

# colors as RGB values
colorspec = {[0.9 0.1 0.1]; [0.1 0.9 0.1]; [0.1 0.1 0.9]; 
             [0.9 0.5 0.1]; [0.1 0.9 0.5]; [0.5 0.1 0.9]; 
             [0.9 0.5 0.5]; [0.5 0.9 0.5]; [0.5 0.5 0.9]; 
             [0.9 0.9 0.1]; [0.1 0.9 0.9]; [0.9 0.1 0.9];
             [0.5 0.5 0.5]; [0.7 0.5 0.1]; [0.1 0.5 0.7]; };

f = fopen("kmeans.out");
data = dlmread(f, ' ');
fclose(f);

# Matriz sem a coluna dos clusters
subdata = data(:, 1:size(data)(2)-1);

#coluna dos clusters
clusters = data(:, size(data)(2));

# x sao os elementos da primeira coluna
x = [subdata(:, 1)];

if ( size(subdata)(2) > 1 ) 
    # y sao os elementos da segunda coluna, caso exista
    y = [subdata(:, 2)];
else 
    y = zeros(size(subdata)(1));
endif

################ centroids ####
f = fopen("centroids.out");
centroids_data = dlmread(f, ' ');
fclose(f);
cx = [centroids_data(:, 1)];
cy = [centroids_data(:, 2)];
###############################

figure(1);
hold on;

###
nClusters = max(clusters);
for i=0:nClusters
    mx = [subdata(clusters(:,1) == i, 1)];

    if ( size(subdata)(2) > 1 ) 
        my = [subdata(clusters(:,1) == i, 2)];
    else
        my = zeros(size(mx)(1));
    endif

    #plot(mx,my,strcat(".",num2str(i+1)));
    plot(mx,my, '.', 'Color', colorspec{mod(i,15)+1}); 
end;
#### centroids ####
plot(cx,cy,"*k", "markersize", 10);
###################

hold off;
xlabel("x");
ylabel("y");
title("kmeans");
grid;

pause (20);
