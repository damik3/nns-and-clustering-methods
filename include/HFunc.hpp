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
    int s[IMGSIZ];

public:

    HFunc();
    HFunc(unsigned int m, unsigned int M, unsigned int w, int seed);
    void init(unsigned int m, unsigned int M, unsigned int w, int seed);
    size_t operator() (const Image& img) const;

};


#endif // HFUNC_HPP