#include "Dist.hpp"

unsigned int ManhattanDistance(Image img1, Image img2);
double EuclideanDistance(Image img1, Image img2);
double EmdDistance(Image img1, Image img2);



double dist(Image img1, Image img2, int method) {
    
    if (method == Manhattan)
        return (double) ManhattanDistance(img1, img2);

    else if (method == Euclidean) 
        return EuclideanDistance(img1, img2);

    else if (method == Emd)
        return EmdDistance(img1, img2);

    else
        throw std::runtime_error("dist: method not defined!");
    
}




unsigned int ManhattanDistance(Image img1, Image img2) {
    
    if (img1.size != img2.size)
        throw std::runtime_error("Invalid image sizes!");

    unsigned int ret = 0;

    for (int i=0; i<img1.size; i++)
        ret += abs(img1.pixels[i] - img2.pixels[i]);

    return ret;
}




double EuclideanDistance(Image img1, Image img2) {

    if (img1.size != img2.size)
        throw std::runtime_error("Invalid image sizes!");

    double ret = 0.0;

    for (int i=0; i<img1.size; i++)
        ret += pow(img1.pixels[i] - img2.pixels[i], 2);

    return sqrt(ret);
}




double EmdDistance(Image img1, Image img2) {
    return 0;
}




Pair::Pair(Image img1, unsigned int d)
    : img(img1), dist(d) 
    {}



bool compareDist(Pair p1, Pair p2){
	return p1.dist < p2.dist;
}