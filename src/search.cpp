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

    /*
    cout << input_original << endl;
    cout << dupto << endl;
    cout << input_new << endl;
    cout << query_original << endl;
    cout << qupto << endl;
    cout << query_new << endl;
    cout << k << endl;
    cout << L << endl;
    cout << w << endl;
    cout << output_file << endl;
    */



    //  Abbreviations:
    //  io -> input original (space)
    //  qo -> query original (space)
    //  in -> input new (space)
    //  qn -> query new (space)



    // Open original space input file
    int fd_io;
    if ((fd_io = open(input_original.c_str(), O_RDONLY)) == -1)
        errExit("search.open(input_original)");

    // Read IDX headers
    int io_magicNumber;
    int io_numImages;
    int io_numRows;
    int io_numColumns;
    getIdxHeaders(fd_io, io_magicNumber, io_numImages, io_numRows, io_numColumns);

    Image img_o(io_numRows, io_numColumns, sizeof(char));
    
    // io_images translates to: input original images
    vector<Image> io_images;

    cout << "Scanning train images from original space...\n" << endl;
    for (int i=0; i<dupto; i++) {
        img_o.scan(fd_io, i+1);
        io_images.push_back(img_o);
    }
    cout << dupto << " images scanned" << endl;

    for (auto it=io_images.begin(); it!=io_images.end(); ++it)
        it->print();



    // Open original space query file
    int fd_qo;
    if ((fd_qo = open(query_original.c_str(), O_RDONLY)) == -1)
        errExit("search.open(query_original)");

    // Read IDX headers
    int qo_magicNumber;
    int qo_numImages;
    int qo_numRows;
    int qo_numColumns;
    getIdxHeaders(fd_qo, qo_magicNumber, qo_numImages, qo_numRows, qo_numColumns);
    
    // qo_images translates to: query original images
    vector<Image> qo_images;

    cout << "Scanning queriy images from original space...\n" << endl;
    for (int i=0; i<qupto; i++) {
        img_o.scan(fd_qo, i+1);
        qo_images.push_back(img_o);
    }
    cout << qupto << " images scanned" << endl;

    for (auto it=qo_images.begin(); it!=qo_images.end(); ++it)
        it->print();





    // Open new space input file
    int fd_in;
    if ((fd_in = open(input_new.c_str(), O_RDONLY)) == -1)
        errExit("search.open(input_new)");

    // Read IDX headers
    int in_magicNumber;
    int in_numImages;
    int in_numRows;
    int in_numColumns;
    getIdxHeaders(fd_in, in_magicNumber, in_numImages, in_numRows, in_numColumns);

    Image img_n(in_numRows, in_numColumns, sizeof(short));
    vector<Image> in_images;

    cout << "Scanning images...\n" << endl;
    for (int i=0; i<dupto; i++) {
        img_n.scan(fd_in, i+1);
        in_images.push_back(img_n);
    }
    cout << dupto << " images scanned" << endl;

    for (auto it=in_images.begin(); it!=in_images.end(); ++it)
        it->print();



    // Open new space query file
    int fd_qn;
    if ((fd_qn = open(query_new.c_str(), O_RDONLY)) == -1)
        errExit("search.open(query_new)");

    // Read IDX headers
    int qn_magicNumber;
    int qn_numImages;
    int qn_numRows;
    int qn_numColumns;
    getIdxHeaders(fd_qn, qn_magicNumber, qn_numImages, qn_numRows, qn_numColumns);

    vector<Image> qn_images;

    cout << "Scanning images...\n" << endl;
    for (int i=0; i<qupto; i++) {
        img_n.scan(fd_qn, i+1);
        qn_images.push_back(img_n);
    }
    cout << qupto << " images scanned" << endl;

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