#include <fcntl.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include "ErrExit.h"
#include "IDX.hpp"
#include "Image.hpp"

using namespace std;

void getCommandLineArgs(int argc, char *argv[],
    string& input_original,
    int& dupto,
    string& input_new,
    string& query_original,
    int& qupto,
    string& query_new,
    int& k,
    int& L, 
    int& w,
    string& output_file
    );






int main(int argc, char* argv[]) {
    cout << "Hello search!" << endl;

    string input_original;
    int dupto = 1000;
    string input_new;
    string query_original;
    int qupto = 1000;
    string query_new;
    int k;
    int L; 
    int w = 40;
    string output_file;

    getCommandLineArgs(argc, argv,
        input_original,
        dupto,
        input_new,
        query_original,
        qupto,
        query_new,
        k,
        L, 
        w,
        output_file);



    //  Abbreviations:
    //  io -> input original (space)
    //  qo -> query original (space)
    //  in -> input new (space)
    //  qn -> query new (space)

    // Read train images from original space
    vector<Image> io_images = getIdxData(input_original.c_str(), sizeof(char), dupto);
    for (auto it=io_images.begin(); it!=io_images.end(); ++it)
        it->print();

    // Read query images from original space
    vector<Image> qo_images = getIdxData(query_original.c_str(), sizeof(char), qupto);
    for (auto it=qo_images.begin(); it!=qo_images.end(); ++it)
        it->print();
    
    // Read train images from new space
    vector<Image> in_images = getIdxData(input_new.c_str(), sizeof(short), dupto);
    for (auto it=in_images.begin(); it!=in_images.end(); ++it)
        it->print();
    
    // Read train images from original space
    vector<Image> qn_images = getIdxData(query_new.c_str(), sizeof(short), qupto);
    for (auto it=qn_images.begin(); it!=qn_images.end(); ++it)
        it->print();

    
    
}




void getCommandLineArgs(int argc, char *argv[],
    string& input_original,
    int& dupto,
    string& input_new,
    string& query_original,
    int& qupto,
    string& query_new,
    int& k,
    int& L, 
    int& w,
    string& output_file) {

    string usage("Usage: ./search –d <input file original space> -dupto <int> -i <input file new space> –q <query file original space> -qupto <int> -s <query file new space> –k <int> -L <int> -w <int> -ο <output file>");

    if ((argc != 15) && (argc != 17) && (argc != 19) && (argc != 21)) {
        cerr << "Invalid number of parameters!" << endl;
        cerr << usage << endl;
        exit(EXIT_FAILURE);
    }



    char d_opt[] = "-d";
    char dupto_opt[] = "-dupto";
    char i_opt[] = "-i";
    char q_opt[] = "-q";
    char qupto_opt[] = "-qupto";
    char s_opt[] = "-s";
    char k_opt[] = "-k";
    char L_opt[] = "-L";
    char w_opt[] = "-w";
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
        else if ( (strcmp(q_opt, argv[i]) == 0) ) {
            i++;
            query_original = argv[i];
        }
        else if ( (strcmp(qupto_opt, argv[i]) == 0) ) {
            i++;
            qupto = atoi(argv[i]);
        }
        else if ( (strcmp(s_opt, argv[i]) == 0) ) {
            i++;
            query_new = argv[i];
        }
        else if ( (strcmp(k_opt, argv[i]) == 0) ) {
            i++;
            k = atoi(argv[i]);
        }
        else if ( (strcmp(L_opt, argv[i]) == 0) ) {
            i++;
            L = atoi(argv[i]);
        }
        else if ( (strcmp(w_opt, argv[i]) == 0) ) {
            i++;
            w = atoi(argv[i]);
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