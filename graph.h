
#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <random>
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

    ui* core;

    ui* offsets;
    VertexID * neighbors;
    ui* ghost_offsets;
    VertexID* ghost_neighbors;

    NodeID* partition;
    NodeID* local_partition;

    ui* ptn_wise_nbr_cnt;
    VertexID** ptn_nbr_array;

    bool left_for_bipartite_graph;
    
    std::unordered_map<VertexID, VertexID> vertex_idx_map;
    std::unordered_map<VertexID, VertexID> other_ptn_vertex_idx_map;
    std::unordered_map<VertexID, bool> self_ptn_cut_vertex_map;
    std::map<std::pair<VertexID, VertexID>, ui> wedge_map;
    std::map<std::pair<VertexID, VertexID>, ui> wedge_map_comm;

    //For Interface Graph
    std::vector<std::pair<VertexID, VertexID>> interface_edges;
    std::unordered_set<std::pair<VertexID, VertexID>, hashFunction> other_ptn_edges;
    std::vector<std::pair<VertexID, VertexID>> other_ptn_edge_list;

    std::vector<VertexID> random_order;

    Graph(){
        
        vertices_count = 0;
        edges_count = 0;
        max_degree = 0;
        other_ptn_edges_count = 0;
        left_for_bipartite_graph = true;

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
    void loadPartitionedLocalGraphWithOnlyCutEdgesFromFile(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphWithOnlyCutEdgesForBipartiteGraph(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphWoCutEdgesBidirection(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphFromFile(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphOptimized(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphLatest(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirection(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirectionOptimized(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirectionLatest(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadCutGraphFromFile(const std::string& file_path, const std::string& vtx_ptn_file);
    void loadCutGraphWithLocalCutEdges(const std::string& file_path, const std::string& vtx_ptn_file, NodeID ptn_idx, std::vector<std::pair<VertexID, VertexID>>& local_cut_edges);
    void loadCutGraphWithLocalCutEdgesOptimized(const std::string& file_path, const std::string& vtx_ptn_file, NodeID ptn_idx, std::vector<std::pair<VertexID, VertexID>>& local_cut_edges);    
    void loadCutGraphWithLocalCutEdgesOptForRplFactor(const std::string& file_path, const std::string& vtx_ptn_file, NodeID ptn_idx);
    void loadCutGraphWithLocalCutEdgesBiEdgesOptForRplFactor(const std::string& file_path, const std::string& vtx_ptn_file, NodeID ptn_idx);
    void loadCutGraphBidirection(const std::string& file_path, const std::string& vtx_ptn_file);
    void loadCutGraphFromCutEdgeFile(const std::string& file_path, const std::string& cut_edge_file_path);


    void loadGraphAndPartitionFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadBidirectionalGraphAndPartitionFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphWoCutEdgesFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedLocalGraphWoCutEdgesBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphOptimizedKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphOptimizedKahipLatest(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirectionOptimizedKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadPartitionedInterfaceGraphBidirectionLatestKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadCutGraphWithLocalCutEdgesOptForRplFactorKahip(const std::string& file_path, const std::string& vtx_ptn_file, NodeID ptn_idx);
    void loadCutGraphWithLocalCutEdgesBiEdgesOptForRplFactorKahip(const std::string& file_path, const std::string& vtx_ptn_file, NodeID ptn_idx);
    void loadCutGraphFromFileKahip(const std::string& file_path, const std::string& vtx_ptn_file);
    void loadCutGraphBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file);

    void loadKroneckerLocalGraphWoCutEdgesBidirection(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadKroneckerLocalGraphWoCutEdgesBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadKroneckerInterfaceGraphBidirectionOptimized(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadKroneckerInterfaceGraphBidirectionOptimizedKahip(const std::string& file_path, const std::string& vtx_ptn_file, int partition_no);
    void loadKroneckerCutGraphBidirection(const std::string& file_path, const std::string& vtx_ptn_file);
    void loadKroneckerCutGraphBidirectionKahip(const std::string& file_path, const std::string& vtx_ptn_file);

    void computeCoreForVertices();

    void convertGraphToMETISFormat(const std::string& output_file_path);
    void convertGraphToMETISFormatBidirectionalEdges(const std::string& output_file_path);
    void printGraphMetaData();
    void printEntireGraphData();

    bool is_smaller(VertexID u, VertexID v);
    bool is_smaller_ro(VertexID u, VertexID v);
    bool is_smaller_core(VertexID u, VertexID v);
    bool is_smaller_core_only(VertexID u, VertexID v);
    void isKroneckerGraphUndirected(const std::string& file_path);
    
    void transformToAugmentedGraph(Graph* augmented_graph);
    void transformToAugmentedGraphWithRandomOrdering(Graph* augmented_graph);
    void transformToAugmentedGraphWithVertexOrdering(Graph* augmented_graph);
    void transformToAugmentedGraphWithCoreOrdering(Graph* augmented_graph);
    void transformToAugmentedGraphWithOnlyCoreOrdering(Graph* augmented_graph);
    void transformToAugmentedGraphWoPartition(Graph* augmented_graph);
    void transformToAugmentedGraphWoPartitionAndVertexOrdering(Graph* augmented_graph);
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

        for(int i = 0; i < partition_count; i++){
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

    void print_graph_data(){

        double avg_degree = 0.0;

        for (ui i = 0; i < vertices_count; i++){
            avg_degree = degrees[i];
        }

        if(vertices_count != 0){
            avg_degree /= vertices_count;
        }

        std::cout << "Vertices Count : " << vertices_count << " Edges Count : " << edges_count << " Avg Degree : " << avg_degree << ""  << std::endl;
        std::cout << "Ghost Vertices Count : " << ghost_vertices_count <<  "Cut Graph Edge Count : " << cut_edges_count << std::endl;
    }


    void print_interface_graph_details(){

        std::cout << "Interface Edge Count : " << interface_edges.size() <<  std::endl;
    }

    void print_wedge_map_stats(){
        std::cout << "Map Size : " << wedge_map.size() << std::endl; 
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


    void deleteAndClear(){

        delete[] offsets;
        delete[] neighbors;
        delete[] ghost_offsets;
        delete[] ghost_neighbors;
        
        vertex_idx_map.clear();
        wedge_map.clear();
        other_ptn_vertex_idx_map.clear();
        interface_edges.clear();
        other_ptn_edges.clear();
        other_ptn_edge_list.clear();
    }

    void deleteAndClearForCutGraph(){

        delete[] offsets;
        delete[] neighbors;
        
        vertex_idx_map.clear();
        wedge_map.clear();
        other_ptn_vertex_idx_map.clear();
        interface_edges.clear();
        other_ptn_edges.clear();
        other_ptn_edge_list.clear();
    }

    void deleteAndClearForCutGraphForBipartite(){

        delete[] offsets;
        delete[] neighbors;
        delete[] ghost_offsets;
        delete[] ghost_neighbors;
        
        vertex_idx_map.clear();
        wedge_map.clear();
        other_ptn_vertex_idx_map.clear();
        interface_edges.clear();
        other_ptn_edges.clear();
        other_ptn_edge_list.clear();
    }


};


#endif