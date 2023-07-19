import math
import sys

class Cluster(object):
    def __init__(self, dataPoint):
        self.centroid = dataPoint.copy()
        self.sumDataPoints = dataPoint.copy()
        self.size = 1

    def distance(self, dataPoint):
        dist = 0
        for i in range(len(self.centroid)):
            dist += (self.centroid[i] - dataPoint[i])**2
        return math.sqrt(dist)

    def addDataPoint(self, dataPoint):
        for i in range(len(self.centroid)):
            self.sumDataPoints[i] += dataPoint[i]
        self.size += 1


def minDistCluster(dataPoint, clusterLst):
    min_distance = float("inf")
    min_cluster = 0
    for cluster in clusterLst:
        dist = cluster.distance(dataPoint)
        if dist < min_distance:
            min_distance = dist
            min_cluster = cluster

    return min_cluster


def deltaCentroids(clusterLst, prevCentroidsLst):
    for i in range(len(clusterLst)):
        if clusterLst[i].distance(prevCentroidsLst[i]) >= 0.001:
            return False

    return True

def countLines(file):
    N=0
    for line in file:
        if line != "":
            N+=1
    file.close()
    return N

def CheckLegal(k,N,iter):
    if(N==0):
        #print error
        print("An Error Has Occurred")
        sys.exit(1)
        try:
            if(k<1 or k>N or int(k)!=k):
                print("Invalid number of clusters!")
                sys.exit(1)
        except:
            print("Invalid number of clusters!")
            sys.exit(1)
        
    elif(iter<1 or iter>1000 or int(iter)!=iter):
        print("Invalid maximum iteration!")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) >3:
        k = int(sys.argv[1])
        iter = int(sys.argv[2])
        fileName = sys.argv[3]
    else:
        k = int(sys.argv[1])
        iter = 200
        fileName = sys.argv[2]

    # Open the file in read mode
    try:
        file = open(fileName, 'r')
    except:
        print("An Error Has Occurred")
        sys.exit(1)

    
    N=countLines(file)
    CheckLegal(k,N,iter)
    
    # Open the file in read mode
    file = open(fileName, 'r')
    
    clusterLst = []
    for i in range(k):
        line = file.readline()
        dataPointStrLst = line.split(",")
        dataPointLst = [float(num) for num in dataPointStrLst]
        clusterLst.append(Cluster(dataPointLst))

    iterationNumber = 0
    while iterationNumber < iter:
        while True:
            line = file.readline()
            if line == "":
                break
            dataPointStrLst = line.split(",")
            dataPointLst = [float(num) for num in dataPointStrLst]
            closest_cluster = minDistCluster(dataPointLst, clusterLst)
            closest_cluster.addDataPoint(dataPointLst)

        prevCentroidsLst = [cluster.centroid.copy() for cluster in clusterLst]
        for cluster in clusterLst:
            for i in range(len(cluster.sumDataPoints)):
                cluster.centroid[i] = cluster.sumDataPoints[i]/cluster.size
                cluster.sumDataPoints[i] = 0
            cluster.size = 0

        if deltaCentroids(clusterLst, prevCentroidsLst):
            break;

        iterationNumber += 1
        file.close()
        file = open(fileName, 'r')


    for cluster in clusterLst:
        print(",".join("%.4f" % num for num in cluster.centroid))

    # Close the file
    file.close()
    


