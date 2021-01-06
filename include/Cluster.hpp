#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <time.h>
#include <stdlib.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "Dist.hpp"
#include "HFunc.hpp"
#include "Hypercube.hpp"
#include "Image.hpp"
#include "LSH.hpp"

#define METHOD_CLASSIC "Classic"
#define METHOD_LSH "LSH"
#define METHOD_HYPERCUBE "Hypercube"
#define MAX_CHANGE 1000
#define HYPERCUBE_w 40




/*

int binSea(std::vector<unsigned int> A, int x, unsigned int pos1, unsigned int pos2);

int newCentroidPos(std::vector<Image> input, std::vector<Image> centroid, int t);

Image median(std::vector<Image> images);

*/

std::vector<std::vector<int> > parse_clustering(string& filename);

struct Cluster {

    int k;
    std::string m;

    std::vector<Image> centroid;                // size k
    std::vector<std::vector<Image> > cluster;   // size k

    Cluster();

    Cluster(int k, std::string m, 
        std::vector<Image> input,
        int number_of_vector_hash_tables,
        int number_of_vector_hash_functions,
        int max_number_M_hypercube,
        int number_of_hypercube_dimensions,
        int number_of_probes);

    Image getCentroid(int c);
    
    size_t getClusterSize(int c);

    std::vector<Image> getCluster(int c);

    std::vector<double> silhouette(); 

    double objective_f();

    void display();

    void replace(std::vector<Image> new_image);

};

#endif // CLUSTER_HPP