#include "Cluster.hpp"

using namespace std;




int binSea(std::vector<unsigned int> A, int x, unsigned int pos1, unsigned int pos2) {

    if (x < (int)A[pos1])
        return pos1;

    if (x > A[pos2])
        return -1;

    if ((pos1 < 0) || (pos2 < 0) || (pos1 >= A.size()) || (pos2 >= A.size()) || (pos2 < pos1))
        return -1;

    if (pos2 == pos1 + 1) {
     
        if (A[pos1] == x)
            return pos1;
        else
            return pos2;
    }

    // std::cout << "binSea(A, " << x << ", " << pos1 << ", " << pos2 << ")" << std::endl;

    unsigned int newp = (pos1 + pos2) / 2;

    return (x < A[newp]) ? binSea(A, x, pos1, newp) : binSea(A, x, newp, pos2);
}




int newCentroidPos(std::vector<Image> input, std::vector<Image> centroid, int t) {

    //
    // For i-th image, compute its minimum distance to a centroid
    // and store it in P[i]
    //

    std::vector<unsigned int> P;

    P.push_back(0);

    unsigned int minDist = pow(2, 31) - 1;

    for (unsigned int i=0; i<input.size(); i++) {

        for (int j=0; j<t; j++) {

            if (dist(input[i], centroid[j]) < minDist)
                minDist = dist(input[i], centroid[j]);
        }

        P.push_back((minDist >> 5) + P[i]); // Normalize dividing by 32
        minDist = pow(2, 31) - 1;
    }
/*
    std::cout << "\n\nPrinting P[i]" << std::endl;
    for (auto it = P.begin(); it != P.end(); ++it)
        std::cout << *it << std::endl;
    std::cout << "\n\n";
*/
    //
    // Pick new centroid based on probabilities P[i]
    //

    srand(time(NULL));

    int x = rand() % P[P.size() - 1];

    return binSea(P, x, 0, P.size() - 1) - 1; // ??? -1 ??? 

}




Image median(std::vector<Image> images) {

    if (images.empty())
        return Image(0, 0, 0);

    Image ret(images[0].num_rows, images[0].num_cols, images[0].pixel_size);

    int img_size = images[0].size;

    for (int j=0; j<img_size; j++) {

        std::vector<PIXEL_T> temp;

        for (unsigned int i=0; i<images.size(); i++) {

            temp.push_back(images[i].pixels[j]);

        }

        sort(temp.begin(), temp.end());

        ret.pixels[j] = temp[temp.size() / 2];

    }

    return ret;
}




int limitUnassigned(int inputSize) {
    return inputSize / 2;
}




Cluster::Cluster() : k(-1) 
    {}




