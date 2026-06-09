#ifndef DSC_ALGORITHM_H
#define DSC_ALGORITHM_H

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
    static long long db_count_square_in_cut_graph(Graph* graph);
    static long long db_count_square_in_local_graph(Graph* graph);
    static long long db_count_square_in_interface_graph(Graph* graph);

};


#endif
