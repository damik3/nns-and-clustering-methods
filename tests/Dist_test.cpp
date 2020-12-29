#include "acutest.h"
#include "Dist.hpp"
#include "Image.hpp"

#include <cmath>
#include <vector>

using namespace std;

vector<PIXEL_T> v_6x6_1{ 1, 2, 3, 4, 5, 6, 2, 3, 4, 5, 6, 1, 3, 4, 5, 6, 1, 2, 4, 5, 6, 1, 2, 3, 5, 6, 1, 2, 3, 4, 6, 1, 2, 3, 4, 5 };
vector<PIXEL_T> v_6x6_2{ 6, 5, 4, 3, 2, 1, 5, 4, 3, 2, 1, 6, 4, 3, 2, 1, 6, 5, 3, 2, 1, 6, 5, 4, 2, 1, 6, 5, 4, 3, 1, 6, 5, 4, 3, 2 };



void test_all_1(void) {
    int rows = 4;
    int cols = 4;
    vector<PIXEL_T> pixels1{ 1, 2, 5, 6, 3, 4, 7, 8, 9, 10, 13, 14, 11, 12, 15, 16 };
    vector<PIXEL_T> pixels2{ 11, 12, 15, 16, 9, 10, 13, 14, 3, 4, 7, 8, 1, 2, 5, 6 };
    Image img1(rows, cols, 1);
    Image img2(rows, cols, 1);
    img1.pixels = pixels1;
    img2.pixels = pixels2;
    TEST_CHECK( dist(img1, img2, Manhattan) == 128 );
    TEST_CHECK( abs( dist(img1, img2, Euclidean) - sqrt(1088) ) < 0.00001 );
}



void test_all_2(void) {
    int rows = 2;
    int cols = 2;
    vector<PIXEL_T> pixels1{ 1, 2, 3, 4 };
    vector<PIXEL_T> pixels2{ 3, 4, 1, 2 };
    Image img1(rows, cols, 1);
    Image img2(rows, cols, 1);
    img1.pixels = pixels1;
    img2.pixels = pixels2;
    TEST_CHECK( dist(img1, img2, Manhattan) == 8 );
    TEST_CHECK( abs( dist(img1, img2, Euclidean) - sqrt(16) ) < 0.00001 );
}



void test_all_3(void) {
    int rows = 1;
    int cols = 1;
    vector<PIXEL_T> pixels1{ 1 };
    vector<PIXEL_T> pixels2{ 1 };
    Image img1(rows, cols, 1);
    Image img2(rows, cols, 1);
    img1.pixels = pixels1;
    img2.pixels = pixels2;
    TEST_CHECK( dist(img1, img2, Manhattan) == 0 );
    TEST_CHECK( abs( dist(img1, img2, Euclidean) - sqrt(0) ) < 0.00001 );
}



void test_all_4(void) {
    int rows = 1;
    int cols = 1;
    Image img1(rows, cols, 1);
    Image img2(rows+1, cols+1, 1);
    TEST_EXCEPTION(dist(img1, img2, Manhattan), runtime_error);
    TEST_EXCEPTION(dist(img1, img2, Euclidean), runtime_error);
}



void test_emd_1(void) {

    int rows = 4;
    int cols = 4;
    vector<PIXEL_T> pixels1{ 1, 2, 5, 6, 3, 4, 7, 8, 9, 10, 13, 14, 11, 12, 15, 16 };
    vector<PIXEL_T> pixels2{ 11, 12, 15, 16, 9, 10, 13, 14, 3, 4, 7, 8, 1, 2, 5, 6 };
    Image img1(rows, cols, 1);
    Image img2(rows, cols, 1);


    TEST_EXCEPTION(dist(img1, img2, Emd, 3), runtime_error);
    dist(img1, img2, Emd, 4);
    TEST_EXCEPTION(dist(img1, img2, Emd, 5), runtime_error);
    TEST_EXCEPTION(dist(img1, img2, Emd, 6), runtime_error);

}



