#include "countingalgo.h"
#include <fstream>


static ui array_intersection(VertexID* array_1, ui array_1_len, VertexID* array_2, ui array_2_len) {
    ui i = 0, j = 0;
    ui count = 0;

    while (i < array_1_len && j < array_2_len) {
        if (array_1[i] == array_2[j]) {
            count++;
            i++;
            j++;
        } 
        else if (array_1[i] < array_2[j]) {
            i++;
        } 
        else {
            j++;
        }
    }

    return count;
}

/*static VertexID array_intersection_elem(VertexID* array_1, ui array_1_len, VertexID* array_2, ui array_2_len) {
    ui i = 0, j = 0;
    ui count = 0;

    while (i < array_1_len && j < array_2_len) {
        if (array_1[i] == array_2[j]) {
            return array_1[i];
            count++;
            i++;
            j++;
        } 
        else if (array_1[i] < array_2[j]) {
            i++;
        } 
        else {
            j++;
        }
    }

    return count;
}*/


long long combinations(int n, int r)
{
    if (r < 0 || r > n)
        return 0;
    if (r == 0 || r == n)
        return 1;
    if (r > n / 2)
        r = n - r;
    long long res = 1;
    for (int k = 1; k <= r; ++k)
    {
        res = res * (n - k + 1);
        res /= k;
    }
    return res;
}


std::pair<VertexID, VertexID> CountingAlgorithm::get_wedge_endpoint_pair(VertexID u, VertexID v)
{

    if (u > v)
    {
        VertexID tmp = v;
        v = u;
        u = tmp;
    }

    return std::make_pair(u, v);
}


long long CountingAlgorithm::aggregate_square_count(std::map<std::pair<VertexID, VertexID>, ui> &wedge_map, long long &global_cnt)
{

    for (const auto &[key, value] : wedge_map)
    {
        global_cnt += combinations(value, 2);
    }

    return global_cnt / 2;
}


long long CountingAlgorithm::bfy_count_in_two_partition(Graph* graph, int l_ptn, int u_ptn){

    std::vector<VertexID> u_ptn_vertices;

    for(ui i = 0; i < graph->vertices_count; i++){
        if(graph->local_partition[i] == u_ptn){
            u_ptn_vertices.push_back(i);
        }
    }

    long long count_3 = 0;
    std::map<std::pair<VertexID, VertexID>, ui> wedge_map;
    std::pair<VertexID, VertexID> search_pair;
    VertexID* nbrs_1, v1;
    ui nbrs_1_cnt = 0;

    for (VertexID i = 0; i < u_ptn_vertices.size(); i++)
    {
        v1 = u_ptn_vertices[i];
        nbrs_1 = graph->getVertexNeighbors(v1, nbrs_1_cnt);

        for (ui j = 0; j < nbrs_1_cnt; j++)
        {
            for (ui k = j+1; k < nbrs_1_cnt; k++)
            {
                search_pair = std::make_pair(std::min(nbrs_1[j], nbrs_1[k]), std::max(nbrs_1[j], nbrs_1[k]));
                auto search = wedge_map.find(search_pair);
                if (search == wedge_map.end()){
                    wedge_map[search_pair] = 1;
                }
                else{
                    wedge_map[search_pair] = wedge_map[search_pair] + 1;
                }
            }
        }
    }

    for (auto const &[key, value] : wedge_map){
        count_3 += (value * (value - 1)) / 2;
    }

    std::cout << "Butterfly Count in Two Partition (" << l_ptn << ", " << u_ptn << ") : "  << count_3 << std::endl;

    return count_3;
}

