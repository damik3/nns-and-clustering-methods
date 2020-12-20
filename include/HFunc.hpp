#ifndef HFUNC_HPP
#define HFUNC_HPP

#include <stdlib.h>

#include <cmath>

#include "Image.hpp"
#include "ModOp.hpp"



class HFunc {

    unsigned int m;
    unsigned int M;
    unsigned int w;
    int img_size;
    int* s;

public:

    HFunc();
    HFunc(unsigned int m, unsigned int M, unsigned int w, int seed, int img_size);
    void init(unsigned int m, unsigned int M, unsigned int w, int seed, int img_size);
    size_t operator() (const Image& img) const;

};


#endif // HFUNC_HPP