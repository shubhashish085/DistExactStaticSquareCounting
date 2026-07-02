#ifndef DESC_ALGORITHM_H
#define DESC_ALGORITHM_H

#include <map>
#include "graph.h"
#include "types.h"

class CountingAlgorithm {

public:

    static std::pair<VertexID, VertexID> get_wedge_endpoint_pair(VertexID u, VertexID v);
    static long long aggregate_square_count(std::map<std::pair<VertexID, VertexID>, ui>& wedge_map, long long&  global_cnt);
    
    static long long sequential_wb_count_square(Graph* graph);
    static long long sequential_db_count_square(Graph* graph);
    

    static void wb_count_square_in_partitioned_graph(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_no);
    static void opt_wb_count_square_in_partitioned_graph(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_no);

    static void db_count_square_in_partitioned_graph(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_no);
    static void db_count_square_in_whole_ptn_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void optimized_db_count_square_in_whole_ptn_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void optimized_db_count_square_in_bidirectional_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);

    static void db_count_square_with_interface_graph_optimization(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void db_count_square_with_interface_graph_optimization_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);

    static long long db_count_square_in_cut_graph(Graph* graph);
    static long long db_count_square_in_local_graph(Graph* graph);
    static long long db_count_square_in_interface_graph(Graph* graph);
    static long long db_count_square_in_interface_graph_optimized(Graph* graph);
    static long long count_square_from_other_ptn_per_vertex(Graph* graph);
    static long long bfy_count_in_two_partition(Graph* graph, int l_ptn, int u_ptn);
    static long long bfy_count_in_three_partition(Graph* graph, int partition_count, int partition_no);    

    static long long count_interface_edge_square(Graph* graph);
    static long long count_interface_edge_square_optimized(Graph* graph);
};


#endif
