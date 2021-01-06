cmake_minimum_required(VERSION 3.16.3)
project(k23-hw3-fall-2020)
 
include_directories(include)
 
set(search_src src/main/search.cpp src/main/IDX.cpp src/main/Image.cpp src/main/LSH.cpp src/main/Dist.cpp src/main/HFunc.cpp src/main/GFunc.cpp src/main/ModOp.cpp)
 
set(cluster_src src/main/cluster.cpp src/main/Cluster.cpp src/main/IDX.cpp src/main/Image.cpp src/main/LSH.cpp src/main/Dist.cpp src/main/HFunc.cpp src/main/GFunc.cpp src/main/ModOp.cpp src/main/FFunc.cpp src/main/Hypercube.cpp)

set(Dist_test_src src/test/Dist_test.cpp src/main/Dist.cpp src/main/Image.cpp )
 
set(Cluster_test_src src/test/Cluster_test.cpp src/main/Cluster.cpp src/main/IDX.cpp src/main/Image.cpp src/main/LSH.cpp src/main/Dist.cpp src/main/HFunc.cpp src/main/GFunc.cpp src/main/ModOp.cpp src/main/FFunc.cpp src/main/Hypercube.cpp)

add_executable(search ${search_src})
add_executable(cluster ${cluster_src})
add_executable(Dist_test ${Dist_test_src})
add_executable(Cluster_test ${Cluster_test_src})