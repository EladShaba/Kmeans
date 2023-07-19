#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "kmeansh.h"

/* calculate euclidean distance */
double distance(double *vector1, double *vector2, int dimension) {
    double dist = 0;
    int i;
    for (i = 0;i<dimension;i++) {
        dist += (vector1[i] - vector2[i]) * (vector1[i] - vector2[i]);
    }
    return sqrt(dist);
}

/* calculate clusters centroid */
void centroids_calculation(double **data ,int *clusters, double **centroids, int n, int k, int dimension) 
{ 
    int i, j;
    int *sizeCluster = malloc(k * sizeof(int));
    /* zerofiy */
    for (i = 0; i < k; i++) {
        sizeCluster[i] = 0;
        for (j = 0; j < dimension; j++) 
        {
            centroids[i][j] = 0;
        }
    }

    /* set cluster with all the values */
    for (i = 0; i < n; i++) {
        int cluster = clusters[i];
        sizeCluster[cluster]++;
        for (j = 0; j < dimension; j++) 
        {
            centroids[cluster][j] += data[i][j];
        }
    }

    /* calculate centroid*/
    for (i=0;i<k;i++) {
        if (sizeCluster[i]>0) {
            for (j = 0; j < dimension; j++) 
            {
                centroids[i][j] /= sizeCluster[i];
            }
        }
    }

    /* free memory*/
    free(sizeCluster);
}

/* calculate what cluster to assign each point */
int *cluster_calculation(double **data, double **centroids, int n, int k, int dimension) 
{
    int *dataPoints, i, j;
    int p;
    double min_dist;
    dataPoints = malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        min_dist = INFINITY;
        p=-1;
        for (j = 0; j < k; j++) {
            double dist = distance(data[i], centroids[j], dimension);
            if (dist<min_dist) 
            {
                min_dist= dist; 
                p = j;
            }
        }
        dataPoints[i] = p; /*assign to cluster*/
    }
    return dataPoints;
}



int main(int argc, char *argv[]) {
    return 0;
}

void kmeans(double **datapoints, double **centroids, int n, int k, int dimension, int iter, double eps) 
{
    double **last_centroids;
    int i,j, count, convergence, *Cluster;

    convergence = 1;
    Cluster = NULL;

    /* Initialize the previous centroids */
    last_centroids = (double *) malloc(k * sizeof(double *));
    for (i = 0; i < k; i++) {
        last_centroids[i] = malloc(dimension * sizeof(double));
        for (j = 0 ; j < dimension; j++) 
        {
            last_centroids[i][j] = centroids[i][j];
        }
    }

    for (count = 0;count<iter;count++) 
    {
        Cluster = cluster_calculation(datapoints, centroids, n, k, dimension);/* create clusters */
        centroids_calculation(datapoints, Cluster,centroids, n, k, dimension);/* update Centorids */
        convergence = 1;
        for (i = 0; i < k; i++) 
        {
            double eucDistance = distance(centroids[i], last_centroids[i], dimension);
            if (eucDistance>eps) {
                convergence = 0;
                break;
            }
        }
        if (1==convergence) /* check for convergence */
        {
            break;
        }

        
        for (i = 0; i < k; i++) /* Update Clusters and previous centroids */
        {
            for (j = 0;j<dimension;j++) 
            {
                last_centroids[i][j] = centroids[i][j];
            }
        }
    }

    /* free used memory */
    for (i = 0; i<k;i++) 
    {
        free(last_centroids[i]); /* free each centroid dimensions array*/
    }
    free(last_centroids);/*free centorid array*/
    free(Cluster);/*free Cluster array*/
    for (i = 0; i<n; i++) 
    {
        free(datapoints[i]);/* free each dataPoint dimensions array*/
    }
    free(datapoints);/*free DataPoint array*/
}