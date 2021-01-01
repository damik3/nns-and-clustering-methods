#include "HFunc.hpp"



HFunc::HFunc() 
    {}



HFunc::HFunc(unsigned int m, unsigned int M, unsigned int w, int seed, int img_size1) {
    this->init(m, M, w, seed, img_size1);
}



void HFunc::init(unsigned int m, unsigned int M, unsigned int w, int seed, int img_size1) {
    this->m = m;
    this->M = M;
    this->w = w;
    this->img_size = img_size1;
    this->s = new int[img_size1];

    srand(seed);


    for (int i=0; i<img_size1; i++) {
        s[i] = rand() % w;
        // std::cout << " s[" << i << "] = " << s[i] << std::endl;
    }

    // std::cout << "HFunc initialized with s[0] = " << s[0] << std::endl;
    // std::cout << m << std::endl;
    // std::cout << M << std::endl;
    // std::cout << w << std::endl;

}



size_t HFunc::operator()(const Image& img) const {
    
    if (this->img_size != img.size)
        throw std::runtime_error("HFunc.operator(): Invalid image sizes!");

    size_t ret = 0;

    for (int i=0; i<img.size; i++) {
        // ret += pow(m, IMGSIZ - i -1) * floor( (img.pixels[i] - s[i]) / double(w) );
        ret = addMod(ret, multMod( powMod(m, img.size - i - 1, M), floor( (img.pixels[i] + s[i]) / double(w)), M), M);
        // std::cout << "ret = " << ret << std::endl;
    }

    return ret;

}