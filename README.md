# k23-hw3-fall-2020

## Authors
 * Sideris Nikolaos, sdi1600153
 * Vargiamis Michail, sdi1300018

## Build 
```
mkdir build && cd build 
cmake .. 
make
```

## Run
From the project root directory:
  * `python ./python/reduce.py` with extra optional command line arguements
    * -dupto <int>: number of images to read from train set
    * -qupto <int>: number of images to read from query set
  * `./build/search` with extra optional command line arguements
    * -dupto <int>: number of images to read from train set
    * -qupto <int>: number of images to read from query set
    * -w <int>: w parameter for LSH
  * `python ./python/search` with extra optional command line arguements
    * -dupto <int>: number of images to read from train set
    * -qupto <int>: number of images to read from query set
  * `./build/cluster`

## Comments and Observations
#### Question A
#### Question B
#### Question C
For the tests we ran we observed that the EMD metric was better than the Manhattan one for EMD configured with 49 clusters (with cluster size 4x4) and it was worse than the Manhattan metric configured with 16 (with cluster size 7x7) clusters. More specifically:
 * For 1000 input images and 10 queries
     * Manhattan
       * Average correct search results: 0.7
       * Average time per query: 0.156
     * EMD with 49 clusters 
       * Average correct search results: 0.82
       * Average time per query: 21.960
     * EMD with 16 clusters 
       * Average correct search results: 0.59
       * Average time per query: 3.153
 * For 500 input images and 20 queries
     * Manhattan
       * Average correct search results: 0.675
       * Average time per query: 0.082
     * EMD with 49 clusters
       * Average correct search results: 0.78
       * Average time per query: 11.107
     * EMD with 16 clusters
       * Average correct search results: 0.58
       * Average time per query: 1.370
#### Question D
