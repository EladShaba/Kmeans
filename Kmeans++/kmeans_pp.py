import numpy as np
import pandas as pd
import math
import sys
import mykmeanssp as kmeans


def CheckLegal(k, N, iter):
    # Check arguments are valid:
    flag=0
    if not(k > 1 and k < N):
        print("Invalid number of clusters!")
        flag=1
    if not(iter > 1 and iter < 1000):
        print("Invalid maximum iteration!")
        flag=1
    if(1==flag):
        sys.exit(1)


def distance(dataPoint, centroid):
    dist = 0
    for i in range(len(centroid)):
        dist += (centroid[i] - dataPoint[i])**2
    return math.sqrt(dist)


def minDistCentroid(dataPoint, centroidsLst):
    min_distance = float("inf")
    for ind in range(len(centroidsLst)):
        dist = distance(dataPoint, centroidsLst[ind])
        if dist < min_distance:
            min_distance = dist

    return min_distance

        
if __name__ == "__main__":
    #extract information from sys.argv
    sys_args_len = len(sys.argv)
    
    if(sys_args_len > 6 or sys_args_len <5 ):
        print("An Error Has Occurred")
        sys.exit(1)

    k = int(sys.argv[1])
    
    if sys_args_len == 5:
        iter = 300
        eps = float(sys.argv[2])
        file1 = sys.argv[3]
        file2 = sys.argv[4]
    else:
        iter = int(sys.argv[2])
        eps = float(sys.argv[3])
        file1 = sys.argv[4]
        file2 = sys.argv[5]
    

    # Open both files in read mode
    df_file1 = pd.read_csv(file1, sep=",", header=None)
    df_file2 = pd.read_csv(file2, sep=",", header=None)
    
    #rename the first column to keys (as per what it does)
    df_file1.rename(columns = {0:'Keys'}, inplace = True)
    df_file2.rename(columns = {0:'Keys'}, inplace = True)
    
    #merge both files - inner merge
    merged_df = pd.merge(df_file1, df_file2, on='Keys')
    
    #sort in ascending order by keys column
    merged_df.sort_values(by=['Keys'], inplace=True)
    
    #drop the keys column
    merged_df.drop(['Keys'], axis=1, inplace=True)
    
    #create a numpy array containing all the points
    datapoints = merged_df.values
    
    # N = number of datapoints in the numpy array , dimension = the dimension of each datapoint
    N = len(datapoints)
    dimension = len(datapoints[0])

    #check that all the information above is legal
    CheckLegal(k, N, iter)

    #set seed
    np.random.seed(0)
    
    #start kmeans++ algorithm stage 1
    rand_int = np.random.choice(N)
    
    #create a new list that will contain the centroids 
    K_centroids = []
    K_centroids.append(datapoints[rand_int].tolist())
    
    #a list of the indexes of said chosen centroids
    rand_int_chosen_arr = []
    rand_int_chosen_arr.append(rand_int)

    #create two new numpy arrays that will contain D(x) and the corresponding probabilities
    D_x = np.zeros(N)
    prob = np.zeros(N)
    
    for i in range(1, k):
        for ind in range(N):
            if ind not in rand_int_chosen_arr:
                #calculate D(x) for datapoints[ind]
                D_x[ind] = minDistCentroid(datapoints[ind], K_centroids)
            else:
                D_x[ind]=0
            
        sum_dists = np.sum(D_x)    
        for j in range(N):
            prob[j] = D_x[j]/sum_dists
        
        rand_int = np.random.choice(N, p = prob)
        
        #add the chosen centroid to the list
        K_centroids.append(datapoints[rand_int].tolist())
        
        #add the chosen centroid index to the list
        rand_int_chosen_arr.append(rand_int)
        
    #transform the numpy arrays to python lists - in the c module numpy arrays are not PyLists
    datapoints_lst = datapoints.tolist()
    
    K_centroids_lst = kmeans.fit(datapoints_lst, K_centroids, N, k, dimension, iter, eps)
    
    print(",".join(str(int(x)) for x in rand_int_chosen_arr))
    for centroid in K_centroids_lst:
        print(",".join(str("{:.4f}".format(round(x, 4))) for x in centroid))
    


