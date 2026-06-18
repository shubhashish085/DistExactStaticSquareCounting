#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <numeric>
#include <set>
#include <vector>
#include <fstream> 
#include <sstream>
#include <iostream>
#include <string>
#include <mpi.h>

#include "matchingcommand.h"
#include "types.h"
#include "countingalgo.h"
#include "analysis.h"
#include "distributedcountingalgo.h"

// Sequential Square Count
/*int main(int argc, char** argv){

    std::string input_data_graph_file = argv[1];

    clock_t read_begin_clock = clock();
    Graph* graph = new Graph();
    //graph->loadGraphFromFile(input_data_graph_file);
    graph->loadGraphFromFileForBothDirectionEdges(input_data_graph_file);
    double input_read_time = (double(clock() - read_begin_clock)) / CLOCKS_PER_SEC;    


    clock_t transformation_begin_clock = clock();
    Graph* augmented_graph = new Graph();
    graph->transformToAugmentedGraph(augmented_graph);
    double transformation_time = (double(clock() - transformation_begin_clock)) / CLOCKS_PER_SEC;    

    clock_t counting_begin_clock = clock();
    long long exact_count = CountingAlgorithm::sequential_db_count_square(augmented_graph);
    double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

    double total_time = input_read_time + transformation_time + counting_time;    

    std::cout << "==============================================" << std::endl;
    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
    std::cout << "Exact Square Count : " << exact_count << std::endl;    
    std::cout << "Input File Reading Time : " << input_read_time << " seconds" << std::endl;
    std::cout << "Transformation Time : " << transformation_time << " seconds" << std::endl;
    std::cout << "Counting Time : " << counting_time << " seconds" << std::endl;
    std::cout << "Total Time : " <<  total_time <<  " seconds" << std::endl;
    std::cout << "==============================================" << std::endl;

}*/


// Wedge Based Square Count
/*int main(int argc, char** argv){

    long long wedge_count = 0;
    long long square_count = 0;

    std::string input_data_graph_file = argv[1];
    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;

    Graph* graph = new Graph();
    graph->loadGraphFromFile(input_data_graph_file);

    for(ui i = 0; i < graph->getVerticesCount(); i++){
        wedge_count += (graph->degrees[i] * (graph->degrees[i] - 1)) / 2;
    }

    std::cout << "Total Wedge Count : " << wedge_count << std::endl;

    square_count = graph->count_exact_square();

    std::cout << "Total Square Count : " << square_count << std::endl;

    std::cout << "==================================================" << std::endl;

}*/



//Sequential Direction Based Square Count for Multiple Partitions
/*int main(int argc, char** argv){

    std::string input_data_graph_file = argv[1];
    std::string vertex_partition_file = argv[2];
    std::string partition_count = argv[3];

    int partition_cnt = std::stoi(partition_count);

    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
    std::cout << "Vertex Partition File : " << vertex_partition_file << std::endl;
    std::cout << "Partition Count : " << partition_count << std::endl;

    //CountingAlgorithm::optimized_db_count_square_in_whole_ptn_graph_seq(input_data_graph_file, vertex_partition_file, partition_cnt);     
    //CountingAlgorithm::optimized_db_count_square_in_bidirectional_graph_seq(input_data_graph_file, vertex_partition_file, partition_cnt);
    CountingAlgorithm::db_count_square_with_interface_graph_optimization(input_data_graph_file, vertex_partition_file, partition_cnt);     

    std::cout << "==================================================" << std::endl;

}*/


//Undirected graph Multiple Partitions
int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    std::string input_data_graph_file = argv[1];
    std::string vertex_partition_file = argv[2];
    std::string partition_count = argv[3];

    int partition_cnt = std::stoi(partition_count);

    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
    std::cout << "Vertex Partition File : " << vertex_partition_file << std::endl;
    std::cout << "Partition Count : " << partition_count << std::endl;

    //DistributedCountingAlgorithm::optimized_db_count_square_in_whole_ptn_graph_parallel(input_data_graph_file, vertex_partition_file, partition_cnt);
    DistributedCountingAlgorithm::db_count_square_with_interface_graph_optimization_detail(input_data_graph_file, vertex_partition_file, partition_cnt);

    MPI_Finalize();
}

//Bidirectional Edges
/*int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    std::string input_data_graph_file = argv[1];
    std::string vertex_partition_file = argv[2];
    std::string partition_count = argv[3];

    int partition_cnt = std::stoi(partition_count);

    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
    std::cout << "Vertex Partition File : " << vertex_partition_file << std::endl;
    std::cout << "Partition Count : " << partition_count << std::endl;

    DistributedCountingAlgorithm::optimized_db_count_square_in_whole_graph_bidirection_edges_parallel(input_data_graph_file, vertex_partition_file, partition_cnt);

    MPI_Finalize();
}*/

//Analysis
/*int main(int argc, char** argv){

    std::string input_data_graph_file = argv[1];
    std::string vertex_partition_file = argv[2];
    std::string partition_count = argv[3];

    int partition_cnt = std::stoi(partition_count);


    std::cout << "====================================================================================" << std::endl;

    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
    std::cout << "Vertex Partition File : " << vertex_partition_file << std::endl;
    std::cout << "Partition Count : " << partition_count << std::endl;

    Analysis::analyse_optimized_db_count_square_in_whole_ptn_graph_seq(input_data_graph_file, vertex_partition_file, partition_cnt);
    std::cout << "=====================================================================================" << std::endl;
}*/


//
/*int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    std::string input_data_graph_file = argv[1];
    std::string vertex_partition_file = argv[2];
    std::string partition_count = argv[3];

    int partition_cnt = std::stoi(partition_count);

    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
    std::cout << "Vertex Partition File : " << vertex_partition_file << std::endl;
    std::cout << "Partition Count : " << partition_count << std::endl;

    DistributedCountingAlgorithm::optimized_db_count_square_in_whole_ptn_graph_parallel_detail(input_data_graph_file, vertex_partition_file, partition_cnt);

    MPI_Finalize();
}*/

