#include "Hypercube.hpp"



int hammingDistance(int n1, int n2) 
{ 
    int x = n1 ^ n2; 
    int setBits = 0; 
  
    while (x > 0) { 
        setBits += x & 1; 
        x >>= 1; 
    } 
  
    return setBits; 
} 



queue<int> hammingNeighbors(int probes, int number, int d1) {
	set<int> neighbors, prev, tempSet;
    queue<int> ret;

    if (probes > pow(2, d1) - 1) {
        probes = pow(2, d1) - 1;
    }

    int i=probes;
	
	prev.insert(number);
	while(i>0){
		for( auto it=prev.begin() ; it!=prev.end(); ) {
			for(int j=0; j<d1 ; j++){
				int temp= (*it) ^ (1u << j); 
                if (temp == number)
                    continue;

				auto r=neighbors.insert(temp);

				if (r.second){
					tempSet.insert(temp);
                    ret.push(temp);
					i--;
                    if (i == 0) 
                        break;
				}
                else{
					continue;
				}
			}
            if (i == 0) 
                break;

			prev.erase(it++);
		}
        prev = tempSet;
        tempSet = set<int>();
	}

	return ret;
}



int Hypercube::hash(Image img) {
    int ret = 0;

    for (unsigned long i=0; i<f.size(); i++) {
        // ret = (ret << 1) | f[i](img); 
        ret = ret << 1;
        ret = ret | f[i](img);
    }

    return ret;
}



Hypercube::Hypercube(int d1, int w, int img_size) {
    this->d1 = d1;

    FFunc ffunc;
    int m = DEFAULT_m;
    int M = DEFAULT_M;
    // default w declared in header file, if not specified
    int seed = time(NULL);

    for (int i=0; i<d1; i++) {
        ffunc.init(m, M, w, seed + i, img_size);
        this->f.push_back(ffunc);
    }

}



void Hypercube::insert(Image img) {
    cube.emplace(hash(img), img);
}



Image Hypercube::bfSearch(Image query) {
    // Nearest neighbor
    Image nn(query.num_rows, query.num_cols, query.size);

    // Distance of nn
    unsigned int d = pow(2, 32) - 1;

    for (auto it = cube.begin(); it != cube.end(); ++it) {

        // Ignore the same image
        if (query == it->second)
            continue;

        if (dist(query, it->second) < d) {
            nn = it->second;
            d = dist(query, it->second);    
            // std::cout << "\nnew d = " << d << std::endl;
        }
    }


    return nn;
}



std::vector<Pair> Hypercube::knnSearch(int M, int k,int probes, Image query){
	
	std::vector<Pair> ret;
	
	if(k>M)
		errExit("\nK-neighbors are more than M points allowed \n");
	
	
	
	int h=hash(query);
	auto range = cube.equal_range(h);
	
	for(auto it=range.first; it!=range.second;++it){
		
		if(!(query==it->second) && M>0){
			Pair temp(it->second,dist(query,it->second));
			
			ret.push_back(temp);
			
			M--;
		}
		
		
	}
	
	std::queue<int> n= hammingNeighbors(probes,h,this->d1);
	
	if(M>0){
		
		while(n.size()>0){
			auto range= cube.equal_range(n.front());
			
			for(auto it=range.first; it!=range.second; ++it){
				
				if(M>0){
					Pair temp(it->second,dist(query,it->second));
					
					ret.push_back(temp);
					M--;
				}
				
			}
			
			n.pop();
			if(M==0){
				break;
			}
		}
		
		
	}
	
	std::sort(ret.begin(),ret.end(),compareDist);
	
	return ret;
	
}


std::vector<Pair> Hypercube::bfknnSearch(Image query){
	
	std::vector<Pair> ret;
	
	for(auto it=cube.begin(); it!=cube.end(); ++it){
		
		if(query==it->second){
			continue;
		}else{
			Pair temp(it->second,dist(query,it->second));
			
			ret.push_back(temp);
		}
		
	}
	
	std::sort(ret.begin(),ret.end(),compareDist);
	
	return ret;
	
}


Image Hypercube::nnSearch(Image query, int probes, int M) {
    // Nearest neighbor
    Image nn(query.num_rows, query.num_cols, query.size);

    // Distance of nn
    unsigned int d = pow(2, 32) - 1;

    // Hash
    int h = hash(query);

    // Neighbors of distance 'probes'
    std::queue<int> neighbors = hammingNeighbors(probes, h, d1);

    // Get images of apppropriate vertice
    auto range = cube.equal_range(h);

    // Start from the first
    auto it = range.first;

    // Number of images checked
    int checked = 0;

    // Number of probes checked
    int probesChecked = 0;

    while (probes && M) {

        // If current bucket is empty, go to the next one
        if (it == range.second) {
            range = cube.equal_range(neighbors.front());
            neighbors.pop();
            it = range.first;
            probes--;

            cout << "Finished checking " << ++probesChecked << " probes" << endl;

        }

        // If current bucket is not empty, check next image
        else {

            cout << "Checked " << ++checked << " images" << endl;

            // If closest neighbor found, keep it
            if (dist(query, it->second) < d) {
                nn = it->second;
                d = dist(query, it->second);    
                std::cout << "\nnew d = " << d << std::endl;
            }

            M--;
            ++it;
        }
        
    }

    return nn;
}



std::vector<Image> Hypercube::rangeSearch(Image query, double R, int probes, int M){
	
    // Nearest neighbors
    std::vector<Image> nn;

    // Hash
    int h = hash(query);

    // Neighbors of distance 'probes'
    std::queue<int> neighbors = hammingNeighbors(probes, h, d1);

    // Get images of apppropriate vertice
    auto range = cube.equal_range(h);

    // Start from the first
    auto it = range.first;

    while (probes && M) {

        // If current bucket is empty, go to the next one
        if (it == range.second) {
            range = cube.equal_range(neighbors.front());
            neighbors.pop();
            it = range.first;
            probes--;
        }

        // If current bucket is not empty, check next image
        else {
            // If closest neighbor found, keep it
            if (dist(query, it->second) < R) {
                nn.push_back(it->second);
            }

            M--;
            ++it;
        }
        
    }

    return nn;

}
