# k23-hw3-fall-2020

## General description

Implementation of various methods for nearest neighbor search and clustering of images for the MNIST database of handwritten digits. Part of an undergrad course homework.

- **Question A:** For this question, we implemented an autoencoder neural network which we tested for different architectures.

- **Question B:** For this question, we took the latent vector representation (basically a 10-d vector from question A) for all images (query and train set) and we used brute force to find for each query, the nearest neighbor in that 10-d vector space. We compared it with brute force in the original 28x28-d vector space and with the Locality Sensitive Hashing (LSH) method we had implemented in a previous homework of the same course. The Manhattan Distance was used for all comparisons above.

- **Question C:** For this question, we implemented the Earth Mover's Distance (EMD) as a different way of measuring the distance between two vectors (or two images) which basically reduces to solving a Linear Programming problem. For each query, we computed its 10 nearest neighbors using EMD and brute force and compared the results bewteen the two.

- **Question D:** For this question, we did three different clusterings of the train set images. Clustering #1 was done by using images in the original 28x28-d vector space. Clustering #2 was done by using the images' latent vector representation as in Question B that is, in the 10-d vector space. Clustering #3 comes of a convolional nerual network for classification we had implemented again in a previous homework of the same course. Again we did tests and compared the results

## Build 
```
mkdir build && cd build 
cmake .. 
make
```

## Sample Run
All following commands are given from the project root directory. 

- **Question A**
  ```
  python python/reduce.py -d ./datasets/train-images-idx3-ubyte -dupto 2500 -q ./datasets/t10k-images-idx3-ubyte -qupto 100 -od ./train-reduced-2500 -oq ./query-reduced-100
  ```
---

- **Question B**
  ```
  ./build/search -d ./datasets/train-images-idx3-ubyte -i ./train-reduced-2500 -dupto 2500 -q ./datasets/t10k-images-idx3-ubyte -s ./query-reduced-100  -qupto 100 -k 2 -L 8 -w 40 -o question2.out
  ``` 
---

- **Question C**
  ```
  python ./python/search.py -d ./datasets/train-images-idx3-ubyte -dupto 1000 -q ./datasets/t10k-images-idx3-ubyte -qupto 10 -l1 ./datasets/train-labels-idx1-ubyte -l2 ./datasets/t10k-labels-idx1-ubyte -o question3.out -EMD
  ```
---

- **Question D**
  ```
  python python/classification.py -d ./datasets/train-images-idx3-ubyte -d1 ./datasets/train-labels-idx1-ubyte -t ./datasets/t10k-images-idx3-ubyte -t1 ./datasets/t10k-labels-idx1-ubyte -model ./python/weights-2-32-64-1-30-32.h5 -o classification1.out -dupto 1000 -tupto 100
  ```
  
  and then
  
  ```
  ./build/cluster -d ./datasets/train-images-idx3-ubyte -i train-reduced-2500 -dupto 1000 -n classification1.out -c cluster.conf -o question4.out
  ```
  
  Note that in the configuration file `cluster.conf` only the fist parameter `number_of_clusters` is actually used by the execeutable. The others are leftovers from hw1.

---

## Results and Observations

#### Question A
For this question we tested are architectures based on avoiding overfitting and small loss percentage both on train and test set. Our executable, after training, it also prints the first 10 original images and their respective autoencoded ones, so we can check with our own eyes if it indeed does a good job at encoding.

