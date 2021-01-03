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
For the tests we ran we observed that the EMD metric was better than the Manhattan one for EMD cluster size 7x7 and it was worse than the Manhattan metric for cluster size 4x4. More specifically:
 * For 1000 input images and 10 queries
     * Manhattan
       * Average correct search results: 0.7
       * Average time for a search: 0.156
     * EMD with cluster size = 7x7 
       * Average correct search results: 0.82
       * Average time for a search: 21.960
     * EMD with cluster size = 4x4 
       * Average correct search results: 0.59
       * Average time for a search: 3.153
 * For 500 input images and 20 queries
     * Manhattan
       * Average correct search results: 0.675
       * Average time for a search: 0.082
     * EMD with cluster size = 7x7 
       * Average correct search results: 0.78
       * Average time for a search: 11.107
     * EMD with cluster size = 4x4 
       * Average correct search results: 0.58
       * Average time for a search: 1.370
#### Question D
