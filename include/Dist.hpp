#ifndef DIST_HPP
#define DIST_HPP

#include "Image.hpp"

#include <cmath>

enum DistMethod {
	Manhattan,
	Euclidean,
	Emd,
};

double dist(Image img1, Image img2, int method = Manhattan);

struct Pair {
	Image img;
	unsigned int dist;
	
	Pair(Image img1, unsigned int d);
		
};

bool compareDist(Pair p1, Pair p2);

#endif // DIST_HPP