long long CountingAlgorithm::bfy_count_in_three_partition(Graph* graph, int partition_count, int partition_no){

    long long count = 0;
    //std::map<std::pair<VertexID, VertexID>, ui> wedge_map;
    std::pair<VertexID, VertexID> search_pair;
    
    VertexID v1, v2;
    ui nbrs_1_cnt = 0, nbrs_2_cnt = 0;


    for (NodeID i = 0; i < partition_count; i++){
        if(i == partition_no){continue;}
        for(NodeID j = i + 1; j < partition_count; j++){
            if (j == partition_no){continue;}
            for(ui k = 0; k < graph->ptn_wise_nbr_cnt[i]; k++){
                v1 = graph->ptn_nbr_array[i][k];                
                for(ui l = 0; l < graph->ptn_wise_nbr_cnt[j]; l++){
                    v2 = graph->ptn_nbr_array[j][l];
                    count += graph->get_wedge_cnt_by_two_vertices(v1, v2);                
                }
            }    
        }        
    }   

    std::cout << "Butterfly Count in Three Partition : " << count << std::endl;

    return count;
}




long long CountingAlgorithm::sequential_db_count_square(Graph* graph){

    long long exact_count = 0;
    VertexID *nbrs_1, *nbrs_2, *nbrs_3, *nbrs_4;
    ui nbrs_1_cnt = 0, nbrs_2_cnt = 0, nbrs_3_cnt = 0, nbrs_4_cnt = 0;
    VertexID v2, v3, v4;


    // 1243 - Square
    long long count_1 = 0;

    for (VertexID v1 = 0; v1 < graph->vertices_count; v1++)
    {
        nbrs_1 = graph->getVertexNeighbors(v1, nbrs_1_cnt);
        for (VertexID j = 0; j < nbrs_1_cnt; j++)
        {
            v2 = nbrs_1[j];
            nbrs_2 = graph->getVertexNeighbors(v2, nbrs_2_cnt);
            for (VertexID k = 0; k < nbrs_1_cnt; k++)
            {
                count_1 = 0;
                v3 = nbrs_1[k];
                if(!(graph->is_smaller(v2, v3))){
                    continue;
                }

                nbrs_3 = graph->getVertexNeighbors(v3, nbrs_3_cnt);
                count_1 = array_intersection(nbrs_2, nbrs_2_cnt, nbrs_3, nbrs_3_cnt);
                exact_count += count_1;

            }
        }
    }

    std::cout << "Count 1 : " << exact_count << std::endl;
    // 1234 - Square

    long long count_2 = 0;

    for (VertexID v1 = 0; v1 < graph->vertices_count; v1++)
    {
        nbrs_1 = graph->getVertexNeighbors(v1, nbrs_1_cnt);
        for (VertexID j = 0; j < nbrs_1_cnt; j++)
        {
            v2 = nbrs_1[j];
            nbrs_2 = graph->getVertexNeighbors(v2, nbrs_2_cnt);
            for (VertexID k = 0; k < nbrs_2_cnt; k++)
            {
                count_2 = 0;
                v3 = nbrs_2[k];
                nbrs_3 = graph->getVertexNeighbors(v3, nbrs_3_cnt);
                count_2 = array_intersection(nbrs_1, nbrs_1_cnt, nbrs_3, nbrs_3_cnt);
                exact_count += count_2;
            }
        }
    }

    std::cout << "Count 2 : " << exact_count << std::endl;

    // 1324 - Square
    long long count_3 = 0;
    std::map<std::pair<VertexID, VertexID>, ui> wedge_map;
    std::pair<VertexID, VertexID> search_pair;

    for (VertexID v1 = 0; v1 < graph->vertices_count; v1++)
    {
        nbrs_1 = graph->getVertexNeighbors(v1, nbrs_1_cnt);

        for (ui j = 0; j < nbrs_1_cnt; j++)
        {
            for (ui k = j+1; k < nbrs_1_cnt; k++)
            {
                search_pair = std::make_pair(std::min(nbrs_1[j], nbrs_1[k]), std::max(nbrs_1[j], nbrs_1[k]));
                auto search = wedge_map.find(search_pair);
                if (search == wedge_map.end())
                {
                    wedge_map[search_pair] = 1;
                }
                else
                {
                    wedge_map[search_pair] = wedge_map[search_pair] + 1;
                }
            }
        }
    }

    for (auto const &[key, value] : wedge_map){
        count_3 += (value * (value - 1)) / 2;
    }

    std::cout << "Count 3 : " << count_3 << std::endl;

    exact_count += count_3;

    return exact_count;

}

