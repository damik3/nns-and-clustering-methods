#include "Dist.hpp"



unsigned int dist(Image img1, Image img2) {
    
    if (img1.size != img2.size)
        throw std::runtime_error("Invalid image sizes!");

    unsigned int ret = 0;

    for (int i=0; i<img1.size; i++)
        ret += abs(img1.pixels[i] - img2.pixels[i]);

    return ret;
}



Pair::Pair(Image img1, unsigned int d)
    : img(img1), dist(d) 
    {}



bool compareDist(Pair p1, Pair p2){
	return p1.dist < p2.dist;
}