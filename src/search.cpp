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
#include "LSH.hpp"

using namespace std;

void getCommandLineArgs1(int argc, char *argv[],
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

void getCommandLineArgs2(int argc, char *argv[],
    string& input,
    int& dupto,
    string& queries,
    int& qupto,
    string& input_labels,
    string& queries_labels,
    string& output_file
    );

Image bf_search(vector<Image> images, Image query);

int main1(int argc, char* argv[]);
int main2(int argc, char* argv[]);



int main(int argc, char* argv[]) {
    
    // Check for EMD flag
    for (int i=1; i<argc; i++)
        if (strcmp(argv[i], "-EMD") == 0)
            return main2(argc, argv);
            
    // If no EMD flag, call main1
    return main1(argc, argv);
}




int main1(int argc, char* argv[]) {

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

    getCommandLineArgs1(argc, argv,
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



    // ******************
    // * Abbreviations: *
    // ******************
    //  o -> original (space)
    //  n -> new (space)
    //  io -> input original (space)
    //  qo -> query original (space)
    //  in -> input new (space)
    //  qn -> query new (space)

    int o_img_size;
    int n_img_size;

    // Read train images from original space and insert into LSH tables
    vector<Image> io_images = getIdxData(input_original.c_str(), sizeof(char), dupto, &o_img_size);
    LSH lsh(w, k, L, dupto, o_img_size);
    for (auto it=io_images.begin(); it!=io_images.end(); ++it) {
        lsh.insert(*it);
    }

    // Read query images from original space
    vector<Image> qo_images = getIdxData(query_original.c_str(), sizeof(char), qupto, &o_img_size);
    
    // Read train images from new space
    vector<Image> in_images = getIdxData(input_new.c_str(), sizeof(short), dupto, &n_img_size);
    
    // Read train images from original space
    vector<Image> qn_images = getIdxData(query_new.c_str(), sizeof(short), qupto, &n_img_size);
    
    

    // Open output file
    ofstream ofs(output_file.c_str());
    if (!ofs.is_open()) 
        errExit("outputFileStream:");



    //
    //  Process queries and write to output file
    //

    std::chrono::duration<double, std::milli> nn_time(0);
    std::chrono::duration<double, std::milli> obf_time(0);
    std::chrono::duration<double, std::milli> nbf_time(0);
    double sum_lsh = 0;
    double sum_reduced = 0;

    for (int i=0; i<qupto; i++) {

        // Query and time

        auto t1 = std::chrono::high_resolution_clock::now();
        Image nn = lsh.nnSearch(qo_images[i]);
        auto t2 = std::chrono::high_resolution_clock::now();
        nn_time += t2 - t1;
        unsigned long nn_dist = dist(nn, qo_images[i]);

        t1 = std::chrono::high_resolution_clock::now();
        Image obf = lsh.bfSearch(qo_images[i]);
        t2 = std::chrono::high_resolution_clock::now();
        obf_time += t2 - t1;
        unsigned long obf_dist = dist(obf, qo_images[i]);

        t1 = std::chrono::high_resolution_clock::now();
        Image nbf = bf_search(in_images, qn_images[i]);
        t2 = std::chrono::high_resolution_clock::now();
        nbf_time += t2 - t1;
        unsigned long nbf_dist = dist(io_images[nbf.id-1], qo_images[i]);

        // Update approximation factors
        sum_lsh += ((double) nn_dist / obf_dist);
        sum_reduced += ((double) nbf_dist / obf_dist);

        // Print to output file
        ofs << "Query: " << i+1 << endl;
        ofs << "Nearest neighbor Reduced: " << nbf.id << endl;
        ofs << "Nearest neighbor LSH: " << nn.id << endl;
        ofs << "Nearest neighbor True: " << obf.id << endl;
        ofs << "distanceReduced: " << nbf_dist << endl;
        ofs << "distanceLSH: " << nn_dist << endl;
        ofs << "distanceTrue: " << obf_dist << endl;
        ofs << endl;

        // Print to cout
        cout << "\n\n************************************************" << endl;
        cout << "Query original" << endl;
        qo_images[i].print();
        cout << "\nBF Original" << endl;
        obf.print();
        cout << "\nLSH Original" << endl;
        nn.print();
        cout << "\nNeuralNet Original" << endl;
        io_images[nbf.id-1].print();


    }

    ofs << "tReduced: " << nbf_time.count() << endl;
    ofs << "tLSH: " << nn_time.count() << endl;
    ofs << "tTrue: " << obf_time.count() << endl;
    ofs << "Approximation Factor LSH: " << sum_lsh / qupto << endl;
    ofs << "Approximation Factor Reduced: " << sum_reduced / qupto << endl;


    ofs.close();
    return 0;
}




int main2(int argc, char* argv[]) {
    
    string input_fn;
    int dupto;
    string queries_fn;
    int qupto;
    string input_labels_fn;
    string queries_labels_fn;
    string output_fn;

    getCommandLineArgs2(argc, argv,
        input_fn,
        dupto,
        queries_fn,
        qupto,
        input_labels_fn,
        queries_labels_fn,
        output_fn);

    cout << input_fn << endl;
    cout << dupto << endl;
    cout << queries_fn << endl;
    cout << qupto << endl;
    cout << input_labels_fn << endl;
    cout << queries_labels_fn << endl;
    cout << output_fn << endl;



    int img_size1;
    int img_size2;

    // Read data from files
    vector<Image> input_images = getIdxData(input_fn.c_str(), sizeof(char), dupto, &img_size1);
    vector<Image> query_images = getIdxData(input_fn.c_str(), sizeof(char), qupto, &img_size2);

    if (img_size1 != img_size2) {
        cerr << "Unequal image sizes for input and query sets!" << endl;
        exit(EXIT_FAILURE);
    }

    // Read labels from files
    vector<int> input_labels = getIdxLabelData(input_labels_fn.c_str(), dupto);
    vector<int> query_labels = getIdxLabelData(input_labels_fn.c_str(), qupto);



    return 0;
}




Image bf_search(vector<Image> images, Image query) {
    
    // Nearest neighbor
    Image nn(query.num_rows, query.num_cols, query.pixel_size);

    // Distance of nn
    unsigned int d = pow(2, 32) - 1;

    for (auto it = images.begin(); it != images.end(); ++it) {

        // Ignore the same image
        if (query == *it)
            continue;

        if (dist(query, *it) < d) {
            nn = *it;
            d = dist(query, *it);    
        }
    }

    return nn;
}




void getCommandLineArgs1(int argc, char *argv[],
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




void getCommandLineArgs2(int argc, char *argv[],
    string& input,
    int& dupto,
    string& queries,
    int& qupto,
    string& input_labels,
    string& queries_labels,
    string& output_file
    ) {

    string usage("./search -EMD –d  <input  file  original  space>  -dupto <optional int>  –q  <query  file  original  space>  -qupto <optional int>  -l1  <labels of input dataset> -l2 <labels of query dataset> -ο <output file>");

    if ((argc != 12) && (argc != 14) && (argc != 16)) {
        cerr << "Invalid number of parameters!" << endl;
        cerr << usage << endl;
        exit(EXIT_FAILURE);
    }

    char d_opt[] = "-d";
    char dupto_opt[] = "-dupto";
    char q_opt[] = "-q";
    char qupto_opt[] = "-qupto";
    char l1_opt[] = "-l1";
    char l2_opt[] = "-l2";
    char o_opt[] = "-o";

    for (int i=1; i<argc; i++) {
        if ( (strcmp(d_opt, argv[i]) == 0) ) {
            i++;
            input = argv[i];
        }
        else if ( (strcmp(dupto_opt, argv[i]) == 0) ) {
            i++;
            dupto = atoi(argv[i]);
        }
        else if ( (strcmp(q_opt, argv[i]) == 0) ) {
            i++;
            queries = argv[i];
        }
        else if ( (strcmp(qupto_opt, argv[i]) == 0) ) {
            i++;
            qupto = atoi(argv[i]);
        }
        else if ( (strcmp(l1_opt, argv[i]) == 0) ) {
            i++;
            input_labels = argv[i];
        }
        else if ( (strcmp(l2_opt, argv[i]) == 0) ) {
            i++;
            queries_labels = argv[i];
        }
        else if ( (strcmp(o_opt, argv[i]) == 0) ) {
            i++;
            output_file = argv[i];
        }
        else if ( (strcmp("-EMD", argv[i]) == 0) ) {
            continue;
        }
        else
        {
            cerr << "Invalid parameter " << argv[i] << "!" << endl;
            cerr << usage << endl;
            exit(EXIT_FAILURE);
        }
    }
}