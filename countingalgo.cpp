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


long long CountingAlgorithm::count_square_from_other_ptn_per_vertex(Graph* graph){

    long long count = 0;
    VertexID v1, v2;
    VertexID* nbrs, *v1_nbrs, *v2_nbrs;
    ui nbrs_cnt = 0, v1_nbrs_cnt = 0, v2_nbrs_cnt = 0;
    

    for(VertexID i = 0; i < graph->ghost_vertices_count; ++i){
        
        nbrs = graph->getVertexGhostNeighbors(i, nbrs_cnt);
        for(VertexID j = 0; j < nbrs_cnt; j++){
            v1 = nbrs[j];
            v1_nbrs = graph->getVertexNeighbors(v1, v1_nbrs_cnt);
            for(VertexID k = j + 1; k < nbrs_cnt; k++){
                v2 = nbrs[k];
                v2_nbrs = graph->getVertexNeighbors(v2, v2_nbrs_cnt);
                count += array_intersection(v1_nbrs, v1_nbrs_cnt, v2_nbrs, v2_nbrs_cnt);
            }
        }
    }

    return count;
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

    //std::cout << "Count 1 : " << exact_count << std::endl;
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

    //std::cout << "Count 2 : " << exact_count << std::endl;

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

    //std::cout << "Count 3 : " << count_3 << std::endl;

    exact_count += count_3;

    return exact_count;

}


long long CountingAlgorithm::sequential_cpb_count_square(Graph* graph){

    long long exact_count = 0;
    VertexID *nbrs_1, *nbrs_2;
    ui nbrs_1_cnt = 0, nbrs_2_cnt = 0;
    VertexID* v1_nbr_idx, *v2_nbr_idx; 
    VertexID v2;

    for (VertexID v1 = 0; v1 < graph->vertices_count; v1++)
    {
        nbrs_1 = graph->getVertexNeighbors(v1, nbrs_1_cnt);
        for (VertexID j = 0; j < nbrs_1_cnt; j++)
        {
            v2 = nbrs_1[j];
            if(v1 >= v2){
                continue;
            }
            
            nbrs_2 = graph->getVertexNeighbors(v2, nbrs_2_cnt);
            v1_nbr_idx = std::upper_bound(nbrs_1, nbrs_1 + nbrs_1_cnt, v2);
            v2_nbr_idx = std::upper_bound(nbrs_2, nbrs_2 + nbrs_2_cnt, v1);

            for(VertexID* left_ptr = v1_nbr_idx; left_ptr < nbrs_1 + nbrs_1_cnt; left_ptr++){
                for(VertexID* right_ptr = v2_nbr_idx; right_ptr < nbrs_2 + nbrs_2_cnt; right_ptr++){
                    if((*left_ptr != *right_ptr) && graph->checkEdgeExistence(std::min(*left_ptr, *right_ptr) , std::max(*left_ptr, *right_ptr))){
                        exact_count++;
                    }
                } 
            }

        }
    }

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


long long CountingAlgorithm::count_interface_edge_square_optimized(Graph* graph){

    VertexID begin, end, min_vtx, max_vtx;
    VertexID* begin_nbrs, *end_nbrs;
    ui begin_nbr_cnt = 0, end_nbr_cnt = 0;

    long long interface_square_cnt = 0;

    for(ui i = 0; i < (graph->interface_edges).size(); i++){
        begin = (graph->interface_edges)[i].first;
        end = (graph->interface_edges)[i].second;

        begin_nbrs = graph->getVertexNeighbors(begin, begin_nbr_cnt);
        end_nbrs = graph->getVertexNeighbors(end, end_nbr_cnt);

        for(ui j = 0; j < begin_nbr_cnt; j++){
            for(ui k = 0; k < end_nbr_cnt; k++){

                min_vtx =  std::min(begin_nbrs[j], end_nbrs[k]);
                max_vtx =  std::max(begin_nbrs[j], end_nbrs[k]); 

                if((min_vtx != max_vtx) && ((graph->other_ptn_edges).find(std::make_pair(min_vtx, max_vtx)) != (graph->other_ptn_edges).end())){
                    interface_square_cnt += 1;
                }
            }
        }        
    }

    return interface_square_cnt;
}


long long CountingAlgorithm::count_interface_edge_square_latest(Graph* graph){

    VertexID begin, end, begin_nbr;
    VertexID* begin_nbrs, *end_nbrs, *nbrs;
    ui begin_nbr_cnt = 0, end_nbr_cnt = 0, nbr_cnt = 0;

    long long interface_square_cnt = 0;

    for(ui i = 0; i < (graph->interface_edges).size(); i++){
        begin = (graph->interface_edges)[i].first;
        end = (graph->interface_edges)[i].second;

        begin_nbrs = graph->getVertexNeighbors(begin, begin_nbr_cnt);
        end_nbrs = graph->getVertexNeighbors(end, end_nbr_cnt);

        for(ui j = 0; j < begin_nbr_cnt; j++){

            begin_nbr = begin_nbrs[j];
            nbrs = graph->getVertexGhostNeighbors(begin_nbr, nbr_cnt);
            interface_square_cnt += array_intersection(nbrs, nbr_cnt, end_nbrs, end_nbr_cnt);            
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

long long CountingAlgorithm::db_count_square_in_interface_graph_optimized(Graph* interface_graph){

    long long exact_square_in_interface_graph = count_interface_edge_square_optimized(interface_graph);
    
    return exact_square_in_interface_graph;
}

long long CountingAlgorithm::db_count_square_in_interface_graph_latest(Graph* interface_graph){

    long long exact_square_in_interface_graph = count_interface_edge_square_latest(interface_graph);
    
    return exact_square_in_interface_graph;
}

void CountingAlgorithm::db_count_square_in_whole_ptn_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphFromFile(file_path, vertex_partition_file_path, ptn_idx);

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, ptn_idx);


        Graph* interface_augmented_graph = new Graph();
        interface_graph->transformToAugmentedGraph(interface_augmented_graph);

        

        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        local_interface_square_count = db_count_square_in_interface_graph(interface_graph);

        //std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        // std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        for(int u_ptn_idx = ptn_idx + 1; u_ptn_idx < partition_cnt; u_ptn_idx++){
            deductible_count += bfy_count_in_two_partition(local_graph, ptn_idx, u_ptn_idx);
        }

        local_graph->buildPartitionWiseNbrCntArray(partition_cnt, ptn_idx);
        three_ptn_deductible_count += bfy_count_in_three_partition(local_graph, partition_cnt, ptn_idx);

        global_square_count += local_square_count;
        global_square_count += local_interface_square_count;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    
    global_square_count += cut_graph_square_count;

    global_square_count -= (2 * deductible_count);
    global_square_count -= three_ptn_deductible_count;

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


void CountingAlgorithm::optimized_db_count_square_in_whole_ptn_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, ptn_idx);

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, ptn_idx); 

        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = db_count_square_in_interface_graph(interface_graph);

        std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        global_square_count += local_square_count;
        global_square_count += local_cut_edge_square_count;
        global_square_count += local_interface_square_count;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    
    global_square_count += cut_graph_square_count;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;
}