void test_emd_2(void) {

    int rows = 6;
    int cols = 6;
    vector<PIXEL_T> pixels1{ 1, 2, 3, 4, 5, 6, 2, 3, 4, 5, 6, 1, 3, 4, 5, 6, 1, 2, 4, 5, 6, 1, 2, 3, 5, 6, 1, 2, 3, 4, 6, 1, 2, 3, 4, 5 };
    vector<PIXEL_T> pixels2{ 6, 5, 4, 3, 2, 1, 5, 4, 3, 2, 1, 6, 4, 3, 2, 1, 6, 5, 3, 2, 1, 6, 5, 4, 2, 1, 6, 5, 4, 3, 1, 6, 5, 4, 3, 2 };
    Image img1(rows, cols, 1);
    Image img2(rows, cols, 1);

    dist(img1, img2, Emd, 1);
    TEST_EXCEPTION(dist(img1, img2, Emd, 2), runtime_error);
    TEST_EXCEPTION(dist(img1, img2, Emd, 3), runtime_error);
    dist(img1, img2, Emd, 4);
    TEST_EXCEPTION(dist(img1, img2, Emd, 5), runtime_error);
    TEST_EXCEPTION(dist(img1, img2, Emd, 6), runtime_error);
    TEST_EXCEPTION(dist(img1, img2, Emd, 7), runtime_error);

}



void is_perfect_square_test(void) {
    TEST_CHECK(is_perfect_square(2) == false);
    TEST_CHECK(is_perfect_square(3) == false);
    TEST_CHECK(is_perfect_square(4) == true);
    TEST_CHECK(is_perfect_square(5) == false);
    TEST_CHECK(is_perfect_square(9) == true);
    TEST_CHECK(is_perfect_square(16) == true);
    TEST_CHECK(is_perfect_square(17) == false);
}



void cluster_pos_test(void) {
    
    pair<int, int> p1(0, 2);
    pair<int, int> p2(0, 0);
    pair<int, int> p3(4, 4);
    pair<int, int> p4(0, 3);
    pair<int, int> p5(0, 0);
    pair<int, int> p6(3, 3);
    
    TEST_CHECK( cluster_pos(v_6x6_1, 3, 2) == p1 );
    TEST_CHECK( cluster_pos(v_6x6_1, 0, 2) == p2 );
    TEST_CHECK( cluster_pos(v_6x6_1, 8, 2) == p3 );

    TEST_CHECK( cluster_pos(v_6x6_1, 2, 3) == p4 );
    TEST_CHECK( cluster_pos(v_6x6_1, 0, 3) == p5 );
    TEST_CHECK( cluster_pos(v_6x6_1, 3, 3) == p6 );


    TEST_EXCEPTION( cluster_pos(v_6x6_1, 9, 2), runtime_error );
    TEST_EXCEPTION( cluster_pos(v_6x6_1, -1, 2), runtime_error );
    TEST_EXCEPTION( cluster_pos(v_6x6_1, 4, 3), runtime_error );
    TEST_EXCEPTION( cluster_pos(v_6x6_1, -1, 3), runtime_error );
}



void cluster_sum_test(void) {

    pair<int, int> p1(0, 2);
    pair<int, int> p2(0, 0);
    pair<int, int> p3(4, 4);
    pair<int, int> p4(0, 3);
    pair<int, int> p5(0, 0);
    pair<int, int> p6(3, 3);
    pair<int, int> p7(-1, 3);
    pair<int, int> p8(3, 6);
    pair<int, int> p9(6, -3);

    TEST_CHECK( cluster_sum(v_6x6_2, p1, 2) == (double)12 );
    TEST_CHECK( cluster_sum(v_6x6_2, p2, 2) == (double)10 );
    TEST_CHECK( cluster_sum(v_6x6_2, p3, 2) == (double)10 );

    TEST_CHECK( cluster_sum(v_6x6_2, p4, 3) == (double)36 );
    TEST_CHECK( cluster_sum(v_6x6_2, p5, 3) == (double)27 );
    TEST_CHECK( cluster_sum(v_6x6_2, p6, 3) == (double)27 );

    TEST_EXCEPTION( cluster_sum(v_6x6_2, p7, 3), runtime_error );
    TEST_EXCEPTION( cluster_sum(v_6x6_2, p8, 3), runtime_error );
    TEST_EXCEPTION( cluster_sum(v_6x6_2, p9, 3), runtime_error );
}



TEST_LIST = {
    { "test_all_1", test_all_1 },
    { "test_all_2", test_all_2 },
    { "test_all_3", test_all_3 },
    { "test_all_4", test_all_4 },
    //{ "test_emd_1", test_emd_1 },
    //{ "test_emd_2", test_emd_2 },
    { "is_perfect_square_test", is_perfect_square_test },
    {"cluster_pos_test", cluster_pos_test},
    {"cluster_sum_test", cluster_sum_test},
    { NULL, NULL }
};
