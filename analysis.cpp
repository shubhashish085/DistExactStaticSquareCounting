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


long long Analysis::communication_analysis_directed_graph(Graph* graph){

    std::map<std::pair<VertexID, VertexID>, ui> wedge_map;
    std::pair<VertexID, VertexID> search_pair;
    VertexID *nbrs_p, *nbrs_q;
    ui nbrs_cnt_p = 0, nbrs_cnt_q = 0;


    long long comm_size = 0, comm_1 = 0, comm_2 = 0;

    for (VertexID p = 0; p < graph->vertices_count; p++){
        nbrs_p = graph->getVertexNeighbors(p, nbrs_cnt_p);

        for (ui j = 0; j < nbrs_cnt_p; j++){
            for(ui k = j + 1; k < nbrs_cnt_p; k++){
                
                search_pair = std::make_pair( std::min(nbrs_p[j], nbrs_p[k]) , std::max(nbrs_p[j], nbrs_p[k]));
                
                auto search_result = wedge_map.find(search_pair);
                if (search_result == wedge_map.end()){
                    wedge_map[search_pair] = 1;
                }
            }
        }
    }

    comm_size += (wedge_map.size() * 3);

    //std::cout << "First Step Done " << std::endl;

    NodeID partition_p, partition_q, partition_r;
    ui nbr_cnt_other_ptn_p = 0, nbr_cnt_other_ptn_q = 0;
    VertexID p, q, r, s;

    // communication -> pqrs

    for (VertexID p = 0; p < graph->vertices_count; p++){
        
        partition_p = graph->partition[p];
        nbrs_p = graph->getVertexNeighbors(p, nbrs_cnt_p);
        nbr_cnt_other_ptn_p = 0;

        for(ui j = 0; j < nbrs_cnt_p; j++){

            q = nbrs_p[j];
            partition_q = graph->partition[q];

            if(partition_p != partition_q){
                nbr_cnt_other_ptn_p += 1;
            }            

            nbrs_q = graph->getVertexNeighbors(q, nbrs_cnt_q);
            nbr_cnt_other_ptn_q = 0;

            for(ui k = 0; k < nbrs_cnt_q; k++){
                r = nbrs_q[k];
                partition_r = graph->partition[r];

                if(partition_r != partition_q){
                    nbr_cnt_other_ptn_q += 1;
                }                
            }

            comm_1 += (nbr_cnt_other_ptn_q * nbrs_cnt_p);
        }

        comm_1 += (nbr_cnt_other_ptn_p * nbrs_cnt_p);
    }

    comm_size += comm_1;

    //std::cout << "Second Step Done " << std::endl;

    // communication -> pqsr

    for (VertexID p = 0; p < graph->vertices_count; p++)
    {
        partition_p = graph->partition[p];
        nbrs_p = graph->getVertexNeighbors(p, nbrs_cnt_p);
        nbr_cnt_other_ptn_p = 0;

        for (VertexID j = 0; j < nbrs_cnt_p; j++)
        {
            q = nbrs_p[j];
            partition_q = graph->partition[q];
            nbrs_q = graph->getVertexNeighbors(q, nbrs_cnt_q);

            if(partition_p != partition_q){
                nbr_cnt_other_ptn_p += 1;
            }

            nbr_cnt_other_ptn_q = 0;
            for (VertexID k = 0; k < nbrs_cnt_p; k++){
                r = nbrs_p[k];

                if(!(graph->is_smaller(q, r))){
                    continue;
                }

                nbr_cnt_other_ptn_q += 1;                
            }

            comm_2 += (nbr_cnt_other_ptn_q * nbrs_cnt_q);
        }

        comm_2 += (nbr_cnt_other_ptn_p * nbrs_cnt_p);
    }

    comm_size += comm_2;

    //std::cout << "Third Step Done " << std::endl;

    std::cout << "Wedge Map Communication Size : " << wedge_map.size() << std::endl;
    std::cout << "PQRS Communication Volume : " << comm_1 << std::endl;
    std::cout << "PQSR Communication Volume : " << comm_2 << std::endl;

    return comm_size;
}


void Analysis::analyse_replication_factor(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long actual_vertices_count = 0, global_vertics_count = 0, ptn_vertices_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        ptn_vertices_count = 0;

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, ptn_idx);

        ptn_vertices_count = local_graph->vertices_count + local_graph->ghost_vertices_count;
        global_vertics_count += ptn_vertices_count;

        std::cout << "Partition : " << ptn_idx << " - Vertices Count : " << ptn_vertices_count << std::endl;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);

    global_vertics_count += cut_graph->vertices_count;
    std::cout << "Cut Graph Vertices Count : " << cut_graph->vertices_count << std::endl;
    std::cout << "Global Vertices Count : " << global_vertics_count << std::endl;
    std::cout << "==========================================================" << std::endl;

}


