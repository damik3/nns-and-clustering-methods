#include "FFunc.hpp"



FFunc::FFunc() 
    {}



FFunc::FFunc(unsigned int m, unsigned int M, unsigned int w, int seed) {
    this->init(m, M, w, seed);
}



void FFunc::init(unsigned int m, unsigned int M, unsigned int w, int seed) {
    h.init(m, M, w, seed);
    srand(seed);
    this->r = rand();

    // std::cout << "FFunc int: seed = " << seed << ", r = " << r << std::endl;
}



int FFunc::operator() (const Image& img) const {

    // return ( h(img) + random_value ) % 2
    return addMod(h(img), r, 2);
}