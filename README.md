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
  * `./build/cluster` with extra optional command line arguements
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
For the tests we ran we observed that the EMD metric was better than the Manhattan one for EMD configured with 49 clusters (with cluster size 4x4) and it was worse than the Manhattan metric for EMD configured with 16 clusters (with cluster size 7x7). More specifically:
 * For 1000 input images and 10 queries
     * Manhattan 
        ```
       * Average correct search results: 0.700
       * Average time per query:         0.156
        ```
     * EMD with 49 clusters 
        ```
       * Average correct search results: 0.820
       * Average time per query:        21.960
        ```
     * EMD with 16 clusters 
        ```
       * Average correct search results: 0.590
       * Average time per query:         3.153
        ```
 * For 500 input images and 20 queries
     * Manhattan
        ```
       * Average correct search results: 0.675
       * Average time per query:         0.082
        ```
     * EMD with 49 clusters
        ```
       * Average correct search results: 0.780
       * Average time per query:        11.107
        ```
     * EMD with 16 clusters
        ```
       * Average correct search results: 0.580
       * Average time per query:         1.370
        ```

#### Question D
We experimented with `n = number of clusters`, ranging from 10 to 18, and we noticed best behaviour for Original Space Clustering (OSC) and New Space Clustering (NSC) for `n = 14`. Both OSC and NSC had a similar Silhouette but the NSC run about 10 times faster. The only reason NSC is faster is the big change in dimensions. OSC has 784-item vectors when NSC has 10-item vectors. 

For the Neaural Network Clustering (NNC), while the Silhouette metric was almost never better than OSC and NSC, when we evaluated the clustering by viewing the images in each cluster ourselves, the NNC was obviously better than both the other two. This led us to the thought that the Silhouette function combined with the Manhattan metric is not a very effective evaluation function for image classification.

Also note, that our `cluster` executable, after it completes the clustering and writing to the output file, it also prints for each clustering (OSC, NSC and NNC), the 4 first centroids with their first 10 images accordingly. That is so you can see for yourselves how good each method classifies the images.

Below we present some of our experiments in further detail:

  * For number of clusters = 18
  ```
    * Original space Silhouette: 		      0.182337
    * Original space objective function: 	1.65468e+07
    * New space Silhouette: 			        0.176998
    * New space objective function:  	    1.72742e+07
  ```
  * For number of clusters = 16
  ```
    * Original space Silhouette: 		      0.190027
    * Original space objective function: 	1.67176e+07
    * New space Silhouette: 			        0.163812
    * New space objective function:  	    1.77052e+07
  ```
  * For number of clusters = 14
  ```
    * Original space Silhouette: 		      0.183848
    * Original space objective function: 	1.71709e+07
    * New space Silhouette: 			        0.188102
    * New space objective function:  	    1.84484e+07
  ```
  * For number of clusters = 12
  ```
    * Original space Silhouette: 		      0.175698
    * Original space objective function: 	1.80835e+07
    * New space Silhouette: 			        0.149321
    * New space objective function:  	    1.83353e+07
  ```
  * For number of clusters = 10
  ```
    * Original space Silhouette: 		      0.175865
    * Original space objective function: 	1.84824e+07
    * New space Silhouette: 			        1.84824e+07
    * New space objective function:  	    1.91691e+07
  ```
  * For Neural Net Clustering with high accuracy and number of clusters = 10
  ```
    * Silhouette: 		      0.1406
    * Objective function: 	1.95733e+07
  ```
