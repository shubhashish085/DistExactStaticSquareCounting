cmake_minimum_required(VERSION 2.8)
project(DistExactStaticSquareCounting)

include_directories(
        ${PROJECT_SOURCE_DIR}/
)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -O3 -g -Wall -march=native -pthread")

set(DISTRIBUTED_SQUARE_COUNTING_FILE_LIST
        commandparser.h
        commandparser.cpp
        countingalgo.h
        countingalgo.cpp
        distributedrun.hpp
        distributedrun.cpp
        distributedrunhpec.cpp
        distributioncoordinator.hpp
        distributioncoordinator.cpp
        graph.h
        graph.cpp
        hpecgraph.cpp
        hpecgraph.hpp
        matchingcommand.h
        matchingcommand.cpp
        partition.hpp
        partition.cpp        
        StudyPerformance.cpp
        types.h)


add_executable(DistExactStaticSquareCounting.out ${DISTRIBUTED_SQUARE_COUNTING_FILE_LIST})

find_package(MPI REQUIRED)
target_link_libraries(DistExactStaticSquareCounting.out PUBLIC MPI::MPI_CXX)
