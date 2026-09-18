#ifndef DESC_STATS_H
#define DESC_STATS_H

#include <map>
#include <algorithm>
#include "graph.h"
#include "types.h"

class Stats {

public:

    void printStatsOfPartitionedGraph (const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count);
    void writeStatsToFile (const std::string& file_path, const std::string& vertex_partition_file_path, const std::string& output_file_path, int partition_count);    
};


#endif
