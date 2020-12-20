#include "LSH.hpp"



LSH::LSH(unsigned int w, int k, int L, int numImages, int img_size) {
    this->m = pow(2, 13) + 1;
    this->M = pow(2, 32 / k);
    this->w = w;
    this->k = k;
    this->L = L;    

    std::unordered_set<Image, GFunc>* hashTable;

    for (int i=0; i<L; i++) {

        hashTable = new std::unordered_set<Image, GFunc>(1, GFunc(m, M, w, k, time(NULL) + i*k, img_size));

        // Set number of images per bucket;
        hashTable->max_load_factor(16.0);
        
        hashTables.push_back(hashTable);
    }
}



LSH::~LSH() {

    for (int i=0; i<this->L; i++) 
        delete hashTables[i];
}



void LSH::insert(Image img) {

    for (int i=0; i<this->L; i++)
        hashTables[i]->insert(img);

}


Image LSH::bfSearch(Image query) {
    // Nearest neighbor
    Image nn(query.num_rows, query.num_cols, query.pixel_size);

    // Distance of nn
    unsigned int d = pow(2, 32) - 1;

    for (auto it = hashTables[0]->begin(); it != hashTables[0]->end(); ++it) {

        // Ignore the same image
        if (query == *it)
            continue;

        if (dist(query, *it) < d) {
            nn = *it;
            d = dist(query, *it);    
            // std::cout << "\nnew d = " << d << std::endl;
        }
    }

    return nn;
}



Image LSH::nnSearch(Image query) {
    // Nearest neighbor
    Image nn(query.num_rows, query.num_cols, query.pixel_size);

    // Distance of nn
    unsigned int d = pow(2, 32) - 1;

    // Bucket number
    int b;

    // For each hash table
    for (int i=0; i<L; i++) {

        //std::cout << "\nChecking " << i << " hashTable, with bucket count = " << hashTables[i]->bucket_count() 
          //  << " and loading factor " << hashTables[i]->load_factor() << std::endl;

        b = hashTables[i]->bucket(query);
        //std::cout << "Items in current bucket = " << hashTables[i]->bucket_size(b) << std::endl;

        // For each image in appropriate bucket
        for (auto it = hashTables[i]->begin(b); it != hashTables[i]->end(b) ; ++it) {
            
            // std::cout << "\nBucket also contains" << std::endl;
            // it->print();

            if (dist(query, *it) < d) {
                nn = *it;
                d = dist(query, *it);    
                //std::cout << "\nnew d = " << d << std::endl;
            }
        }
    }

    std::cout << "of distance d = " << d << std::endl;;

    return nn;
}



std::vector<Image> LSH::rangeSearch(Image img, double R){
	
    std::vector<Image> ret;

	for (int i=0; i<this->L; i++){
		
		//std::cout << "Hash Table : " << i << "\n";
    
        int b = hashTables[i]->bucket(img);

        for(auto it = hashTables[i]->begin(b); it != hashTables[i]->end(b); ++it){
        	if(!(img == *it))
        		if(dist(img,*it) < R)
                    ret.push_back(*it);
		}
	}

    return ret;
}



std::vector<Pair> LSH::knnSearch(Image query, int k){

    std::vector<Pair> ret;



  
    for(int i=0 ; i<L ; i++){
	
    	int b = hashTables[i]->bucket(query);
    	
    	
    	for(auto it = hashTables[i]->begin(b); it != hashTables[i]->end(b); ++it){
    		if(!(query==*it)){
			
    			Pair temp(*it,dist(query,*it));
    		//temp.img->print();
    		
    			ret.push_back(temp);
    		}
		}
	}
    std::sort(ret.begin(),ret.end(),compareDist);
    
	

    return ret;
}


std::vector<Pair> LSH::bfknnSearch(Image query, int k){
	std::vector<Pair> ret;
	
	
	for(auto it=hashTables[0]->begin(); it!=hashTables[0]->end(); ++it){
		if (query == *it){
		
            continue;
    	}else{
    		Pair temp(*it,dist(query,*it));
    		
    		ret.push_back(temp);
		}
		
        
        	
	}
	
	std::sort(ret.begin(),ret.end(),compareDist);
	
	return ret;
	
}