Cluster::Cluster(int k1, std::string m1, 
        std::vector<Image> input,
        int number_of_vector_hash_tables,
        int number_of_vector_hash_functions,
        int max_number_M_hypercube,
        int number_of_hypercube_dimensions,
        int number_of_probes) 
    : k(k1), m(m1), centroid(k, Image(input[0].num_rows, input[0].num_cols, input[0].size)), cluster(k) {

    srand(time(NULL));
    int img_size = input[0].size;


    //
    // k means ++ initialization
    //

    int t = 0;

    // Chose first centroid at random
    int pos = rand() % input.size();

    // Assign new centroid
    centroid[t] = input[pos];
    centroid[t].id = 0;

    // Erase centroid from input ??
    // input.erase(input.begin() + pos);

    // Substitute with random image
    input[pos] = input[rand() % input.size()];
    input[pos].id = pos + 1;

    // Change duplicate image at one bit at random
    input[pos].pixels[rand() % img_size] = rand() % (int)pow(2, 8*sizeof(PIXEL_T));

    t++;



    while (t < k) {

        // Pick new centroid
        pos = newCentroidPos(input, centroid, t);

        // Assign new centroid
        centroid[t] = input[pos];
        centroid[t].id = 0;

        // Erase centroid from input ??
        // input.erase(input.begin() + pos);

        // Substitute with random image
        input[pos] = input[rand() % input.size()];
        input[pos].id = pos + 1;

        // Change duplicate image at one bit at random
        input[pos].pixels[rand() % img_size] = rand() % (int)pow(2, 8*sizeof(PIXEL_T));

        t++;
    }


/*
    // Print all centroids
    std::cout << "\n\n\t\t\tPrinting all centroids after initialization++!" << std::endl;
    for (auto it = centroid.begin(); it != centroid.end(); ++it)
        it->print();
*/


    // Find minimum distance between two centroids, gonna use it for LSH & Hypercube
    unsigned int initRange = pow(2, 32) - 1;

    for (int i=0; i<k; i++) 
        for (int j=i+1; j<k; j++) 
            if (dist(centroid[i], centroid[j]) < initRange)
                initRange = dist(centroid[i], centroid[j]);

    // std::cout << "initRange = " << initRange << std::endl;



    //
    // Assignment of images to centroids
    //

    int n = 0;
    bool stop = false;  


    // Insert all images into LSH hash tables
    LSH lsh(DEFAULT_w, number_of_vector_hash_functions, number_of_vector_hash_tables, input.size(), img_size);

    if (m == METHOD_LSH)
        for (auto it = input.begin(); it != input.end(); ++it)
            lsh.insert(*it);

    // Same for hypercube
    Hypercube cube(number_of_hypercube_dimensions, HYPERCUBE_w, img_size);

    if (m == METHOD_HYPERCUBE)
        for (auto it = input.begin(); it != input.end(); ++it)
            cube.insert(*it);

    // i-th position of imgAssignedToCluster contains a list of centroids image i has been assigned
    // i is derived from Image.id
    std::vector<std::vector<int> > imgAssignedToCluster(input.size());



    do {

        n++;

        

        //
        // Classic Lloyd's
        //

        if (m == METHOD_CLASSIC) {

            // If little or no change in centroids, stop
            // But only for Lloyd's
            if (stop)
                break;

            for (int j=0; j<k; j++)
                cluster[j].clear();


            unsigned int minDist;
            int pos;

            for (unsigned int i=0; i<input.size(); i++) {

                minDist = pow(2, 31) - 1;

                for (int j=0; j<k; j++) {

                    if (dist(input[i], centroid[j]) < minDist) {
                        minDist = dist(input[i], centroid[j]);
                        pos = j;
                    }

                }

                cluster[pos].push_back(input[i]);

            }

        }



        //
        // LSH or Hypercube
        //

        else if ( (m == METHOD_LSH) or (m == METHOD_HYPERCUBE) ) {

            for (int j=0; j<k; j++)
                cluster[j].clear();

            // Clear set
            for (int i=0; i< (int) input.size(); i++)
                imgAssignedToCluster[i].clear();



            //
            // For each centroid, do range query and assign to it images returned by query
            //

            for (int i=0; i<k; i++) {

                std::vector<Image> ret;

                if (m == METHOD_LSH)
                    ret = lsh.rangeSearch(centroid[i], (double) initRange);
                else if (m == METHOD_HYPERCUBE)
                    ret = cube.rangeSearch(centroid[i], (double) initRange, number_of_probes, max_number_M_hypercube);


                for (auto it = ret.begin(); it != ret.end(); ++it) {

                    cluster[i].push_back(*it);

                    // std::cout << "Gonna push image " << it->id - 1 << " into cluster " << i << std::endl;

                    imgAssignedToCluster[it->id - 1].push_back(i);

                }

                // Remove duplicates from cluster[i]

                std::unordered_set<Image, HFunc> tempSet(1, HFunc(DEFAULT_m, DEFAULT_M, DEFAULT_w, time(NULL), img_size));

                for (auto it = cluster[i].begin(); it != cluster[i].end(); ++it)
                    tempSet.insert(*it);

                cluster[i].clear();

                for (auto it = tempSet.begin(); it != tempSet.end(); ++it)
                    cluster[i].push_back(*it);

            }



            //
            // Check if some images are assigned to 2 or more centroids and tidy up
            //
            
            for (int i=0; i < (int) input.size(); i++) {

                if (imgAssignedToCluster[i].size() > 1) {

                    unsigned nearestCentroidDist = pow(2, 32) - 1;
                    int nearestCentroid = -1;

                    for (int j=0; j<(int) imgAssignedToCluster[i].size(); j++) {

                        if (dist(input[i], centroid[imgAssignedToCluster[i][j]]) < nearestCentroidDist) {

                            nearestCentroidDist = dist(input[i], centroid[imgAssignedToCluster[i][j]]);
                            nearestCentroid = imgAssignedToCluster[i][j];

                        }

                    }

                    imgAssignedToCluster[i].clear();

                    if (nearestCentroid == -1) 
                        errExit("DEATH");

                    imgAssignedToCluster[i].push_back(nearestCentroid);
                    
                }

            }


/*
            //
            // Print imgAssignedToCluster
            //

            std::cout << "\n\nAfter tidy up\n" << std::endl;
            for (int i=0; i < (int) input.size(); i++) {
                std::cout << "imgAssignedToCluster[" << i << "] = ";
                for (int j=0; j<(int) imgAssignedToCluster[i].size(); j++) {
                    std::cout << imgAssignedToCluster[i][j] << ", ";
                }
                std::cout << std::endl;
            }
*/

            
            // Check number of unassigned images
            int unassignedImages = 0;

            for (int i=0; i<(int) input.size(); i++) 
                if (imgAssignedToCluster[i].size() == 0)
                    unassignedImages++;

            // std::cout << "unassigned images = " << unassignedImages << ", limit = " << limitUnassigned(input.size()) << std::endl;

            if (unassignedImages < limitUnassigned(input.size())) 
                break;


            initRange *= 2;
            if (initRange <= 0)
                break;
            // std::cout << "new initRange = " << initRange << std::endl;

        }   // else if (m == METHOD_LSH) or (m == METHOD_HYPERCUBE) 



        //
        // Calculate new median for each cluster
        //

        stop = true;

        for (int j=0; j<k; j++) {

            if (cluster[j].size()) {

                Image newCentroid = median(cluster[j]);

                if (dist(newCentroid, centroid[j]) < MAX_CHANGE)
                    stop = stop && true;
                else 
                    stop = stop && false;

                centroid[j] = newCentroid; 
                centroid[j].id = 0;
            
            }

            // std::cout << "\n\ncentroid[" << j << "]" << std::endl;
            // centroid[j].print();   
            
        }

        // If little or no change in centroids, stop

        //if (stop)
            //break;

    } while (1);


/*
    //
    // Print stuff
    //

    unsigned int total_images = 0;

    std::cout << "\n\n\t\t\tPrinting all centroids after assignment! n = " << n << std::endl;
    for (int i=0; i<k; i++) {

        std::cout << "\n\n\n\nCentroid with size ("<< cluster[i].size() << ")\n****************************************************************************************************************\n\n\n" << std::endl;
        centroid[i].print();
        total_images += cluster[i].size();

        int l = 20;

        for (auto it = cluster[i].begin(); it != cluster[i].end(); ++it) {
            //it->print();

            if (l == 0)
                break;

            l--;
        }

    }

    // std::cout << "\n\nTotal images = " << total_images << std::endl;
*/
} 




