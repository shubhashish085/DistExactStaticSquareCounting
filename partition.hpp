#ifndef DISTRIBUTEDSQUARECOUNTING_PARTITION_H
#define DISTRIBUTEDSQUARECOUNTING_PARTITION_H

#include <map>
#include "graph.h"
#include "hpecgraph.hpp"
#include "types.h"

class GraphPartitioning {

public:

    static long long hash_vertex(VertexID& v, ui& numberOfPartitions);

    static void even_degree_partition(Graph* data_graph, VertexID*& vtx_partition_array, ui& n_partition);
    static void hpec_even_degree_partition(HpecGraph* data_graph, ui& n_partition);
    static void even_degree_partition(Graph* data_graph, ui& n_partition, ui*& partition_limit);

    static void partition_with_degree_refinement(Graph* data_graph, NodeID*& metis_part, ui& numberOfPartitions, NodeID*& final_part);
    static void partition_with_target_degree_refinement(Graph* data_graph, NodeID*& metis_part, ui& numberOfPartitions, NodeID*& final_part, double threshold);
};


#endif