void CountingAlgorithm::db_count_square_with_interface_graph_optimization(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, ptn_idx);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphOptimized(file_path, vertex_partition_file_path, ptn_idx); 

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);        

        clock_t counting_begin_clock = clock();
        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = db_count_square_in_interface_graph_optimized(interface_graph);
        double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

        std::cout << "===================================================================================" << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Counting Time - " << counting_time << std::endl;
        std::cout << "===================================================================================" << std::endl;

        global_square_count += local_square_count;
        global_square_count += local_cut_edge_square_count;
        global_square_count += local_interface_square_count;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    clock_t counting_begin_clock = clock();
    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    std::cout << "Cut Graph : Counting Time - " << counting_time << std::endl;
    
    global_square_count += cut_graph_square_count;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;
}


void CountingAlgorithm::db_count_square_with_interface_graph_optimization_latest(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, ptn_idx);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphLatest(file_path, vertex_partition_file_path, ptn_idx); 

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);        

        clock_t counting_begin_clock = clock();
        
        clock_t local_count_begin_clock = clock();
        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        double local_graph_counting_time = (double(clock() - local_count_begin_clock)) / CLOCKS_PER_SEC;

        clock_t local_cut_count_begin_clock = clock();
        local_cut_edge_square_count = count_square_from_other_ptn_per_vertex(local_graph);
        double local_cut_graph_counting_time = (double(clock() - local_cut_count_begin_clock)) / CLOCKS_PER_SEC;

        clock_t ifc_count_begin_clock = clock();
        local_interface_square_count = db_count_square_in_interface_graph_latest(interface_graph);
        double ifc_graph_counting_time = (double(clock() - ifc_count_begin_clock)) / CLOCKS_PER_SEC;


        double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;
        

        std::cout << "===================================================================================" << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Graph Counting Time - " << local_graph_counting_time << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Graph Counting Time - " << local_cut_graph_counting_time << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Interface Graph Counting Time - " << ifc_graph_counting_time << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Counting Time - " << counting_time << std::endl;
        std::cout << "===================================================================================" << std::endl;

        global_square_count += local_square_count;
        global_square_count += local_cut_edge_square_count;
        global_square_count += local_interface_square_count;

        local_graph->deleteAndClear();
        interface_graph->deleteAndClear();
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    clock_t counting_begin_clock = clock();
    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

    cut_graph->deleteAndClear();

    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    std::cout << "Cut Graph : Counting Time - " << counting_time << std::endl;
    
    global_square_count += cut_graph_square_count;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;
}




void CountingAlgorithm::db_count_square_with_interface_graph_optimization_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFileKahip(file_path, vertex_partition_file_path, ptn_idx);

        std::cout << "Local Loading Finished" << std::endl;

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphOptimizedKahip(file_path, vertex_partition_file_path, ptn_idx);

        std::cout << "Interface Graph Loading Finished" << std::endl; 

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);        

        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = db_count_square_in_interface_graph_optimized(interface_graph);

        std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        global_square_count += local_square_count;
        global_square_count += local_cut_edge_square_count;
        global_square_count += local_interface_square_count;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFileKahip(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    
    global_square_count += cut_graph_square_count;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;
}