Image Cluster::getCentroid(int c) {
    return centroid[c];
}




size_t Cluster::getClusterSize(int c) {
    return cluster[c].size();
}




std::vector<Image> Cluster::getCluster(int c) {
    return cluster[c];    
}




//
// c        : index of a centroid in vector centroids
// returns  : index of nearest centroid
//
int nextBestNeighbor(int c, std::vector<Image> centroids) {

    int pos;
    unsigned int minDist = pow(2, 31) - 1;

    for (int i=0; i<(int)centroids.size(); i++) {

        if (i == c)
            continue;

        if (dist(centroids[i], centroids[c]) < minDist) {
            pos = i;
            minDist = dist(centroids[i], centroids[c]);
        }
    }

    return pos;
}



std::vector<double> Cluster::silhouette() {

    std::vector<double> ret;

    std::vector<std::vector<double> > s(k);

    long double sum = 0;
    long double sumAll = 0;
    long double count = 0;
    long double countAll = 0;
    double a;
    double b;

    // For each cluster
    for (int k1=0; k1<k; k1++) {

        // std::cout << "\n\nChecking cluster " << k1 << " with size " << cluster[k1].size() << std::endl;

        int k2 = nextBestNeighbor(k1, centroid);

        // For each image in cluster k1
        for (int i=0; i<(int) cluster[k1].size(); i++) {

            // Compute a_i

            sum = 0;
            count = 0;

            // Average will all images of cluster
            for (int j=0; j<(int) cluster[k1].size(); j++) {

                sum += dist(cluster[k1][i], cluster[k1][j]);
                count++;

            }

            a = (sum / (double) count);

            // Compute b_i

            sum = 0;
            count = 0;

            // Average will all images of cluster
            for (int j=0; j<(int) cluster[k2].size(); j++) {

                sum += dist(cluster[k1][i], cluster[k2][j]);
                count++;

            }

            b = (sum / (double) count);

            s[k1].push_back((b - a) / std::max(a, b));

            sumAll += s[k1][i];
            countAll++;
            
        } // End For each image in cluster k1

        sum = 0;
        count = 0;
        for (int i=0; i<(int)s[k1].size(); i++) {
            sum += s[k1][i];
            count++;
        }

        ret.push_back(sum / (double) count);

    }   // End for each cluster

    ret.push_back(sumAll/ (double) countAll);

    return ret;

}




