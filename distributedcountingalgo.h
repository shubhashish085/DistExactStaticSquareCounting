#ifndef DESC_DIST_ALGORITHM_H
#define DESC_DIST_ALGORITHM_H

#include <map>
#include "graph.h"
#include "types.h"

class DistributedCountingAlgorithm {

public:

    static void optimized_db_count_square_in_whole_ptn_graph_parallel(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count); 
};


#endif
