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