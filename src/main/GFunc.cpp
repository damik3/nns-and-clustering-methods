#include "GFunc.hpp"



unsigned int concat(std::vector<size_t> hnumbers, int k) {
	size_t res=0;
	for(unsigned int i=0; i<hnumbers.size();i++){
		res = res << (sizeof(int) * 8 / k);
		res = res | hnumbers[i];
	}
	return res;
}



GFunc::GFunc(unsigned int m,unsigned int M,unsigned int w,unsigned int k, int seed, int img_size){
	this->k=k;
	HFunc h;

	for(unsigned int i=0; i<this->k; i++){
		h.init(m, M, w, seed + i, img_size);
		hashf.push_back(h);
	}
    
}



size_t GFunc::operator() (const Image& img) const {

    std::vector<size_t> hashes;

    for (unsigned int i=0; i<this->k; i++)
        hashes.push_back(hashf[i](img));

    return concat(hashes, this->k);
}