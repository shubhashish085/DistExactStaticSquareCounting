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

#include "matchingcommand.h"
#include "types.h"
#include "countingalgo.h"

// int main(int argc, char** argv){

//     std::string input_data_graph_file = argv[1];
//     std::string vertex_partition_file = argv[2];
//     std::string partition_no = argv[3];

//     int n_partition = std::stoi(partition_no);

//     Graph* graph = new Graph();
//     graph->loadGraphFromFile(input_data_graph_file);

//     long long comm_cost = CountingAlgorithm::naive_comm_cost_analysis(graph, vertex_partition_file, n_partition);

//     std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
//     std::cout << "Vertex Partition File : " << vertex_partition_file << std::endl;
//     std::cout << "Partition No : " << n_partition << std::endl;
//     std::cout << "Naive Communication Cost : " << comm_cost << std::endl;

// }

/*int main(int argc, char** argv){

    std::string input_data_graph_file = argv[1];
    std::string vertex_partition_file = argv[2];
    std::string partition_no = argv[3];

    int n_partition = std::stoi(partition_no);

    Graph* graph = new Graph();
    graph->loadGraphFromFile(input_data_graph_file);
    
    Graph* augmented_graph = new Graph();
    graph->transformToAugmentedGraph(augmented_graph);

    long long exact_count = CountingAlgorithm::sequential_db_count_square(augmented_graph);

    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
    std::cout << "Vertex Partition File : " << vertex_partition_file << std::endl;
    std::cout << "Exact Square Count : " << exact_count << std::endl;

}*/


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
int main(int argc, char** argv){

    std::string input_data_graph_file = argv[1];
    std::string vertex_partition_file = argv[2];
    std::string partition_count = argv[3];

    int partition_cnt = std::stoi(partition_count);

    std::cout << "Input Graph File : " << input_data_graph_file << std::endl;
    std::cout << "Vertex Partition File : " << vertex_partition_file << std::endl;
    std::cout << "Partition Count : " << partition_count << std::endl;

    CountingAlgorithm::optimized_db_count_square_in_whole_ptn_graph_seq(input_data_graph_file, vertex_partition_file, partition_cnt);     

    std::cout << "==================================================" << std::endl;

}

