#ifndef HYPERCUBE_HPP
#define HYPERCUBE_HPP

#include <ctime>
#include <vector>
#include <unordered_map>
#include <set>
#include <cmath>
#include <queue>
using namespace std;

#include "Dist.hpp"
#include "Image.hpp"
#include "FFunc.hpp"
#include "LSH.hpp"

#define DEFAULT_m pow(2, 13) + 1 
#define DEFAULT_M pow(2, 30)



int hammingDistance(int n1, int n2);

queue<int> hammingNeighbors(int probes, int number, int d1);



class Hypercube {

    int d1;

    std::vector<FFunc> f;

    std::unordered_multimap<int, Image> cube;

public:

    int hash(Image img);

    Hypercube(int d1, int w = 40);
    void insert(Image img);

    Image bfSearch(Image query);
    Image nnSearch(Image query, int probes, int M);
    std::vector<Image> rangeSearch(Image query, double R, int probes, int M);
    
    std::vector<Pair> knnSearch(int M,int k,int probes,Image query);
    std::vector<Pair> bfknnSearch(Image query);
};

#endif // HYPERCUBE_HPP
