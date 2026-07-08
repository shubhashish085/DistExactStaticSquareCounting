#ifndef DESC_ANALYSIS_H
#define DESC_ANALYSIS_H

#include <map>
#include "graph.h"
#include "types.h"

class Analysis {

public:

    static void print_graph_metadata(Graph* graph);
    static void degree_based_analysis(Graph* graph);

    //static void opt_wb_count_square_in_partitioned_graph(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_no);

    static long long communication_analysis_directed_graph(Graph* graph);

    static void analyse_optimized_db_count_square_in_whole_ptn_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    //static void analyse_optimized_db_count_square_in_bidirectional_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void analyse_replication_factor(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void analyse_edge_replication_factor(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);

    static void analyse_replication_factor_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void analyse_edge_replication_factor_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);


    static void analyse_replication_factor_for_bidirectional_edges(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void analyse_edge_replication_factor_for_bidirectional_edges(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);

    static void analyse_replication_factor_for_bidirectional_edges_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void analyse_edge_replication_factor_for_bidirectional_edges_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);


    static void print_detailed_statistics(Graph* graph, int ptn_idx, double* time_array, long long square_count);

};


#endif