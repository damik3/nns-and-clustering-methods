#include "Dist.hpp"



unsigned int dist(Image img1, Image img2) {
    
    unsigned int ret = 0;

    for (int i=0; i<IMGSIZ; i++)
        ret += abs(img1.pixels[i] - img2.pixels[i]);

    return ret;
}



Pair::Pair(Image img1, unsigned int d){
	this->img = img1;
	this->dist = d;
}



bool compareDist(Pair p1, Pair p2){
	return p1.dist < p2.dist;
}