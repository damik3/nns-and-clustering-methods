#include "acutest.h"
#include "Cluster.hpp"

#include <vector>
#include <iostream>

using namespace std;

vector<PIXEL_T> pixels1{ 1, 2 };
vector<PIXEL_T> pixels2{ 4, 0 };
vector<PIXEL_T> pixels3{ 5, 1 };
vector<PIXEL_T> pixels4{ 2, 2 };

vector<PIXEL_T> pixelsc1{ 1, 1 };
vector<PIXEL_T> pixelsc2{ 4, 4 };


void test_objective_f(void) {
    int numrows = 1;
    int numcols = 2;

    vector<PIXEL_T> pixels1{ 1, 2 };
    vector<PIXEL_T> pixels2{ 4, 0 };
    vector<PIXEL_T> pixels3{ 5, 1 };
    vector<PIXEL_T> pixels4{ 2, 2 };

    vector<PIXEL_T> pixelsc1{ 1, 1 };
    vector<PIXEL_T> pixelsc2{ 4, 4 };

    Image img1(numrows, numcols, 1);
    Image img2(numrows, numcols, 1);    
    Image img3(numrows, numcols, 1);
    Image img4(numrows, numcols, 1);

    img1.pixels = pixels1;
    img2.pixels = pixels2;
    img3.pixels = pixels3;
    img4.pixels = pixels4;

    vector<Image> cluster1;
    vector<Image> cluster2;

    cluster1.push_back(img1);
    cluster1.push_back(img4);

    cluster2.push_back(img2);
    cluster2.push_back(img3);
    
    Image centroid1(numrows, numcols, 1);    
    Image centroid2(numrows, numcols, 1);
    
    centroid2.pixels = pixelsc2;
    centroid1.pixels = pixelsc1;


    Cluster cluster;
    cluster.k = 2;
    cluster.m = METHOD_CLASSIC;
    cluster.centroid.push_back(centroid1);
    cluster.centroid.push_back(centroid2);
    cluster.cluster.push_back(cluster1);
    cluster.cluster.push_back(cluster2);

    // cluster.display();

    TEST_CHECK( cluster.objective_f() == (double)11 );

}



void test_replace(void) {
    int numrows = 1;
    int numcols = 2;

    // Original space images
    vector<PIXEL_T> pixels1{ 100, 150 };
    vector<PIXEL_T> pixels2{ 130, 130 };
    vector<PIXEL_T> pixels3{ 110, 180 };
    vector<PIXEL_T> pixels4{ 100, 130 };

    vector<PIXEL_T> pixelsc1{ 100, 150 };
    vector<PIXEL_T> pixelsc2{ 130, 130 };

    Image img1(numrows, numcols, 1);
    Image img2(numrows, numcols, 1);    
    Image img3(numrows, numcols, 1);
    Image img4(numrows, numcols, 1);

    img1.id = 1;
    img2.id = 2;
    img3.id = 3;
    img4.id = 4;

    img1.pixels = pixels1;
    img2.pixels = pixels2;
    img3.pixels = pixels3;
    img4.pixels = pixels4;

    vector<Image> cluster1;
    vector<Image> cluster2;

    cluster1.push_back(img1);
    cluster1.push_back(img4);

    cluster2.push_back(img2);
    cluster2.push_back(img3);
    
    Image centroid1(numrows, numcols, 1);    
    Image centroid2(numrows, numcols, 1);

    centroid1.id = -1;
    centroid2.id = -1;
    
    centroid2.pixels = pixelsc2;
    centroid1.pixels = pixelsc1;


    Cluster cluster;
    cluster.k = 2;
    cluster.m = METHOD_CLASSIC;
    cluster.centroid.push_back(centroid1);
    cluster.centroid.push_back(centroid2);
    cluster.cluster.push_back(cluster1);
    cluster.cluster.push_back(cluster2);

    // cluster.display();

    TEST_CHECK( cluster.objective_f() == (double)90 );


    // New space images
    vector<PIXEL_T> pixels1n{ 10, 15 };
    vector<PIXEL_T> pixels2n{ 13, 13 };
    vector<PIXEL_T> pixels3n{ 11, 18 };
    vector<PIXEL_T> pixels4n{ 10, 13 };

    Image img1n(numrows, numcols, 1);
    Image img2n(numrows, numcols, 1);    
    Image img3n(numrows, numcols, 1);
    Image img4n(numrows, numcols, 1);

    img1n.id = 1;
    img2n.id = 2;
    img3n.id = 3;
    img4n.id = 4;

    img1n.pixels = pixels1n;
    img2n.pixels = pixels2n;
    img3n.pixels = pixels3n;
    img4n.pixels = pixels4n;

    vector<Image> new_image;
    new_image.push_back(img1n);
    new_image.push_back(img2n);
    new_image.push_back(img3n);
    new_image.push_back(img4n);

    Cluster cluster_new = cluster;
    cluster_new.replace(new_image);

    // cluster_new.display();

    TEST_CHECK( cluster_new.objective_f() == (double)9 );

}



