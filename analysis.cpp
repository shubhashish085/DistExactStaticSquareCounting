#include <iostream>
#include "graph.h"
#include "analysis.h"
#include "countingalgo.h"



void Analysis::print_graph_metadata(Graph* graph){

    std::cout << "Local Graph Vertices Count : " << graph->vertices_count << std::endl;
    std::cout << "Ghost Vertices Count : " << graph->ghost_vertices_count << std::endl;
    std::cout << "Local Graph Edges Count : " << graph->edges_count << std::endl;
    std::cout << "Ghost Edges Count : " << graph->cut_edges_count << std::endl;    
}

void Analysis::degree_based_analysis(Graph* graph){

    long long total_degree = 0, ghost_total_degree = 0;
    double avg_degree = 0.0, ghost_avg_degree = 0.0;

    for(ui i = 0; i < graph->vertices_count; i++){
        total_degree += graph->degrees[i];
    }

    for(ui i = 0; i < graph->ghost_vertices_count; i++){
        ghost_total_degree += graph->ghost_degrees[i];
    }

    avg_degree = (double) total_degree / (double)(graph->vertices_count);
    ghost_avg_degree = (double) ghost_total_degree / (double)(graph->ghost_vertices_count);

    std::cout << "Partition Graph : Total Degree - " << total_degree << ", Average Degree - " << avg_degree  << std::endl;    
    std::cout << "Partition Ghost Graph : Total Degree - " << total_degree << ", Average Degree - " << avg_degree  << std::endl;
}


void Analysis::print_detailed_statistics(Graph* graph, int partition_idx, double* time_array, long long square_count){

    std::cout << "===================================="<< std::endl; 
    std::cout << "Partition Index : " << partition_idx << std::endl;
    print_graph_metadata(graph);
    degree_based_analysis(graph);
    std::cout << "Exact Square Count : " << square_count << std::endl;
    std::cout << "Reading Time : " << time_array[0] << std::endl;
    std::cout << "Transformation Time : " << time_array[1] << std::endl;
    std::cout << "Counting Time : " << time_array[2] << std::endl;
    std::cout << "===================================="<< std::endl;
}

void Analysis::analyse_optimized_db_count_square_in_whole_ptn_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        clock_t read_begin_clock = clock();
        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, ptn_idx);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, ptn_idx);
        double input_read_time = (double(clock() - read_begin_clock)) / CLOCKS_PER_SEC;

        clock_t transformation_begin_clock = clock();
        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);
        double transformation_time = (double(clock() - transformation_begin_clock)) / CLOCKS_PER_SEC;    

        clock_t counting_begin_clock = clock();
        local_square_count = CountingAlgorithm::db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = CountingAlgorithm::count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = CountingAlgorithm::db_count_square_in_interface_graph(interface_graph);
        double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

        double* time_array = new double[3];
        time_array[0] = input_read_time;
        time_array[1] = transformation_time;
        time_array[2] = counting_time;

        print_detailed_statistics(local_graph, ptn_idx, time_array, (local_square_count + local_cut_edge_square_count));

    }

    clock_t read_begin_clock = clock();
    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);
    double input_read_time = (double(clock() - read_begin_clock)) / CLOCKS_PER_SEC;
    

    clock_t transformation_begin_clock = clock();
    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);
    double transformation_time = (double(clock() - transformation_begin_clock)) / CLOCKS_PER_SEC;  

    clock_t counting_begin_clock = clock();
    cut_graph_square_count = CountingAlgorithm::db_count_square_in_cut_graph(transformed_cut_graph);
    double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

    double* time_array = new double[3];
    time_array[0] = input_read_time;
    time_array[1] = transformation_time;
    time_array[2] = counting_time;

    print_detailed_statistics(cut_graph, -1, time_array, cut_graph_square_count);    
}