
#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>
#include "types.h"

struct hashFunction
{
  uint64_t operator()(const std::pair<VertexID, VertexID> &x) const
  {
    return (((uint64_t) x.first) << 32) | x.second;
  }
};


class Graph{

public:

    std::vector<VertexID> vertices;
    std::map<VertexID, VertexID> ghost_vertex_map;
    std::map<VertexID, VertexID> ghost_vertex_idx_map;
    ui ghost_vertices_count;
    ui vertices_count;
    ui edges_count;
    ui cut_edges_count;
    ui other_ptn_edges_count;
    ui max_degree;

    ui* degrees;
    ui* ghost_degrees;
    ui* main_degrees;

    ui* offsets;
    VertexID * neighbors;
    ui* ghost_offsets;
    VertexID* ghost_neighbors;

    NodeID* partition;
    NodeID* local_partition;

    ui* ptn_wise_nbr_cnt;
    VertexID** ptn_nbr_array;
    
    std::unordered_map<VertexID, VertexID> vertex_idx_map;
    std::unordered_map<VertexID, VertexID> other_ptn_vertex_idx_map;
    std::map<std::pair<VertexID, VertexID>, ui> wedge_map;
    std::map<std::pair<VertexID, VertexID>, ui> wedge_map_comm;

    //For Interface Graph
    std::vector<std::pair<VertexID, VertexID>> interface_edges;
    std::unordered_set<std::pair<VertexID, VertexID>, hashFunction> other_ptn_edges;

    Graph(){
        
        vertices_count = 0;
        edges_count = 0;
        max_degree = 0;
        other_ptn_edges_count = 0;

        offsets = NULL;
        neighbors = NULL;
        wedge_map.clear();
    }

    ~Graph() {
        delete[] offsets;
        delete[] neighbors;
    }

public:
    void loadGraphFromFile(const std::string& file_path);
    void loadKroneckerGraphFromFile(const std::string& file_path);
    void loadOrderedGraphFromFile(const std::string& file_path);
    void loadGraphMetaDataFromFile(const std::string& file_path);
    void loadGraphFromFileForBothDirectionEdges(const std::string& file_path);
    void loadOrderedGraphFromFileForBothDirectionEdges(const std::string& file_path);
    void loadGraphMetaDataFromFileForBothDirectionEdges(const std::string& file_path);

    void loadGraphAndPartitionFromFile(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadBidirectionalGraphAndPartitionFromFile(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphFromFile(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphWoCutEdgesFromFile(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphWoCutEdgesBidirection(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphFromFile(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphOptimized(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirection(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirectionOptimized(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadCutGraphFromFile(const std::string& file_path, const std::string& vtx_ptn_file);
    void loadCutGraphBidirection(const std::string& file_path, const std::string& vtx_ptn_file);
    void loadCutGraphFromCutEdgeFile(const std::string& file_path, const std::string& cut_edge_file_path);


    void loadGraphAndPartitionFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadBidirectionalGraphAndPartitionFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphWoCutEdgesFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphWoCutEdgesBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphOptimizedKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirectionOptimizedKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadCutGraphFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file);
    void loadCutGraphBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file);

    void loadKroneckerLocalGraphWoCutEdgesBidirection(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadKroneckerLocalGraphWoCutEdgesBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadKroneckerInterfaceGraphBidirectionOptimized(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadKroneckerInterfaceGraphBidirectionOptimizedKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadKroneckerCutGraphBidirection(const std::string& file_path, const std::string& vtx_ptn_file);
    void loadKroneckerCutGraphBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file);

    void convertGraphToMETISFormat(const std::string& output_file_path);
    void convertGraphToMETISFormatBidirectionalEdges(const std::string& output_file_path);
    void printGraphMetaData();
    void printEntireGraphData();

    bool is_smaller(VertexID u, VertexID v);
    void isKroneckerGraphUndirected(const std::string& file_path);
    
    void transformToAugmentedGraph(Graph* augmented_graph);
    void transformToAugmentedGraphWoPartition(Graph* augmented_graph);
    void buildPartitionWiseNbrCntArray (int partition_count, int partition_no){
        ptn_wise_nbr_cnt = new ui[partition_count];
        std::fill(ptn_wise_nbr_cnt, ptn_wise_nbr_cnt + partition_count, 0);

        ptn_nbr_array = new VertexID* [partition_count];

        std::vector<ui> ptn_offset(partition_count, 0);
        NodeID ptn;

        for(ui i = 0; i < vertices_count; i++){
            ptn = local_partition[i];
            if(ptn != partition_no){
                ptn_wise_nbr_cnt[ptn] += 1;
            }
        }

        for(ui i = 0; i < partition_count; i++){
            ptn_nbr_array[i] = new VertexID[ptn_wise_nbr_cnt[i]];
        }

        for(ui i = 0; i < vertices_count; i++){
            ptn = local_partition[i];
            if(ptn != partition_no){
                ptn_nbr_array[ptn][ptn_offset[ptn]] = i;
                ptn_offset[ptn] += 1;
            }
        } 
    }

    long long get_wedge_cnt_by_two_vertices(VertexID v1, VertexID v2);
    //long long sma_count_exact_square();

    const ui* getOffsets() const {
        return offsets;
    }

    const ui* getNeighbors() const {
        return neighbors;
    }

    const ui getVerticesCount() const {
        return vertices_count;
    }

    const ui getEdgesCount() const {
        return edges_count;
    }

    const ui getVertexDegree(const VertexID id) const {
        return offsets[id + 1] - offsets[id];
    }


    const ui getGraphMaxDegree() const {
        return max_degree;
    }

    ui * getVertexNeighbors(const VertexID id, ui& count) const {
        count = offsets[id + 1] - offsets[id];
        return neighbors + offsets[id];
    }

    ui * getVertexGhostNeighbors(const VertexID id, ui& count) const {
        count = ghost_offsets[id + 1] - ghost_offsets[id];
        return ghost_neighbors + ghost_offsets[id];
    }

    ui * getVertexNeighbors_partitioned(const VertexID vid, ui& count) const {
        
        VertexID id = (vertex_idx_map.find(vid))->second;
        count = offsets[id + 1] - offsets[id]; // used for neighbor count
        return neighbors + offsets[id];
    }

    void getNeighborCount(const VertexID id, ui& count) const {
        count = offsets[id + 1] - offsets[id];
    }

    void printNeighbors(const VertexID id) const {
        ui count = offsets[id + 1] - offsets[id];

        std::cout << "VID : " << id << " --- " ;

        for(ui i = 0; i < count; i++){
            std::cout << neighbors[offsets[id] + i] << "   "; 
        }

        std::cout << std::endl;
    }


    bool checkEdgeExistence(VertexID u, VertexID v) const {
        if (getVertexDegree(u) < getVertexDegree(v)) {
            std::swap(u, v);
        }
        ui count = 0;
        const VertexID* neighbors =  getVertexNeighbors(v, count);

        int begin = 0;
        int end = count - 1;
        while (begin <= end) {
            int mid = begin + ((end - begin) >> 1);
            if (neighbors[mid] == u) {
                return true;
            }
            else if (neighbors[mid] > u)
                end = mid - 1;
            else
                begin = mid + 1;
        }

        return false;
    }

};


#endif