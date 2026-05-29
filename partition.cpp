#include "partition.hpp"
#include <cstring>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>


void GraphPartitioning::even_degree_partition(Graph* data_graph, VertexID*& vtx_partition_array, ui& n_partition) {


    ui* cand_degree_offset = new ui[data_graph->getVerticesCount() + 1];
    cand_degree_offset[0] = 0;
    ui* candidate_limit = new ui[n_partition];

    for(ui j = 0; j < data_graph->getVerticesCount() ; j++){
        cand_degree_offset[j + 1] = cand_degree_offset[j] + data_graph->getVertexDegree(j);
    }

    ui total_degree = cand_degree_offset[data_graph->getVerticesCount()];
    ui avg_degree = total_degree / n_partition;

    ui last_node_degree_offset = 0;
    ui node_idx = 0;

    for(ui j = 0; j < data_graph->getVerticesCount(); j++){
        if(node_idx == n_partition - 1){
            candidate_limit[node_idx++] = data_graph->getVerticesCount() - 1;
            last_node_degree_offset = cand_degree_offset[data_graph->getVerticesCount()];
            break;
        }else if(j == data_graph->getVerticesCount() - 1){
            candidate_limit[node_idx++] = j;
            last_node_degree_offset = cand_degree_offset[j + 1];
        }else if(cand_degree_offset[j + 1] - last_node_degree_offset >= avg_degree){
            candidate_limit[node_idx++] = j;
            last_node_degree_offset = cand_degree_offset[j + 1];
        }
    }

    ui start_point = 0, k  = 0;
    while (k < n_partition){

        for(ui i = start_point; i <= candidate_limit[k]; i++){
            vtx_partition_array[i] = k;
        }

        start_point = candidate_limit[k];
        k++;
    }
        
}


void GraphPartitioning::hpec_even_degree_partition(HpecGraph* data_graph, ui& n_partition) {


    ui* cand_degree_offset = new ui[data_graph->getVerticesCount() + 1];
    cand_degree_offset[0] = 0;
    ui* candidate_limit = new ui[n_partition];

    for(ui j = 0; j < data_graph->getVerticesCount() ; j++){
        cand_degree_offset[j + 1] = cand_degree_offset[j] + data_graph->getVertexDegree(j);
    }

    ui total_degree = cand_degree_offset[data_graph->getVerticesCount()];
    ui avg_degree = total_degree / n_partition;

    ui last_node_degree_offset = 0;
    ui node_idx = 0;

    for(ui j = 0; j < data_graph->getVerticesCount(); j++){
        if(node_idx == n_partition - 1){
            candidate_limit[node_idx++] = data_graph->getVerticesCount() - 1;
            last_node_degree_offset = cand_degree_offset[data_graph->getVerticesCount()];
            break;
        }else if(j == data_graph->getVerticesCount() - 1){
            candidate_limit[node_idx++] = j;
            last_node_degree_offset = cand_degree_offset[j + 1];
        }else if(cand_degree_offset[j + 1] - last_node_degree_offset >= avg_degree){
            candidate_limit[node_idx++] = j;
            last_node_degree_offset = cand_degree_offset[j + 1];
        }
    }

    ui start_point = 0, k  = 0;
    while (k < n_partition){

        for(ui i = start_point; i <= candidate_limit[k]; i++){
            data_graph->vertex_partition_map[i] = k;
        }

        start_point = candidate_limit[k];
        k++;
    }
        
}



void GraphPartitioning::even_degree_partition(Graph* data_graph, ui& n_partition, ui*& partition_limit) {


    ui* cand_degree_offset = new ui[data_graph->getVerticesCount() + 1];
    cand_degree_offset[0] = 0;

    for(ui j = 0; j < data_graph->getVerticesCount() ; j++){
        cand_degree_offset[j + 1] = cand_degree_offset[j] + data_graph->getVertexDegree(j);
    }

    ui total_degree = cand_degree_offset[data_graph->getVerticesCount()];
    ui avg_degree = total_degree / n_partition;

    ui last_node_degree_offset = 0;
    ui node_idx = 0;

    for(ui j = 0; j < data_graph->getVerticesCount(); j++){
        if(node_idx == n_partition - 1){
            partition_limit[node_idx++] = data_graph->getVerticesCount() - 1;
            last_node_degree_offset = cand_degree_offset[data_graph->getVerticesCount()];
            break;
        }else if(j == data_graph->getVerticesCount() - 1){
            partition_limit[node_idx++] = j;
            last_node_degree_offset = cand_degree_offset[j + 1];
        }else if(cand_degree_offset[j + 1] - last_node_degree_offset >= avg_degree){
            partition_limit[node_idx++] = j;
            last_node_degree_offset = cand_degree_offset[j + 1];
        }
    }        
}

