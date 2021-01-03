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
 
 
