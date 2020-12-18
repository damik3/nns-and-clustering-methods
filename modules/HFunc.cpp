#include "HFunc.hpp"



HFunc::HFunc() 
    {}



HFunc::HFunc(unsigned int m, unsigned int M, unsigned int w, int seed) {
    this->init(m, M, w, seed);
}



void HFunc::init(unsigned int m, unsigned int M, unsigned int w, int seed) {
    this->m = m;
    this->M = M;
    this->w = w;

    srand(seed);


    for (int i=0; i<IMGSIZ; i++) {
        s[i] = rand() % w;
        // std::cout << " s[" << i << "] = " << s[i] << std::endl;
    }

    // std::cout << "HFunc initialized with s[0] = " << s[0] << std::endl;
    // std::cout << m << std::endl;
    // std::cout << M << std::endl;
    // std::cout << w << std::endl;

}



size_t HFunc::operator()(const Image& img) const {
    
    size_t ret = 0;

    for (int i=0; i<IMGSIZ; i++) {
        // ret += pow(m, IMGSIZ - i -1) * floor( (img.pixels[i] - s[i]) / double(w) );
        ret = addMod(ret, multMod( powMod(m, IMGSIZ - i - 1, M), floor( (img.pixels[i] + s[i]) / double(w)), M), M);
        // std::cout << "ret = " << ret << std::endl;
    }

    return ret;

}