void test_parse_clustering(void) {

    string s("../src/test/input/parse_cluster_input_1.txt");
    auto ret = parse_clustering(s);

    vector<int> v1{1, 2, 3};
    vector<int> v2{4, 5};
    vector<int> v3{6};

    vector<vector<int> > correct;
    correct.push_back(v1);
    correct.push_back(v2);
    correct.push_back(v3);
/*
    // Display
    for (auto cluster = ret.begin(); cluster != ret.end(); ++cluster) {
        for (auto element = cluster->begin(); element != cluster->end(); ++element) {
            cout << *element << ", ";
        }
        cout << endl;
    }
*/
    for (int i=0; i<3; i++)
        TEST_CHECK(ret[i] == correct[i]);

}



void test_classes_as_clusters(void) {

    int numrows = 1;
    int numcols = 2;

    vector<PIXEL_T> pixels1{ 100, 150 };
    vector<PIXEL_T> pixels2{ 130, 130 };
    vector<PIXEL_T> pixels3{ 110, 180 };
    vector<PIXEL_T> pixels4{ 100, 130 };
    vector<PIXEL_T> pixels5{ 100, 140 };
    vector<PIXEL_T> pixels6{ 100, 150 };

    Image img1(numrows, numcols, 1);
    Image img2(numrows, numcols, 1);    
    Image img3(numrows, numcols, 1);
    Image img4(numrows, numcols, 1);
    Image img5(numrows, numcols, 1);
    Image img6(numrows, numcols, 1);

    img1.id = 1;
    img2.id = 2;
    img3.id = 3;
    img4.id = 4;
    img5.id = 5;
    img6.id = 6;

    img1.pixels = pixels1;
    img2.pixels = pixels2;
    img3.pixels = pixels3;
    img4.pixels = pixels4;
    img5.pixels = pixels5;
    img6.pixels = pixels6;

    vector<Image> images;
    images.push_back(img1);
    images.push_back(img2);
    images.push_back(img3);
    images.push_back(img4);
    images.push_back(img5);
    images.push_back(img6);


    string s("../src/test/input/parse_cluster_input_2.txt");
    auto cluster = classes_as_clusters(s, images);

    cluster.display();

    TEST_CHECK( cluster.cluster[0][0] == images[0] );
    TEST_CHECK( cluster.cluster[0][1] == images[1] );
    TEST_CHECK( cluster.cluster[0][2] == images[5] );
    TEST_CHECK( cluster.cluster[1][0] == images[3] );
    TEST_CHECK( cluster.cluster[1][1] == images[4] );
    TEST_CHECK( cluster.cluster[2][0] == images[2] );

}



TEST_LIST = {
    {"objective_f", test_objective_f},
    {"replace", test_replace},
    {"parse_clustering", test_parse_clustering},
    {"classes_as_clusters", test_classes_as_clusters},
    {NULL, NULL}
};