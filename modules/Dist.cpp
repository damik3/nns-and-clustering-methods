#include "Dist.hpp"

#include <numeric>

using namespace std;


template <class T>
double ManhattanDistance(vector<T> v1, vector<T> v2); 

template <class T>
double EuclideanDistance(vector<T> v1, vector<T> v2);

template <class T>
double EmdDistance(vector<T> v1, vector<T> v2, int num_clusters);



double dist(Image img1, Image img2, int method, int emd_n) {
    
    if (method == Manhattan)
        return ManhattanDistance<PIXEL_T>(img1.pixels, img2.pixels);

    else if (method == Euclidean) 
        return EuclideanDistance<PIXEL_T>(img1.pixels, img2.pixels);

    else if (method == Emd)
        return EmdDistance(img1.pixels, img2.pixels, emd_n);

    else
        throw std::runtime_error("dist: method not defined!");
    
}



template <class T>
double ManhattanDistance(vector<T> v1, vector<T> v2) {
    
    if (v1.size() != v2.size())
        throw std::runtime_error("Dist.ManhattanDistance: Invalid vector sizes!");

    double ret = 0;

    for (int i=0; i<(int)v1.size(); i++)
        ret += abs(v1[i] - v2[i]);

    return ret;
}



template <class T>
double EuclideanDistance(vector<T> v1, vector<T> v2) {
    
    if (v1.size() != v2.size())
        throw std::runtime_error("Invalid image sizes!");

    double ret = 0;

    for (int i=0; i<(int)v1.size(); i++)
        ret += pow(v1[i] - v2[i], 2);

    return sqrt(ret);
}




template <class T>
double EmdDistance(vector<T> v1, vector<T> v2, int num_clusters) {

    // Check validity

    if (num_clusters == 0)
        throw std::overflow_error("Dist.EmdDistance: Divide by zero exception");

    if (v1.size() != v2.size())
        throw std::runtime_error("Dist.EmdDistance: Invalid image sizes!");

    // Assume vector size is a perfect square
    int v_size = v1.size();
    if (!is_perfect_square(v_size))
        throw std::runtime_error("Dist.EmdDistance: cluster size not a perfect square");

    // Cluster size must have an integer square root in order for the cluster to be a square
    int cluster_size = v1.size() / num_clusters;
    if (!is_perfect_square(cluster_size))
        throw std::runtime_error("Dist.EmdDistance: cluster size not a perfect square");



    // Size of one dimension of cluster
    // Both dimensions are the same, since a cluster is a square
    int r = sqrt(cluster_size);

    // Compute the sum of each image, for normalization later
    long double sum1 = accumulate(v1.begin(), v1.end(), 0);
    long double sum2 = accumulate(v2.begin(), v2.end(), 0);

    // Each cluster is represented by a pair p. For example
    // p.first = (2, 3), its "location"
    // p.second = 20, its sum of weights
    // We want an array of clusters for each image, so we have
    vector<pair<pair<int, int>, double> > cluster1(num_clusters);
    vector<pair<pair<int, int>, double> > cluster2(num_clusters);

    for (int i=0; i<num_clusters; i++) {
        cluster1[i].first = cluster_pos(v1, i, r);
        cluster1[i].second = cluster_sum(v1, cluster1[i].first, r) / sum1;
        cluster2[i].first = cluster_pos(v2, i, r);
        cluster2[i].second = cluster_sum(v2, cluster2[i].first, r) / sum2;
    }



    // Return optimal solution of emd modeled as linear programming
    return lp_solve(cluster1, cluster2);
}



bool is_perfect_square(double n) {
    double r = sqrt(n);
    return r == floor(r);
}



// Returns position of n-th cluster (e.g. (1, 3))
// Cluster size is r*r
template <class T>
pair<int, int> cluster_pos(vector<T> v, int n, int r) {

    pair<int, int> ret;

    // num_rows = num_cols for a 2d square vector
    int num_cols = (int)sqrt(v.size());
    int cluster_size = r*r;
    int clusters_per_row = num_cols / r;
    int num_clusters = v.size() / cluster_size;

    if ((n < 0) || (n >= num_clusters))
        throw runtime_error("Dist. cluster_pos: Invalid cluster number!");

    ret.first = r * (n % clusters_per_row);
    ret.second = r * (n / clusters_per_row);

    return ret;
}



// Returns sum of cluster indentified by cluster_pos (e.g. cluster_pos = (2, 2))
// Cluster size is r*r
template <class T>
double cluster_sum(vector<T> v, pair<int, int> cluster_pos, int r) {

    double ret = 0;
    int count = r*r;
    int num_cols = (int)sqrt(v.size());
    int num_rows = num_cols;

    if ((cluster_pos.first < 0) || (cluster_pos.first >= num_cols) || (cluster_pos.second < 0) || (cluster_pos.second >= num_rows))
        throw runtime_error("Dist. cluster_sum: Invalid cluster position!");

    // v is a 1d vector
    // but represents 2d array
    // v starts counting from upper left corner of the 2d array, that is its (0, 0) position
    // for some reason, we decided that for cluster(i, j) we start counting for bottom left corner
    // thus the (num_rows - j - 1) coefficient
    for (int j=cluster_pos.second; 0 < count; j = (j+1) % r + cluster_pos.second) {
        for (int i=cluster_pos.first; i < cluster_pos.first + r ; i++) {
            ret += v[ (num_rows - j - 1)*num_cols + i ];
            count--;
        }
    }

    return ret;
}




template <class T>
double lp_solve(std::vector<T> cluster1, std::vector<T> cluster2) {
    
}



Pair::Pair(Image img1, unsigned int d)
    : img(img1), dist(d) 
    {}



bool compareDist(Pair p1, Pair p2){
	return p1.dist < p2.dist;
}