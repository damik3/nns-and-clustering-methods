#ifndef LSH_HPP
#define LSH_HPP

#include <time.h>

#include <cmath>
#include <new>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "Dist.hpp"
#include "Image.hpp"
#include "GFunc.hpp"

#define DEFAULT_w 40



class LSH {

    unsigned int m;
    unsigned int M;
    int w;
    int k;
    int L;

    std::vector<std::unordered_set<Image, GFunc> *> hashTables;

public:

    LSH(unsigned int w, int k, int L, int numImages, int img_size);
    ~LSH();

    void insert(Image img);
    Image bfSearch(Image query);
    Image nnSearch(Image query);
    std::vector<Image> rangeSearch(Image query,double R);

    std::vector<Pair> knnSearch(Image query, int k);
    std::vector<Pair> bfknnSearch(Image query, int k);


};

#endif // LSH_HPP