We tested the following NN architectures and got these results:

  * Arch 1 : Good behaviour, avoided overfitting, recreated original images with success (input size: 2500, queries 100) . 
    ```
    * convolution layers:                          2
    * size of filter 1:                           32
    * size of filter 2:                           64
    * number of convolutional filters per layer:   1
    * number of epochs:                           20
    * batch size:                                 64  
    ```
  * Arch 2: Good behaviour, avoided overfitting, recreated original images with success (input size:2500, queries: 100). Corresponding files a_query_new_1, q_train_new_1.
    ```
    * convolution layers:                          2
    * size of filter 1:                           40
    * size of filter 2:                           80
    * number of convolutional filters per layer:   1
    * number of epochs:                           25
    * batch size:                                 64
    ```
  * Arch 3: Not good behaviour, avoided overfitting but was not able to succesfully recreate autoencoded images (input size:2500, queries: 100). The only thing we changed from Arch2 was the batch size.
    ```
    * convolution layers:                          2
    * size of filter 1:                           40
    * size of filter 2:                           80
    * number of convolutional filters per layer:   1
    * number of epochs:                           25
    * batch size:                                100
    ```
  * Arch 4: Good behaviour, avoided overfitting, recreated original images with success (input size:2500, queries: 100). Corresponding files a_query_new_2, q_train_new_2.
    ```
    * convolution layers:                          2
    * size of filter 1:                           60
    * size of filter 2:                           60
    * number of convolutional filters per layer:   1
    * number of epochs:                           20
    * batch size:                                 64
    ```
    * Arch 5: Good behaviour, avoided overfitting, recreated original images with success (input size:2500, queries: 100). Corresponding files a_query_new_4, q_train_new_4.
    ```
    * convolution layers:                          2
    * size of filter 1:                           28
    * size of filter 2:                           84
    * number of convolutional filters per layer:   1
    * number of epochs:                           25
    * batch size:                                 64
    ```
  

#### Question B
For this question we only tested the architectures from question 1 that produced good results. Most of the times (but not always), the NeuralNet method did better than LSH. They did about the same time, but NeuralNet had an overall better approximation factor. It obviously depends on the Architecture and hyperparameters of question 1, but we showed that it can perform better than LSH when tuned appropriately.

Our `search` executable for this question, prints each query, and for each query the Brute Force, LSH and NeuralNet closet neighbor if you want to check the results with your own eyes.

  * For Arch 1, LSH and NeuralNet were about the same. We got:
    ```
     * tReduced:  598.057
     * tLSH:      499.478
     * tTrue:    8031.740
     * Approximation Factor LSH:      1.57590
     * Approximation Factor Reduced:  1.55799
    ```
  * For Arch 2, NeuralNet did better than LSH. We got:
    ```
     * tReduced:  364.818
     * tLSH:      367.164
     * tTrue:    3720.040
     * Approximation Factor LSH:      1.46096
     * Approximation Factor Reduced:  1.27103
    ```
  * For Arch 4, NeuraNet did better than LSH. We got:
    ```
     * tReduced:  741.992
     * tLSH:      794.935
     * tTrue:    8672.910
     * Approximation Factor LSH:      1.42435
     * Approximation Factor Reduced:  1.29990
    ```
  * For Arch 5, LSH did better than LSH. We got:
    ```
     * tReduced:  370.492
     * tLSH:      507.651
     * tTrue:    4745.400
     * Approximation Factor LSH:      1.44375
     * Approximation Factor Reduced:  1.72814
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
    * Original space Silhouette:          0.182337
    * Original space objective function:  1.65468e+07
    * New space Silhouette:               0.176998
    * New space objective function:       1.72742e+07
  ```
  * For number of clusters = 16
  ```
    * Original space Silhouette:          0.190027
    * Original space objective function:  1.67176e+07
    * New space Silhouette:               0.163812
    * New space objective function:       1.77052e+07
  ```
  * For number of clusters = 14
  ```
    * Original space Silhouette:          0.183848
    * Original space objective function:  1.71709e+07
    * New space Silhouette:               0.188102
    * New space objective function:       1.84484e+07
  ```
  * For number of clusters = 12
  ```
    * Original space Silhouette:          0.175698
    * Original space objective function:  1.80835e+07
    * New space Silhouette:               0.149321
    * New space objective function:       1.83353e+07
  ```
  * For number of clusters = 10
  ```
    * Original space Silhouette:          0.175865
    * Original space objective function:  1.84824e+07
    * New space Silhouette:               1.84824e+07
    * New space objective function:       1.91691e+07
  ```
  * For Neural Net Clustering with high accuracy and number of clusters = 10
  ```
    * Silhouette:          0.1406
    * Objective function:  1.95733e+07
  ```