void Analysis::analyse_replication_factor_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long actual_vertices_count = 0, global_vertics_count = 0, ptn_vertices_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        ptn_vertices_count = 0;

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFileKahip(file_path, vertex_partition_file_path, ptn_idx);

        ptn_vertices_count = local_graph->vertices_count + local_graph->ghost_vertices_count;
        global_vertics_count += ptn_vertices_count;

        std::cout << "Partition : " << ptn_idx << " - Vertices Count : " << ptn_vertices_count << std::endl;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFileKahip(file_path, vertex_partition_file_path);

    global_vertics_count += cut_graph->vertices_count;
    std::cout << "Cut Graph Vertices Count : " << cut_graph->vertices_count << std::endl;
    std::cout << "Global Vertices Count : " << global_vertics_count << std::endl;
    std::cout << "==========================================================" << std::endl;

}



void Analysis::analyse_edge_replication_factor(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long actual_vertices_count = 0, global_edges_count = 0, ptn_edges_count = 0;
    double edge_replication_factor = 0.0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        ptn_edges_count = 0;

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, ptn_idx);

        ptn_edges_count = (local_graph->edges_count * 2) + local_graph->cut_edges_count;
        global_edges_count += ptn_edges_count;

        std::cout << "Partition : " << ptn_idx << " - Edges Count : " << ptn_edges_count << std::endl;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);

    global_edges_count += cut_graph->edges_count;
    std::cout << "Cut Graph Edges Count : " << cut_graph->edges_count << std::endl;
    std::cout << "Global Edges Count : " << global_edges_count << std::endl;
    std::cout << "==========================================================" << std::endl;

}


void Analysis::analyse_edge_replication_factor_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long actual_vertices_count = 0, global_edges_count = 0, ptn_edges_count = 0;
    double edge_replication_factor = 0.0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        ptn_edges_count = 0;

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFileKahip(file_path, vertex_partition_file_path, ptn_idx);

        ptn_edges_count = (local_graph->edges_count * 2) + local_graph->cut_edges_count;
        global_edges_count += ptn_edges_count;

        std::cout << "Partition : " << ptn_idx << " - Edges Count : " << ptn_edges_count << std::endl;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFileKahip(file_path, vertex_partition_file_path);

    global_edges_count += cut_graph->edges_count;
    std::cout << "Cut Graph Edges Count : " << cut_graph->edges_count << std::endl;
    std::cout << "Global Edges Count : " << global_edges_count << std::endl;
    std::cout << "==========================================================" << std::endl;

}


void Analysis::analyse_replication_factor_for_bidirectional_edges(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long actual_vertices_count = 0, global_vertics_count = 0, ptn_vertices_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        ptn_vertices_count = 0;

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesBidirection(file_path, vertex_partition_file_path, ptn_idx);

        ptn_vertices_count = local_graph->vertices_count + local_graph->ghost_vertices_count;
        global_vertics_count += ptn_vertices_count;

        std::cout << "Partition : " << ptn_idx << " - Vertices Count : " << ptn_vertices_count << std::endl;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphBidirection(file_path, vertex_partition_file_path);

    global_vertics_count += cut_graph->vertices_count;
    std::cout << "Cut Graph Vertices Count : " << cut_graph->vertices_count << std::endl;
    std::cout << "Global Vertices Count : " << global_vertics_count << std::endl;
    std::cout << "==========================================================" << std::endl;

}


void Analysis::analyse_edge_replication_factor_for_bidirectional_edges(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long actual_vertices_count = 0, global_edges_count = 0, ptn_edges_count = 0;
    double edge_replication_factor = 0.0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        ptn_edges_count = 0;

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesBidirection(file_path, vertex_partition_file_path, ptn_idx);

        ptn_edges_count = (local_graph->edges_count) + (local_graph->cut_edges_count / 2);
        global_edges_count += ptn_edges_count;

        std::cout << "Partition : " << ptn_idx << " - Edges Count : " << ptn_edges_count << std::endl;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphBidirection(file_path, vertex_partition_file_path);

    global_edges_count += (cut_graph->edges_count / 2);
    std::cout << "Cut Graph Edges Count : " << cut_graph->edges_count << std::endl;
    std::cout << "Global Edges Count : " << global_edges_count << std::endl;
    std::cout << "==========================================================" << std::endl;

}

