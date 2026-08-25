#ifndef DESC_DIST_ALGORITHM_H
#define DESC_DIST_ALGORITHM_H

#include <map>
#include "graph.h"
#include "types.h"

class DistributedCountingAlgorithm {

public:

    static void optimized_db_count_square_in_whole_ptn_graph_parallel(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void optimized_db_count_square_in_whole_ptn_graph_parallel_detail(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void db_count_square_with_interface_graph_optimization_detail(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void db_count_square_with_interface_graph_optimization_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void db_count_square_with_interface_graph_latest_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 

    static void optimized_db_count_square_in_whole_graph_bidirection_edges_parallel(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void db_count_square_with_interface_graph_optimization_in_bi_edges_detail(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void db_count_square_with_interface_graph_optimization_in_bi_edges_latest(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void db_count_square_with_interface_graph_optimization_in_bi_edges_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void db_count_square_with_interface_graph_optimization_in_bi_edges_kahip_latest(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
    static void db_count_square_in_knkr(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    static void db_count_square_in_knkr_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 

};


#endif
