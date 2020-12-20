#ifndef GFUNC_HPP
#define GFUNC_HPP

#include <vector>
#include <iostream>

#include "HFunc.hpp"
#include "Image.hpp"



unsigned int concat(std::vector<size_t> hnumbers, int k);



class GFunc {
	
	std::vector<HFunc> hashf;
	unsigned int k;
	
public:
	
	GFunc(unsigned int m, unsigned int M, unsigned int w, unsigned int k, int seed, int img_size);
	size_t operator() (const Image& img) const;
	
};

#endif

