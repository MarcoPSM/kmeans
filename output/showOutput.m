#! /usr/bin/octave -qf
# Author: Marco Martins <marcopaulomartins@hotmail.com>
# Date: 10/05/2020

dimensions = 2;
debug = 0;

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

figure(1);
plot(x,y,"*3");
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

    plot(mx,my,strcat("*",num2str(i+1)));
end;
hold off;
xlabel("x");
ylabel("y");
title("kmeans");
grid;

pause (4);


if debug == 1

    #fprintf("%f\n", data(1,1));
    fprintf("Linhas: %d\n", size(data)(1));
    fprintf("colunas: %d\n", size(data)(2));
    #fprintf("%d\n", size(data(1)));
    #fprintf("%d\n", data(1,2));
    fprintf("Linhas: %d\n", size(subdata)(1));
    fprintf("colunas: %d\n", size(subdata)(2));
    fprintf("Linhas: %d\n", size(clusters)(1));
    fprintf("colunas: %d\n", size(clusters)(2));

    fprintf("Max= %f\n", max(clusters));

    fprintf("Min= %f\n", min(subdata(:, 1)) );
    fprintf("Max= %f\n", max(subdata(:, 1)) );

    fprintf("%d\n", interval);

    fprintf("Linhas MX: %d\n", size(mx)(1));
    fprintf("colunas MX: %d\n", size(mx)(2));

    #
    #interval =  ceil( (max(subdata(:, 1)) - min(subdata(:, 1)) ) / 10 );
    #x = [(min(subdata(:, 1)) : interval : max(subdata(:, 1)) )];

    #interval =  ceil( (max(subdata(:, 2)) - min(subdata(:, 2)) ) / 10 );
    #y = [(min(subdata(:, 2)) : interval : max(subdata(:, 2)) )];
    #
endif