double Cluster::objective_f() {

    double ret = 0;

    for (int k1=0; k1<k; k1++)
        for (int i=0; i<(int) cluster[k1].size(); i++)  
            ret += dist(cluster[k1][i], centroid[k1]);

    return ret;
}




void Cluster::display() {

    for (int k1=0; k1<k; k1++) {

        cout << "\n\n********************************************" << endl;
        cout << "********************************************" << endl;
        cout << "centroid[" << k1 << "]" << endl;
        centroid[k1].print();
        cout << "********************************************" << endl;
        cout << "Contains..." << endl;

        for (int i=0; i<(int) cluster[k1].size(); i++)  {
            cluster[k1][i].print();
        }
    }

}




void Cluster::replace(std::vector<Image> new_image) {

    // For each image in each cluster,
    // replace it with corresponding image from new_image.
    for (int k1=0; k1<k; k1++) 
        for (int i=0; i<(int) cluster[k1].size(); i++) 
            cluster[k1][i] = new_image[cluster[k1][i].id - 1];    
        
    // Recompute centroids
    for (int j=0; j<k; j++) 
        if (cluster[j].size()) 
            centroid[j] = median(cluster[j]);

}




std::vector<std::vector<int> > parse_clustering(string& filename) {

    std::vector<std::vector<int> > ret;

    ifstream ifs(filename);

    if (ifs.is_open() == false) 
        errExit("Cluster.parse_clustering: Can't open file!");

    string s;
    char c;
    int size = 0;
    int val;

    while (ifs >> s) {  // read "CLUSTER-k"
        std::vector<int> v;

        ifs >> s;       // read "{"
        ifs >> s;       // read "size:"
        
        ifs >> s;
        // Convert for example "3," to "3" and then to 3
        size = stoi(s.substr(0, s.size()-1));
        
        for (int i=0; i<size; i++) {
            ifs >> s;
            val = stoi(s.substr(0, s.size()-1));

            v.push_back(val);
        }
        
        ifs >> s;       // read "}"
    
        ret.push_back(v);
    }

    return ret;
}



Cluster classes_as_clusters(string& filename, vector<Image> images) {
    auto cluster_imgn = parse_clustering(filename);

    int num_clusters = cluster_imgn.size();

    vector<vector<Image> > clusters(num_clusters);

    for (int i=0; i<num_clusters; i++)
        for (int j = 0; j < cluster_imgn[i].size(); j++)
            clusters[i].push_back(images[cluster_imgn[i][j] - 1]);

    vector<Image> centroid;
    for (int k=0; k< num_clusters; k++) 
        centroid.push_back( median(clusters[k]) );

    Cluster cluster;
    cluster.k = num_clusters;
    cluster.m = METHOD_CLASSIC;
    cluster.centroid = centroid;
    cluster.cluster = clusters;

    return cluster;
}