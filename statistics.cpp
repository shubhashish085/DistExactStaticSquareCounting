#include "statistics.h"
#include <fstream>


void Stats::printStatsOfPartitionedGraph (const std::string& file_path, const std::string& vertex_partition_file_path, int partition_count){


    VertexID vertex_id, begin, end;
    NodeID partition_id;
    bool contains_in_partition = false;

    std::ifstream vertex_partition_file(vertex_partition_file_path);
    std::ifstream vtx_ptn_file(vertex_partition_file_path);
    std::ifstream infile(file_path);

    if (!vertex_partition_file.is_open() || !infile.is_open()){
        std::cout << "Can not open the graph file " << vertex_partition_file_path << " or " << file_path << "." << std::endl;
        exit(-1);
    }

    ui total_vertices_count = 0;

    while (vertex_partition_file >> vertex_id){

        vertex_partition_file >> partition_id;
        total_vertices_count++;
    }

    vertex_partition_file.close();

    NodeID* partition = new NodeID[total_vertices_count];
    ui* vtx_cnt_array = new ui[partition_count], *edge_cnt_array = new ui[partition_count], *cut_edge_cnt_array = new ui[partition_count];

    std::fill(vtx_cnt_array, vtx_cnt_array + partition_count, 0);
    std::fill(edge_cnt_array, edge_cnt_array + partition_count, 0);
    std::fill(cut_edge_cnt_array, cut_edge_cnt_array + partition_count, 0);

    while (vtx_ptn_file >> vertex_id)
    {
        vtx_ptn_file >> partition_id;
        partition[vertex_id] = partition_id;
        vtx_cnt_array[partition_id]++;
    }

    vtx_ptn_file.close();    


    std::string input_line;
    ui line_count = 0, count = 0, comment_line_count = 4;

    while (std::getline(infile, input_line)){

        if (input_line.rfind("#", 0) == 0){
            line_count++;
        }

        if (line_count >= comment_line_count){
            break;
        }
    }

    NodeID ptn_begin, ptn_end;

    while (infile >> begin){

        infile >> end;
        contains_in_partition = false;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count))
        {
            ptn_begin = partition[begin];
            ptn_end = partition[end];

            if(ptn_begin == ptn_end){
                edge_cnt_array[ptn_begin]++;

            }else {
                cut_edge_cnt_array[ptn_begin]++;
                cut_edge_cnt_array[ptn_end]++;
            }
        }
    }

    infile.close();

    ui max_val, min_val, total_val = 0;
    double avg_val;

    min_val = *std::min_element(vtx_cnt_array, vtx_cnt_array + partition_count);
    max_val = *std::max_element(vtx_cnt_array, vtx_cnt_array + partition_count);

    for(ui i = 0; i < partition_count; i++){
        total_val += vtx_cnt_array[i];
    }

    avg_val = ((double) total_val)/ partition_count;

    std::cout << "Vertex Count Stats : Min - " << min_val << " Max - " << max_val << " Avg - " << avg_val << std::endl;

    min_val = *std::min_element(edge_cnt_array, edge_cnt_array + partition_count);
    max_val = *std::max_element(edge_cnt_array, edge_cnt_array + partition_count);

    total_val = 0;

    for(ui i = 0; i < partition_count; i++){
        total_val += edge_cnt_array[i];
    }

    avg_val = ((double) total_val)/ partition_count;

    std::cout << "Edge Count Stats : Min - " << min_val << " Max - " << max_val << " Avg - " << avg_val << std::endl;

    min_val = *std::min_element(cut_edge_cnt_array, cut_edge_cnt_array + partition_count);
    max_val = *std::max_element(cut_edge_cnt_array, cut_edge_cnt_array + partition_count);

    total_val = 0;

    for(ui i = 0; i < partition_count; i++){
        total_val += cut_edge_cnt_array[i];
    }

    avg_val = ((double) total_val)/ (2 * partition_count);
    std::cout << "Cut Edge Stats : Min - " << min_val << " Max - " << max_val << " Avg - " << avg_val << std::endl;

    std::cout << "--------------------------------------------------------" << std::endl;

    delete[] vtx_cnt_array;
    delete[] edge_cnt_array;
    delete[] cut_edge_cnt_array;

}


void Stats::writeStatsToFile (const std::string& file_path, const std::string& vertex_partition_file_path, const std::string& output_file_path, int partition_count){
    
}

