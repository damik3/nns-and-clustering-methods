import sys
import math
import numpy as np

from Image import Image

def is_perfect_square(s):
    r = math.sqrt(s)
    return r == np.floor(r)

def EmdDistance(img1, img2, num_clusters):

    p1 = img1.pixels
    p2 = img2.pixels

    if num_clusters == 0:
        print("dist.EmdDistance error : Divided by zero !")
        sys.exit()


    if p1.size != p2.size:
        print("dist.EmdDistance error : Invalid vector sizes")
        sys.exit()

    cluster_size = p1.size / num_clusters
    if is_perfect_square(cluster_size) == 0:
        print("dist.EmdDistance error : cluster size not a perfect square")
        sys.exit()

    r = math.sqrt(cluster_size)

    sum1 = np.sum(p1)
    sum2 = np.sum(p2)

    print(sum1)
    
def EuclideanDistance(img1,img2):

    p1 = img1.pixels
    p2 = img2.pixels
    
    if p1.size != p2.size:
        print("dist.ManhattanDistance error : Invalid vector sizes")
        sys.exit()


    ret = 0;
    for i in range(img1.numrows):
        for j in range(img1.numcols):
            ret += pow(p1[i][j] - p2[i][j], 2)

    r = math.sqrt(ret)

    return r

def dist(img1, img2, method, n):
    if method == "Euclidean":
        ret = EuclideanDistance(img1, img2)
        return ret
    elif method == "EMD":
        ret = EmdDistance(img1, img2, n)
        return
    else:
        print("Invalid method")
        sys.exit()