void GraphPartitioning::partition_with_degree_refinement(Graph* data_graph, NodeID*& metis_part, ui& n_partition, NodeID*& final_part){

    long long* load = new long long[n_partition];
    std::fill(load, load + n_partition, 0);

    ui* cand_degree_offset = new ui[data_graph->getVerticesCount() + 1];
    cand_degree_offset[0] = 0;

    for(ui j = 0; j < data_graph->getVerticesCount() ; j++){
        cand_degree_offset[j + 1] = cand_degree_offset[j] + data_graph->getVertexDegree(j);
    }

    ui total_degree = cand_degree_offset[data_graph->getVerticesCount()];
    ui target_degree = total_degree / n_partition;

    for(ui i = 0; i < data_graph->getVerticesCount(); i++){
        load[metis_part[i]] += data_graph->degrees[i];
    }

    std::vector<ui> order(data_graph->getVerticesCount());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return data_graph->degrees[a] > data_graph->degrees[b];
    });

    for (int v : order) {
        int bestPart = metis_part[v];
        int minLoad = load[bestPart];

        for (int p = 0; p < n_partition; p++) {
            if (load[p] < minLoad) {
                bestPart = p;
                minLoad = load[p];
            }
        }

        if (bestPart != metis_part[v]) {
            load[metis_part[v]] -= data_graph->degrees[v];
            //metis_part[v] = bestPart;
            load[bestPart] += data_graph->degrees[v];
        }

        final_part[v] = bestPart;        
    }    
}

void GraphPartitioning::partition_with_target_degree_refinement(Graph* data_graph, NodeID*& metis_part, ui& n_partition, NodeID*& final_part, double threshold){

    long long* load = new long long[n_partition];
    std::fill(load, load + n_partition, 0);

    std::memcpy(final_part, metis_part, sizeof(NodeID));

    std::vector<NodeID> highly_loaded_partitions;
    std::vector<NodeID> less_loaded_partitions;

    ui* cand_degree_offset = new ui[data_graph->getVerticesCount() + 1];
    cand_degree_offset[0] = 0;

    for(ui j = 0; j < data_graph->getVerticesCount() ; j++){
        cand_degree_offset[j + 1] = cand_degree_offset[j] + data_graph->getVertexDegree(j);
    }

    ui total_degree = cand_degree_offset[data_graph->getVerticesCount()];
    ui target_degree = total_degree / n_partition;

    for(ui i = 0; i < data_graph->getVerticesCount(); i++){
        load[metis_part[i]] += data_graph->degrees[i];
    }

    for(ui i = 0; i < n_partition; i++){
        if(load[i] > threshold * target_degree){
            highly_loaded_partitions.push_back(i);
        }else if(load[i] < target_degree){
            less_loaded_partitions.push_back(i);
        }
    }

    std::vector<VertexID> order(data_graph->getVerticesCount());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](VertexID a, VertexID b) {
        return data_graph->degrees[a] > data_graph->degrees[b];
    });

    std::sort(less_loaded_partitions.begin(), less_loaded_partitions.end(), [&](int a, int b) {
        return load[a] < load[b];
    });

    int bestPart;
    std::vector<VertexID> ptn_vertices;
    ui loaded_ptn_idx;  

    for(ui i = 0; i < highly_loaded_partitions.size(); i++){
        loaded_ptn_idx = highly_loaded_partitions[i];
        for(ui j = 0; j < order.size(); j++){
            if(metis_part[order[j]] == loaded_ptn_idx){
                ptn_vertices.push_back(order[j]);
            }            
        }

        for(ui j = 0; j < ptn_vertices.size(); j++){
            if(less_loaded_partitions.size() > 0 && load[less_loaded_partitions[0]] < target_degree){
                final_part[ptn_vertices[j]] = less_loaded_partitions[0];
                load[less_loaded_partitions[0]] += data_graph->degrees[ptn_vertices[j]];
                load[loaded_ptn_idx] -= data_graph->degrees[ptn_vertices[j]];

            }else if (less_loaded_partitions.size() > 0){
                less_loaded_partitions.erase(less_loaded_partitions.begin());
            }

            if(load[loaded_ptn_idx] <= threshold * target_degree){
                break;
            }
        }

        ptn_vertices.clear();
    }   
}



long long GraphPartitioning::hash_vertex(VertexID& v, ui& numberOfPartitions) {
    uint32_t x = v;

    // 32-bit mix (MurmurHash3 finalizer style)
    x ^= x >> 16;
    x *= 0x85ebca6bU;
    x ^= x >> 13;
    x *= 0xc2b2ae35U;
    x ^= x >> 16;

    return (static_cast<long long>(x % (numberOfPartitions - 1)) + 1);
}