void CountingAlgorithm::db_count_square_with_interface_graph_optimization_kahip_latest(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFileKahip(file_path, vertex_partition_file_path, ptn_idx);

        std::cout << "Local Loading Finished" << std::endl;

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphOptimizedKahipLatest(file_path, vertex_partition_file_path, ptn_idx);

        std::cout << "Interface Graph Loading Finished" << std::endl; 

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);        

        clock_t counting_begin_clock = clock();
        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = db_count_square_in_interface_graph_latest(interface_graph);
        double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

        std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Counting Time - " << counting_time << std::endl;

        global_square_count += local_square_count;
        global_square_count += local_cut_edge_square_count;
        global_square_count += local_interface_square_count;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphFromFileKahip(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    clock_t counting_begin_clock = clock();
    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;
    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    std::cout << "Cut Graph : Counting Time - " << counting_time << std::endl;
    
    global_square_count += cut_graph_square_count;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;
}



void CountingAlgorithm::optimized_db_count_square_in_bidirectional_graph_seq(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesBidirection(file_path, vertex_partition_file_path, ptn_idx);

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphBidirection(file_path, vertex_partition_file_path, ptn_idx); 

        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = db_count_square_in_interface_graph(interface_graph);

        std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        global_square_count += local_square_count;
        global_square_count += local_cut_edge_square_count;
        global_square_count += local_interface_square_count;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadCutGraphBidirection(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    
    global_square_count += cut_graph_square_count;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;
}


void CountingAlgorithm::db_count_square_in_knkr(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;
    long long deductible_count = 0, three_ptn_deductible_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadKroneckerLocalGraphWoCutEdgesBidirection(file_path, vertex_partition_file_path, ptn_idx);

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);

        Graph* interface_graph = new Graph();
        interface_graph->loadKroneckerInterfaceGraphBidirectionOptimized(file_path, vertex_partition_file_path, ptn_idx); 

        clock_t counting_begin_clock_1 = clock();  
        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        double counting_time_1 = (double(clock() - counting_begin_clock_1)) / CLOCKS_PER_SEC;
        std::cout << "Partition - " << ptn_idx << " : Counting Time (1)- " << counting_time_1 << std::endl;
        clock_t counting_begin_clock_2 = clock();
        local_cut_edge_square_count = count_square_from_other_ptn_per_vertex(local_graph);
        double counting_time_2 = (double(clock() - counting_begin_clock_2)) / CLOCKS_PER_SEC;
        std::cout << "Partition - " << ptn_idx << " : Counting Time (2)- " << counting_time_2 << std::endl;
        clock_t counting_begin_clock_3 = clock(); 
        local_interface_square_count = db_count_square_in_interface_graph_optimized(interface_graph);
        double counting_time_3 = (double(clock() - counting_begin_clock_3)) / CLOCKS_PER_SEC;
        std::cout << "Partition - " << ptn_idx << " : Counting Time (3)- " << counting_time_3 << std::endl;
        

        std::cout << "===================================================================================" << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;        
        std::cout << "===================================================================================" << std::endl;

        global_square_count += local_square_count;
        global_square_count += local_cut_edge_square_count;
        global_square_count += local_interface_square_count;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadKroneckerCutGraphBidirection(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    clock_t counting_begin_clock = clock(); 
    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    std::cout << "Cut Graph : Counting Time - " << counting_time << std::endl;
    
    global_square_count += cut_graph_square_count;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;
}

void CountingAlgorithm::db_count_square_in_knkr_kahip(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){

    long long global_square_count = 0, local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;

    for (int ptn_idx = 0; ptn_idx < partition_cnt; ptn_idx++){

        Graph* local_graph = new Graph();
        local_graph->loadKroneckerLocalGraphWoCutEdgesBidirectionKahip(file_path, vertex_partition_file_path, ptn_idx);

        std::cout << "Local Loading Finished" << std::endl;

        Graph* interface_graph = new Graph();
        interface_graph->loadKroneckerInterfaceGraphBidirectionOptimizedKahip(file_path, vertex_partition_file_path, ptn_idx);

        std::cout << "Interface Graph Loading Finished" << std::endl; 

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);        

        local_square_count = db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = db_count_square_in_interface_graph_optimized(interface_graph);

        std::cout << "Partition - " << ptn_idx << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << ptn_idx << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        global_square_count += local_square_count;
        global_square_count += local_cut_edge_square_count;
        global_square_count += local_interface_square_count;
    }

    Graph* cut_graph = new Graph();
    cut_graph->loadKroneckerCutGraphBidirectionKahip(file_path, vertex_partition_file_path);

    Graph* transformed_cut_graph = new Graph();
    cut_graph->transformToAugmentedGraph(transformed_cut_graph);

    cut_graph_square_count = db_count_square_in_cut_graph(transformed_cut_graph);
    std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
    
    global_square_count += cut_graph_square_count;

    std::cout << "==============================================" << std::endl;
    std::cout << "Global Square Count : " << global_square_count << std::endl;
    std::cout << "==============================================" << std::endl;
}
