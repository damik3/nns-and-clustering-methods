#include <fcntl.h>
#include <string.h>

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ErrExit.h"
#include "IDX.hpp"
#include "Image.hpp"
#include "Cluster.hpp"

using namespace std;




void get_args(int argc, char* argv[],
    string& input_original,
    int& dupto,
    string& input_new,
    string& nn_cluster,
    string& conf_file,
    string& output_file);




int main(int argc, char* argv[]) {

    string input_original;
    int dupto;
    string input_new;
    string nn_cluster;
    string conf_file;
    string output_file;

    get_args(argc, argv,
    input_original,
    dupto,
    input_new,
    nn_cluster,
    conf_file,
    output_file);

    cout << input_original << endl;
    cout << dupto << endl;
    cout << input_new << endl;
    cout << nn_cluster << endl;
    cout << conf_file << endl;
    cout << output_file << endl;



    //
    // Read configuration file
    //

    ifstream confFileStream(conf_file.c_str());

    if (!confFileStream.is_open()) 
        errExit("confFileStream"); 

    string s;
    int number_of_clusters;
    int number_of_vector_hash_tables;
    int number_of_vector_hash_functions;
    int max_number_M_hypercube;
    int number_of_hypercube_dimensions;
    int number_of_probes;

    confFileStream >> s >> number_of_clusters;
    confFileStream >> s >> number_of_vector_hash_tables;
    confFileStream >> s >> number_of_vector_hash_functions;
    confFileStream >> s >> max_number_M_hypercube;
    confFileStream >> s >> number_of_hypercube_dimensions;
    confFileStream >> s >> number_of_probes;



    // ******************
    // * Abbreviations: *
    // ******************
    //  o -> original (space)
    //  n -> new (space)
    //  io -> input original (space)
    //  in -> input new (space)

    int o_img_size;
    int n_img_size;

    // Read train images from original space and insert into LSH tables
    vector<Image> io_images = getIdxData(input_original.c_str(), sizeof(char), dupto, &o_img_size);

    // Read train images from new space
    vector<Image> in_images = getIdxData(input_new.c_str(), sizeof(short), dupto, &n_img_size);
    


    // Create clusters for original space images and time
    auto t1 = std::chrono::high_resolution_clock::now();

    Cluster cluster1(number_of_clusters, 
        METHOD_CLASSIC, 
        io_images,
        number_of_vector_hash_tables,
        number_of_vector_hash_functions,
        max_number_M_hypercube,
        number_of_hypercube_dimensions,
        number_of_probes);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cluster_time_1 = t2 - t1;

    // Compute silhouette
    std::vector<double> silhouette1 = cluster1.silhouette();
    for (int i=0; i<(int)silhouette1.size(); i++) 
        cout << "silhouette1[" << i << "] = " << silhouette1[i] << endl;



    // Create clusters for new space images and time
    t1 = std::chrono::high_resolution_clock::now();

    Cluster cluster2(number_of_clusters, 
        METHOD_CLASSIC, 
        in_images,
        number_of_vector_hash_tables,
        number_of_vector_hash_functions,
        max_number_M_hypercube,
        number_of_hypercube_dimensions,
        number_of_probes);

    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cluster_time_2 = t2 - t1;

    // Compute silhouette
    std::vector<double> silhouette2 = cluster2.silhouette();
    for (int i=0; i<(int)silhouette2.size(); i++) 
        cout << "silhouette2[" << i << "] = " << silhouette2[i] << endl;




    //
    // Print to ouput file
    //

    // Open output file
    ofstream outputFileStream(output_file.c_str());
    if (!outputFileStream.is_open()) 
        errExit("outputFileStream:");

    // For new space
    outputFileStream << "NEW SPACE" << endl;

    for (int i=0; i<number_of_clusters; i++) {

        outputFileStream << "CLUSTER-" << i+1 << ": {" << cluster2.getClusterSize(i) << ", [";

        Image centroid = cluster2.getCentroid(i);

        for (int p=0; p<centroid.size; p++) {

            outputFileStream << (int)centroid.pixels[p];
            
            if (p < centroid.size - 1)
                outputFileStream << ", ";
        }

        outputFileStream << "]}" << endl;
    }

    outputFileStream << "clustering_time: " << cluster_time_2.count() << " ms" << endl;

    outputFileStream << "Silhouette: [";

    for (int i=0; i<number_of_clusters; i++) 
        outputFileStream << silhouette2[i] << ", ";

    outputFileStream << silhouette2[number_of_clusters] << "]" << endl;

    


    // For original space
    outputFileStream << "\nORIGINAL SPACE" << endl;

    for (int i=0; i<number_of_clusters; i++) {

        outputFileStream << "CLUSTER-" << i+1 << ": {" << cluster1.getClusterSize(i) << ", [";

        Image centroid = cluster1.getCentroid(i);

        for (int p=0; p<centroid.size; p++) {

            outputFileStream << (int)centroid.pixels[p];
            
            if (p < centroid.size - 1)
                outputFileStream << ", ";
        }

        outputFileStream << "]}" << endl;
    }

    outputFileStream << "clustering_time: " << cluster_time_1.count() << " ms" << endl;

    outputFileStream << "Silhouette: [";

    for (int i=0; i<number_of_clusters; i++) 
        outputFileStream << silhouette1[i] << ", ";

    outputFileStream << silhouette1[number_of_clusters] << "]" << endl;

    // Close files
    outputFileStream.close();
    confFileStream.close();
} 




void get_args(int argc, char* argv[],
    string& input_original,
    int& dupto,
    string& input_new,
    string& nn_cluster,
    string& conf_file,
    string& output_file) {
    
    string usage("cluster –d <input file original space> -dupto <optional int> -i <input file new space>  -n <classes from NN as clusters file> –c <configuration file> -o <output file>");

    if ((argc != 11) && (argc != 13)) {
        cerr << "Invalid number of parameters!" << endl;
        cerr << usage << endl;
        exit(EXIT_FAILURE);
    }

    char d_opt[] = "-d";
    char dupto_opt[] = "-dupto";
    char i_opt[] = "-i";
    char n_opt[] = "-n";
    char c_opt[] = "-c";
    char o_opt[] = "-o";

    for (int i=1; i<argc; i++) {
        if ( (strcmp(d_opt, argv[i]) == 0) ) {
            i++;
            input_original = argv[i];
        }
        else if ( (strcmp(dupto_opt, argv[i]) == 0) ) {
            i++;
            dupto = atoi(argv[i]);
        }
        else if ( (strcmp(i_opt, argv[i]) == 0) ) {
            i++;
            input_new = argv[i];
        }
        else if ( (strcmp(n_opt, argv[i]) == 0) ) {
            i++;
            nn_cluster = argv[i];
        }
        else if ( (strcmp(c_opt, argv[i]) == 0) ) {
            i++;
            conf_file = argv[i];
        }
        else if ( (strcmp(o_opt, argv[i]) == 0) ) {
            i++;
            output_file = argv[i];
        }
        else
        {
            cerr << "Invalid parameter " << argv[i] << "!" << endl;
            cerr << usage << endl;
            exit(EXIT_FAILURE);
        }
        
    }


}