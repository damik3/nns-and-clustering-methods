#ifndef DIST_HPP
#define DIST_HPP

#include "Image.hpp"

#include <cmath>

#define DIST_DEFAULT_SIGNATURE_SIZE 4*4



enum DistMethod {
	Manhattan,
	Euclidean,
	Emd,
};



double dist(Image img1, Image img2, int method = Manhattan, int emd_n = DIST_DEFAULT_SIGNATURE_SIZE);



bool is_perfect_square(double n);



template <class T>
std::pair<int, int> cluster_pos(std::vector<T> v, int n, int r);



template <class T>
double cluster_sum(std::vector<T> v, std::pair<int, int> cluster_pos, int r);



template <class T>
double lp_solve(std::vector<T> cluster1, std::vector<T> cluster2);



struct Pair {
	Image img;
	unsigned int dist;
	
	Pair(Image img1, unsigned int d);
		
};



bool compareDist(Pair p1, Pair p2);

#endif // DIST_HPP