long long CountingAlgorithm::count_interface_edge_square(Graph* graph){

    VertexID begin, end;
    VertexID* begin_nbrs, *end_nbrs;
    ui begin_nbr_cnt = 0, end_nbr_cnt = 0;
    NodeID self_ptn, v3_ptn, v4_ptn;

    long long interface_square_cnt = 0;

    for(ui i = 0; i < (graph->interface_edges).size(); i++){
        begin = (graph->interface_edges)[i].first;
        end = (graph->interface_edges)[i].second;

        self_ptn = graph->local_partition[begin];

        begin_nbrs = graph->getVertexNeighbors(begin, begin_nbr_cnt);
        end_nbrs = graph->getVertexNeighbors(end, end_nbr_cnt);

        for(ui j = 0; j < begin_nbr_cnt; j++){
            v3_ptn = graph->local_partition[begin_nbrs[j]];

            for(ui k = 0; k < end_nbr_cnt; k++){                
                v4_ptn = graph->local_partition[end_nbrs[k]];

                if((begin_nbrs[j] != end_nbrs[k]) && (graph->checkEdgeExistence(begin_nbrs[j], end_nbrs[k])) && 
                        (self_ptn != v3_ptn) && (self_ptn != v4_ptn)){
                    interface_square_cnt += 1;
                    //std::cout << interface_square_cnt << "--  " << graph->local_partition[begin] << "," << graph->local_partition[end] << ","<< v3_ptn << "," << v4_ptn << std::endl;
                }
            }
        }        
    }

    return interface_square_cnt;
}


long long CountingAlgorithm::db_count_square_in_cut_graph(Graph* cut_graph){

    long long exact_square_in_cut_graph = sequential_db_count_square(cut_graph);

    return exact_square_in_cut_graph;
}

long long CountingAlgorithm::db_count_square_in_local_graph(Graph* local_graph){

    long long exact_square_in_local_graph = sequential_db_count_square(local_graph);

    return exact_square_in_local_graph;
}

long long CountingAlgorithm::db_count_square_in_interface_graph(Graph* interface_graph){

    //long long exact_square_in_interface_graph = sequential_db_count_square(interface_graph);
    long long exact_square_in_interface_graph = count_interface_edge_square(interface_graph);
    
    return exact_square_in_interface_graph;
}

void CountingAlgorithm::db_count_square_in_whole_ptn_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphFromFile(file_path, vertex_partition_file_path, ptn_idx);

        // Graph* local_augmented_graph = new Graph();
        // local_graph->transformToAugmentedGraph(local_augmented_graph);

        // Graph* interface_graph = new Graph();
        // interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, ptn_idx);


        //Graph* interface_augmented_graph = new Graph();
        //interface_graph->transformToAugmentedGraph(interface_augmented_graph);

        

        // local_square_count = db_count_square_in_local_graph(local_augmented_graph);

        // local_interface_square_count = db_count_square_in_interface_graph(interface_graph);

        //std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        // std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        // for(int u_ptn_idx = ptn_idx + 1; u_ptn_idx < partition_cnt; u_ptn_idx++){
        //     deductible_count += bfy_count_in_two_partition(local_graph, ptn_idx, u_ptn_idx);
        // }

        local_graph->buildPartitionWiseNbrCntArray(partition_cnt, ptn_idx);
        three_ptn_deductible_count += bfy_count_in_three_partition(local_graph, partition_cnt, ptn_idx);

        // global_square_count += local_square_count;
        // global_square_count += local_interface_square_count;
    }

    // Graph* cut_graph = new Graph();
    // cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);

    // Graph* transformed_cut_graph = new Graph();
    // cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    // cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    // std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    
    // global_square_count += cut_graph_square_count;

    // global_square_count -= (2 * deductible_count);

    std::cout << "==============================================" << std::endl;
    std::cout << "Three Partition Deductible Square Count : " << three_ptn_deductible_count << std::endl;
    std::cout << "==============================================" << std::endl;

    std::cout << "==============================================" << std::endl;
    std::cout << "Deductible Square Count : " << deductible_count << std::endl;
    std::cout << "==============================================" << std::endl;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;

    



}