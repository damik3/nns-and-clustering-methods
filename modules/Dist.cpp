#include "Dist.hpp"

#include <numeric>

using namespace std;


template <class T>
double ManhattanDistance(vector<T> v1, vector<T> v2); 

template <class T>
double EuclideanDistance(vector<T> v1, vector<T> v2);

template <class T>
double EmdDistance(vector<T> v1, vector<T> v2, int num_clusters);

bool is_perfect_square(double n);

template <class T>
pair<int, int> cluster_pos(vector<T> v, int n, int r);

template <class T>
double cluster_sum(vector<T> v, pair<int, int> cluster_pos);




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

    if (num_clusters == 0)
        throw std::overflow_error("Dist:Divide by zero exception");

    if (v1.size() != v2.size())
        throw std::runtime_error("Dist:Invalid image sizes!");

    // Assume vector size is a perfect square
    int v_size = v1.size();
    if (!is_perfect_square(v_size))
        throw std::runtime_error("Dist: cluster size not a perfect square");

    // Cluster size must have an integer square root in order for the cluster to be a square
    int cluster_size = v1.size() / num_clusters;
    if (!is_perfect_square(cluster_size))
        throw std::runtime_error("Dist: cluster size not a perfect square");

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
        cluster1[i].second = cluster_sum<PIXEL_T>(v1, cluster1[i].first) / sum1;
        cluster2[i].first = cluster_pos(v2, i, r);
        cluster2[i].second = cluster_sum<PIXEL_T>(v2, cluster2[i].first) / sum2;
    }

    return 0;
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
    int clusters_per_row = num_cols / cluster_size;

    int i = n % clusters_per_row;
    int j = n / clusters_per_row;

    ret.first = i * r;
    ret.second = j * r;

    return ret;
}



// Returns sum of cluster indentified by cluster_pos 
template <class T>
double cluster_sum(vector<T> v, pair<int, int> cluster_pos) {

    double ret = 0;



    return ret;
}



Pair::Pair(Image img1, unsigned int d)
    : img(img1), dist(d) 
    {}



bool compareDist(Pair p1, Pair p2){
	return p1.dist < p2.dist;
}