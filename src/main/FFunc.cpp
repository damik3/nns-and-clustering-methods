#include "FFunc.hpp"



FFunc::FFunc() 
    {}



FFunc::FFunc(unsigned int m, unsigned int M, unsigned int w, int seed, int img_size) {
    this->init(m, M, w, seed, img_size);
}



void FFunc::init(unsigned int m, unsigned int M, unsigned int w, int seed, int img_size) {
    h.init(m, M, w, seed, img_size);
    srand(seed);
    this->r = rand();

    // std::cout << "FFunc int: seed = " << seed << ", r = " << r << std::endl;
}



int FFunc::operator() (const Image& img) const {

    // return ( h(img) + random_value ) % 2
    return addMod(h(img), r, 2);
}