#include "countingalgo.h"
#include "distributioncoordinator.hpp"
#include <fstream>
#include <mpi.h>


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

                //std::cout << "v1 : " << v1 << " v2 : " << v2 << " v3 : " << v3 << std::endl;
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
