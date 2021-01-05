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
    * -dupto <int>: number of images to read from input file both in original and new space

## Comments and Observations

#### Question A
For this question we tested the following NN architectures and got these results:
  * Arch 1: Good behaviour, avoided overfitting, recreated original images with success.
    ```
    * convolution layers: 2
    * size of filter 1: 32
    * size of filter 2: 64
    * number of convolutional filters per layer: 1
    * number of epochs: 20
    * batch size: 64  
    ```
  * Arch 2: Bad behaviour, could not recreate original images.
    ```
    * convolution layers: 2
    * size of filter 1: 64
    * size of filter 2: 96
    * number of convolutional filters per layer: 1
    * number of epochs: 20
    * batch size: 125
    ```  
  * Arch 3: OK behaviour. avoided overfitting, recreated original images with partial success.
    ```
    * convolution layers: 2
    * size of filter 1: 32
    * size of filter 2: 32
    * number of convolutional filters per layer: 1
    * number of epochs: 20
    * batch size: 32
    ```
  * Arch 4: Bad behaviour, could not recreate original images.
    ```
    * convolution layers: 2
    * size of filter 1: 64
    * size of filter 2: 96
    * number of convolutional filters per layer: 1
    * number of epochs: 20
    * batch size: 50
    ```
  * Arch 5: Good behaviour, avoided overfitting, recreated original images with success (input size:2500, queries: 100).
    ```
    * convolution layers: 2
    * size of filter 1: 40
    * size of filter 2: 80
    * number of convolutional filters per layer: 1
    * number of epochs: 25
    * batch size: 64
    ```

#### Question B
For this question we only tested the architectures that produced good results.
  * For Arch 1, LSH and NeuralNet were about the same. We got:
    ```
    tReduced: 598.057
    tLSH: 499.478
    tTrue: 8031.74
    Approximation Factor LSH: 1.5759
    Approximation Factor Reduced: 1.55799
    ```
  * For Arch 5, NeuralNet did better thatn LSH. We got:
    ```
    tReduced: 364.818
    tLSH: 367.164
    tTrue: 3720.04
    Approximation Factor LSH: 1.46096
    Approximation Factor Reduced: 1.27103
    ```

#### Question C
For the tests we ran we observed that the EMD metric was better than the Manhattan one for EMD configured with 49 clusters (with cluster size 4x4) and it was worse than the Manhattan metric configured with 16 clusters (with cluster size 7x7) clusters. More specifically:
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
