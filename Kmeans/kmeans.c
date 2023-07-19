#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/* declare all functions*/
void centroids_calculation(double **data ,int *clusters ,double **centroids,int n,int k,int len);
int *cluster_calculation(double **data, double **centroids, int n, int k, int dimension);
double distance(double *vector1, double *vector2, int dimension);
void kmeans(double **dataPoint, int n,int k,int dimension,int iter,double eps); 

/* calculate clusters centroid */
void centroids_calculation(double **data ,int *clusters ,double **centroids,int n,int k,int len) 
{ 
    int *sizeCluster = (int *)malloc(k * sizeof(double));
    int i, j;

    /* zerofiy */
    for (i=0;i<k;i++) {
        sizeCluster[i] = 0;
        for (j=0;j<len;j++) 
        {
            centroids[i][j] = 0;
        }
    }

    /* set cluster with all the values */
    for (i = 0; i < n; i++) {
        int cluster = clusters[i];
        sizeCluster[cluster]++;
        for (j=0;j<len;j++) 
        {
            centroids[cluster][j] += data[i][j];
        }
    }

    /* calculate centroid*/
    for (i=0;i<k;i++) {
        if (sizeCluster[i]>0) {
            for (j=0;j<len;j++) 
            {
                centroids[i][j] /= sizeCluster[i];
            }
        }
    }

    /* free memory*/
    
    free(sizeCluster);
}
#define INFINITY (__builtin_inff ());
/* calculate what cluster to assign each point */
int *cluster_calculation(double **data, double **centroids, int n, int k, int dimension) 
{
    int *dataPoints, i, j;
    int p=-1;
    double min_dist;
    dataPoints = malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        min_dist = INFINITY;
        p=-1;
        for (j=0; j<k;j++) {
            double dist = distance(data[i], centroids[j], dimension);
            if (dist<min_dist) 
            {
                min_dist= dist; 
                p= j;
            }
        }
        dataPoints[i] = p; /*assign to cluster*/
    }
    return dataPoints;
}


/* calculate euclidean distance */
double distance(double *vector1, double *vector2, int dimension) {
    double dist = 0;
    int i;
    for (i = 0;i<dimension;i++) {
        dist += (vector1[i] - vector2[i]) * (vector1[i] - vector2[i]);
    }
    return sqrt(dist);
}


int main(int argc, char *argv[]) {
    int NumOfP,k, vec, c, iter, dimension;
    int flag;
    double **dataPoint,eps;
    char charCount;
    /*char *filePath, *line, *pline;
    FILE *fp;
    size_t len;*/
    eps = 0.001;
    flag=0;
    /* check if input is valid */
    
    if(argc<2 || argc>3)
    {
        printf("An Error Has Occured \n");
        return 1;
    }
    if(3==argc)
    {
        k = atoi(argv[1]);
        iter = atoi(argv[2]);
    }
    if(2==argc)
    {
        k = atoi(argv[1]);
        iter = 200;
    }
    if(k<=1)
    {
        printf("Invalid number of clusters!\n");
        flag=1;
    }
    if(iter<=1 || iter>=1000)
    {
        printf("Invalid maximum iteration!\n");
        flag=1;
    }
    if(flag==1)
    {
        return 1;
    }
    (void)argc;
    (void)flag;
    
    /* calculate how many lines(points), and the dimension of the space */
    NumOfP=0;
    dimension = 1;
    while ((charCount = getchar()) != EOF) /* switch with fget? */
    {
        if (charCount == '\n') {
            NumOfP=NumOfP+1;
        } 
        else{
            if (NumOfP == 0 && charCount == ',') 
            {
            dimension=dimension+1;
            } 
        }
    }
    rewind(stdin); /* reset pointer*/
    if(k>NumOfP)
    {
        printf("Invalid number of clusters!\n");
        return 1;
    }
    
    /* input data into 2D array*/
    dataPoint = (double **)malloc(NumOfP * sizeof(double *));
    for (vec=0;vec<NumOfP;vec++) {
        dataPoint[vec] = (double *)malloc(dimension * sizeof(double));
        for (c=0;c<dimension;c++) 
        {
            if (scanf("%lf,", &dataPoint[vec][c]) != 1) /* check data assigned successfully */
            { 
                printf("An Error Has Occurred\n");
                return 1;
            }
        }
    }
    kmeans(dataPoint,NumOfP,k,dimension,iter,eps);
    return 0;
}

void kmeans(double **dataPoint, int n,int k,int dimension,int iter,double eps) 
{
    double **centroids;
    double **last_centroids;
    int i,j, count, convergence;
    int* Cluster;
    convergence=1;
    Cluster = NULL;
    /* Create first k Centroids */
    centroids = malloc(k * sizeof(double *));
    for (i = 0; i < k; i++) 
    {
        centroids[i] = malloc(dimension*sizeof(double));
        for (j=0; j<dimension; j++) 
        {
            centroids[i][j] = dataPoint[i][j];
        }
    }

    /* Initialize the previous centroids */
    last_centroids = malloc(k*sizeof(double *));
    for (i = 0; i < k; i++) {
        last_centroids[i] = malloc(dimension*sizeof(double));
        for (j =0 ; j<dimension;j++) 
        {
            last_centroids[i][j] = centroids[i][j];
        }
    }

    
    for (count = 0;count<iter;count++) 
    {
        Cluster = cluster_calculation(dataPoint,centroids,n,k,dimension);/* create clusters */
        centroids_calculation(dataPoint, Cluster,centroids,n,k,dimension);/* update Centorids */
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

        free(Cluster);
        for (i = 0; i < k; i++) /* Update Clusters and previous centroids */
        {
            for (j = 0;j<dimension;j++) 
            {
                last_centroids[i][j] = centroids[i][j];
            }
        }
    }

    for (i = 0; i < k; i++) {
        for (j=0; j<dimension; j++) 
        {
            printf("%.4f", centroids[i][j]);
            if (j <dimension - 1) 
            {
                printf(",");
            }
        }
        printf("\n");
    }

    /* free used memory */
    for (i = 0; i<k;i++) 
    {
        free(centroids[i]); /* free each centroid dimensions array*/
        free(last_centroids[i]); /* free each centroid dimensions array*/
    }
    free(centroids);/*free centorid array*/
    free(last_centroids);/*free centorid array*/
    free(Cluster);/*free Cluster array*/
    for (i = 0; i<n; i++) 
    {
        free(dataPoint[i]);/* free each dataPoint dimensions array*/
    }
    free(dataPoint);/*free DataPoint array*/
}