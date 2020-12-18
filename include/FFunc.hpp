#ifndef FFUNC_HPP
#define FFUNC_HPP

#include <stdlib.h>

#include <cmath>

#include "HFunc.hpp"
#include "Image.hpp"
#include "ModOp.hpp"

class FFunc {

    HFunc h;

    // Randomizer
    unsigned int r;

public:
    FFunc();
    FFunc(unsigned int m, unsigned int M, unsigned int w, int seed); 
    void init(unsigned int m, unsigned int M, unsigned int w, int seed);
    int operator() (const Image& img) const;

};

#endif // FFUNC_HPP