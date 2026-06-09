

#include "graph.h"
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>

void Graph::loadGraphFromFile(const std::string &file_path)
{

    std::cout << "############# Loading Graph With Edges ###############" << std::endl;

    std::ifstream infile(file_path);

    if (!infile.is_open())
    {
        std::cout << "Can not open the graph file " << file_path << " ." << std::endl;
        exit(-1);
    }

    char type;
    std::string input_line;
    ui label = 0;

    std::cout << "Reading File............ " << std::endl;

    ui line_count = 0, count = 0, comment_line_count = 4;

    while (std::getline(infile, input_line))
    {

        if (input_line.rfind("#", 0) == 0)
        {

            line_count++;

            if (input_line.rfind("# Nodes", 0) == 0)
            {

                std::stringstream ss(input_line);
                std::string token;
                int count = 0;

                while (!ss.eof())
                {

                    std::getline(ss, token, ' ');

                    if (!(token.rfind("#", 0) == 0 || token.rfind("Nodes:", 0) == 0 || token.rfind("Edges:", 0) == 0))
                    {

                        if (count == 0)
                        {

                            vertices_count = stoi(token);
                            std::cout << "Vertex Count : " << vertices_count << std::endl;
                            degrees = new ui[vertices_count];
                            std::fill(degrees, degrees + vertices_count, 0);
                            count = 1;
                        }
                        else
                        {
                            edges_count = stoi(token);
                            count = 0;
                        }
                        std::cout << "Vertices Count : " << vertices_count << " Edges Count : " << edges_count
                                  << std::endl;
                    }
                }
            }
        }

        if (line_count >= comment_line_count)
        {
            break;
        }
    }

    VertexID begin, end;

    while (infile >> begin)
    {

        infile >> end;

        if (begin != end && begin < vertices_count && end < vertices_count)
        {
            degrees[begin] += 1;
            degrees[end] += 1;
        }
    }

    infile.close();

    std::ifstream input_file(file_path);

    offsets = new ui[vertices_count + 1];
    offsets[0] = 0;

    neighbors = new VertexID[edges_count * 2];
    max_degree = 0;

    std::cout << "Initialization Finished" << std::endl;

    LabelID max_label_id = 0, begin_vtx_label, end_vtx_label;
    std::vector<ui> neighbors_offset(vertices_count, 0); // used for adjust neighbors with offset

    for (ui id = 0; id < vertices_count; id++)
    {
        offsets[id + 1] = offsets[id] + degrees[id];

        if (degrees[id] > max_degree)
        {
            max_degree = degrees[id];
        }
    }

    line_count = 0;

    while (std::getline(input_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count)
        {
            break;
        }
    }

    while (input_file >> begin)
    {

        input_file >> end;

        line_count++;
        if (begin >= vertices_count || end >= vertices_count || begin == end)
        {
            continue;
        }

        ui offset = offsets[begin] + neighbors_offset[begin];
        neighbors[offset] = end;

        offset = offsets[end] + neighbors_offset[end];
        neighbors[offset] = begin;

        neighbors_offset[begin] += 1;
        neighbors_offset[end] += 1;
    }

    input_file.close();

    for (ui i = 0; i < vertices_count; ++i)
    {
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }
}

void Graph::loadGraphFromFileForBothDirectionEdges(const std::string &file_path)
{
    std::ifstream infile(file_path);

    if (!infile.is_open())
    {
        std::cout << "Can not open the graph file " << file_path << " ." << std::endl;
        exit(-1);
    }

    char type;
    std::string input_line;
    ui label = 0;

    std::cout << "Reading File............ " << std::endl;

    ui line_count = 0, count = 0, comment_line_count = 4;

    while (std::getline(infile, input_line))
    {

        if (input_line.rfind("#", 0) == 0)
        {
            line_count++;

            if (input_line.rfind("# Nodes", 0) == 0)
            {

                std::stringstream ss(input_line);
                std::string token;
                int count = 0;

                while (!ss.eof())
                {
                    std::getline(ss, token, ' ');

                    if (!(token.rfind("#", 0) == 0 || token.rfind("Nodes:", 0) == 0 || token.rfind("Edges:", 0) == 0))
                    {

                        if (count == 0)
                        {

                            vertices_count = stoi(token);
                            std::cout << "Vertex Count : " << vertices_count << std::endl;
                            degrees = new ui[vertices_count];
                            std::fill(degrees, degrees + vertices_count, 0);
                            count = 1;
                        }
                        else
                        {
                            count = 0;
                        }
                        std::cout << "Vertices Count : " << vertices_count << " Edges Count : " << edges_count << std::endl;
                    }
                }
            }
        }

        if (line_count >= comment_line_count){
            break;
        }
    }

    VertexID begin, end;

    edges_count = 0;

    while (infile >> begin)
    {
        infile >> end;

        if (begin != end && begin < vertices_count && end < vertices_count)
        {
            degrees[begin] += 1;
            edges_count++;
        }
    }

    infile.close();

    std::ifstream input_file(file_path);

    offsets = new ui[vertices_count + 1];
    offsets[0] = 0;

    neighbors = new VertexID[edges_count];

    std::cout << "Initialization Finished" << std::endl;

    LabelID max_label_id = 0, begin_vtx_label, end_vtx_label;
    std::vector<ui> neighbors_offset(vertices_count, 0); // used for adjust neighbors with offset

    for (ui id = 0; id < vertices_count; id++)
    {
        offsets[id + 1] = offsets[id] + degrees[id];
    }

    line_count = 0;

    while (std::getline(input_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count)
        {
            break;
        }
    }

    while (input_file >> begin)
    {
        input_file >> end;

        line_count++;
        if (begin >= vertices_count || end >= vertices_count || begin == end)
        {
            continue;
        }

        ui offset = offsets[begin] + neighbors_offset[begin];
        neighbors[offset] = end;

        neighbors_offset[begin] += 1;
    }

    input_file.close();

    for (ui i = 0; i < vertices_count; ++i)
    {
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }
}



void Graph::loadPartitionedGraphFromFile( const std::string &file_path, const std::string &vertex_partition_file_path, int partition_no)
{

    VertexID vertex_id, begin, end;
    int partition_id;
    bool contains_in_partition = false;

    std::ifstream vertex_partition_file(vertex_partition_file_path);
    std::ifstream vtx_ptn_file(vertex_partition_file_path);
    std::ifstream infile(file_path);

    if (!vertex_partition_file.is_open() || !infile.is_open())
    {
        std::cout << "Can not open the graph file " << vertex_partition_file_path << " or " << file_path << "." << std::endl;
        exit(-1);
    }

    ui total_vertices_count = 0;

    while (vertex_partition_file >> vertex_id)
    {

        vertex_partition_file >> partition_id;

        if (partition_id == partition_no)
        {
            vertices.push_back(vertex_id);
            vertex_idx_map[vertex_id] = vertices_count;
            vertices_count++;
        }

        total_vertices_count++;
    }

    vertex_partition_file.close();

    partition = new NodeID[total_vertices_count];

    while (vtx_ptn_file >> vertex_id)
    {
        vtx_ptn_file >> partition_id;
        partition[vertex_id] = partition_id;
    }

    vtx_ptn_file.close();

    degrees = new ui[vertices_count];
    ghost_vertices_count = 0;
    std::fill(degrees, degrees + vertices_count, 0);

    char type;
    std::string input_line;
    ui label = 0, offset;

    std::cout << "Reading File............ " << std::endl;

    ui line_count = 0, count = 0, comment_line_count = 4;

    while (std::getline(infile, input_line))
    {

        if (input_line.rfind("#", 0) == 0)
        {
            line_count++;
        }

        if (line_count >= comment_line_count)
        {
            break;
        }
    }

    ui ghost_edges_count = 0;

    while (infile >> begin)
    {

        infile >> end;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count)
        {
            if (partition[begin] != partition_no && partition[end] == partition_no){

                ghost_edges_count++;

                if (ghost_vertex_map.find(begin) != ghost_vertex_map.end()) {
                    ghost_vertex_map[begin] += 1;
                } else {
                    ghost_vertex_map[begin] = 1;
                    ghost_vertices_count += 1;
                }
            } else if (partition[begin] == partition_no && partition[end] != partition_no){

                ghost_edges_count++;

                if (ghost_vertex_map.find(end) != ghost_vertex_map.end()) {
                    ghost_vertex_map[end] += 1;
                }
                else{
                    ghost_vertex_map[end] = 1;
                    ghost_vertices_count += 1;
                }
            }

            if (vertex_idx_map.find(begin) != vertex_idx_map.end())
            {
                degrees[vertex_idx_map[begin]] += 1;
                contains_in_partition = true;
            }

            if (vertex_idx_map.find(end) != vertex_idx_map.end())
            {
                degrees[vertex_idx_map[end]] += 1;
                contains_in_partition = true;
            }

            if (contains_in_partition)
            {
                edges_count++;
            }

            contains_in_partition = false;
        }
    }

    infile.close();

    std::ifstream input_file(file_path);

    offsets = new ui[vertices_count + 1];
    g_offsets = new ui[ghost_vertices_count + 1];
    offsets[0] = 0;
    g_offsets[0] = 0;

    neighbors = new VertexID[edges_count * 2];
    g_neighbors = new VertexID[ghost_edges_count];
    max_degree = 0;

    std::vector<ui> neighbors_offset(vertices_count, 0); // used for adjust neighbors with offset
    std::vector<ui> g_neighbors_offset(ghost_vertices_count, 0);
    VertexID id;

    for (VertexID i = 0; i < vertices_count; i++){
        /*id = vertices[i];
        offsets[id + 1] = offsets[id] + degrees[id];*/
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    VertexID i = 0;
    for (const auto &[key, value] : ghost_vertex_map)
    {
        ghost_vertex_idx_map[key] = i;
        g_offsets[i + 1] = g_offsets[i] + value;
        i++;
    }

    line_count = 0;

    while (std::getline(input_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count)
        {
            break;
        }
    }

    VertexID ghost_idx;
    ui g_offset;
    ui begin_idx, end_idx;

    while (input_file >> begin)
    {
        input_file >> end;

        if((begin == end) || (begin >= total_vertices_count) || (end >= total_vertices_count)){
            continue;
        }


        if (vertex_idx_map.find(begin) != vertex_idx_map.end()){
            begin_idx = vertex_idx_map[begin];
            offset = offsets[begin_idx] + neighbors_offset[begin_idx];
            neighbors[offset] = end;
            neighbors_offset[begin] += 1;
        }

        if (vertex_idx_map.find(end) != vertex_idx_map.end()){
            end_idx = vertex_idx_map[end];
            offset = offsets[end_idx] + neighbors_offset[end_idx];
            neighbors[offset] = begin;
            neighbors_offset[end] += 1;
        }

        if (partition[begin] != partition_no && partition[end] == partition_no){
            ghost_idx = ghost_vertex_idx_map[begin];
            g_offset = g_offsets[ghost_idx] + g_neighbors_offset[ghost_idx];
            g_neighbors[g_offset] = end;
            g_neighbors_offset[ghost_idx] += 1;
        }

        if (partition[begin] == partition_no && partition[end] != partition_no){
            ghost_idx = ghost_vertex_idx_map[end];
            g_offset = g_offsets[ghost_idx] + g_neighbors_offset[ghost_idx];
            g_neighbors[g_offset] = begin;
            g_neighbors_offset[ghost_idx] += 1;
        }
    }

    input_file.close();

    for (ui i = 0; i < vertices_count; ++i)
    {
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }

    for (ui i = 0; i < ghost_vertices_count; i++)
    {
        std::sort(g_neighbors + g_offsets[i], g_neighbors + g_offsets[i + 1]);
    }
}


void Graph::loadPartitionedLocalGraphFromFile(const std::string &file_path, const std::string &vertex_partition_file_path, int partition_no)
{

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

    partition = new NodeID[total_vertices_count];

    while (vtx_ptn_file >> vertex_id)
    {
        vtx_ptn_file >> partition_id;
        partition[vertex_id] = partition_id;
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

    edges_count = 0;

    while (infile >> begin){

        infile >> end;
        contains_in_partition = false;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count)
        {
            if (partition[begin] == partition_no || partition[end] == partition_no){

                contains_in_partition = true;

                if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
                    vertex_idx_map[vertex_id] = vertices_count;
                    vertices_count++;
                }

                if(vertex_idx_map.find(end) == vertex_idx_map.end()){
                    vertex_idx_map[vertex_id] = vertices_count;
                    vertices_count++;
                }

                if(contains_in_partition){
                    edges_count++;
                }                
            }
        }
    }

    infile.close();

    ui offset;

    degrees = new ui[vertices_count];
    std::fill(degrees, degrees + vertices_count, 0);
    offsets = new ui[vertices_count + 1];

    line_count = 0;

    std::ifstream input_file(file_path);

    while (std::getline(input_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count)
        {
            break;
        }
    }

    ui begin_idx, end_idx;

    while (input_file >> begin)
    {
        input_file >> end;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count){
            if (partition[begin] == partition_no || partition[end] == partition_no){
                degrees[vertex_idx_map[begin]] += 1;
                degrees[vertex_idx_map[end]] += 1;                
            }
        }
    }

    input_file.close();

    offsets[0] = 0;

    neighbors = new VertexID[edges_count * 2];
    std::vector<ui> neighbors_offset(vertices_count, 0);

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    line_count = 0;

    std::ifstream in_file(file_path);

    while (std::getline(in_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count){
            break;
        }
    }

    VertexID begin_idx, end_idx;

    while (in_file >> begin)
    {
        in_file >> end;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count){
            if (partition[begin] == partition_no || partition[end] == partition_no){

                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];

                offset = offsets[begin_idx] + neighbors_offset[begin_idx];
                neighbors[offset] = end_idx;
                neighbors_offset[begin_idx] += 1;

                offset = offsets[end_idx] + neighbors_offset[end_idx];
                neighbors[offset] = begin_idx;
                neighbors_offset[end_idx] += 1;                                    
            }
        }
    }

    in_file.close();

    for (ui i = 0; i < vertices_count; ++i)
    {
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }
}


void Graph::loadPartitionedInterfaceGraphFromFile(const std::string& file_path, const std::string& vtx_ptn_file_path, int partition_no){

    NodeID partition_id;
    VertexID vertex_id;
    bool contains_in_partition = false;

    std::ifstream vertex_partition_file(vtx_ptn_file_path);
    std::ifstream infile(file_path);

    if (!vertex_partition_file.is_open() || !infile.is_open())
    {
        std::cout << "Can not open the graph file " << vtx_ptn_file_path << " or " << file_path << "." << std::endl;
        exit(-1);
    }

    ui total_vertices_count = 0;

    while (vertex_partition_file >> vertex_id){

        vertex_partition_file >> partition_id;
        total_vertices_count++;
    }

    vertex_partition_file.close();

    std::ifstream vtx_ptn_file(vtx_ptn_file_path);

    partition = new NodeID[total_vertices_count];

    while (vtx_ptn_file >> vertex_id)
    {
        vtx_ptn_file >> partition_id;
        partition[vertex_id] = partition_id;
    }

    vtx_ptn_file.close();

    ui line_count = 0, comment_line_count = 4;
    VertexID begin, end;
    std::string input_line;

    while (std::getline(infile, input_line)){

        if (input_line.rfind("#", 0) == 0){
            line_count++;
        }

        if (line_count >= comment_line_count){
            break;
        }
    }

    vertices_count = 0;
    vertex_idx_map.clear();

    while (infile >> begin){

        infile >> end;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count){
            if(partition[begin] != partition[end]){
                
                if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
                    vertex_idx_map[begin] = vertices_count;
                    vertices_count++;
                }

                if(vertex_idx_map.find(end) == vertex_idx_map.end()){
                    vertex_idx_map[end] = vertices_count;
                    vertices_count++;
                }                
            }
        }
    }

    infile.close();

    std::ifstream input_file(file_path);

    degrees = new ui[vertices_count];
    offsets = new ui[vertices_count + 1];
    std::fill(degrees, degrees + vertices_count, 0);

    ui  offset;
    line_count = 0, comment_line_count = 4;

    while (std::getline(input_file, input_line)){

        if (input_line.rfind("#", 0) == 0){
            line_count++;
        }

        if (line_count >= comment_line_count){
            break;
        }
    }

    edges_count = 0;

    VertexID begin_idx, end_idx;

    while (input_file >> begin){

        input_file >> end;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){
                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end]; 
                degrees[begin_idx] += 1;
                degrees[end_idx] += 1;
                edges_count++;
            }
        }
    }

    input_file.close();

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    std::ifstream in_file(file_path);

    line_count = 0, comment_line_count = 4;

    offsets[0] = 0;
    neighbors = new VertexID[edges_count * 2];

    std::vector<ui> neighbors_offset(vertices_count, 0);

    while (std::getline(in_file, input_line)){

        if (input_line.rfind("#", 0) == 0){
            line_count++;
        }

        if (line_count >= comment_line_count){
            break;
        }
    }    

    while (in_file >> begin){

        in_file >> end;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){

                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }

                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];        

                offset = offsets[begin_idx] + neighbors_offset[begin_idx];
                neighbors[offset] = end_idx;
                neighbors_offset[begin_idx] += 1;

                offset = offsets[end_idx] + neighbors_offset[end_idx];
                neighbors[offset] = begin_idx;
                neighbors_offset[end_idx] += 1;
            }
        }    
    }

    in_file.close();
    
    for (ui i = 0; i < vertices_count; ++i){
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }    
}

void Graph::loadCutGraphFromFile(const std::string& file_path, const std::string& vtx_ptn_file_path)
{

    NodeID partition_id;
    VertexID vertex_id;
    bool contains_in_partition = false;

    std::ifstream vertex_partition_file(vtx_ptn_file_path);
    std::ifstream infile(file_path);

    if (!vertex_partition_file.is_open() || !infile.is_open())
    {
        std::cout << "Can not open the graph file " << vtx_ptn_file_path << " or " << file_path << "." << std::endl;
        exit(-1);
    }

    ui total_vertices_count = 0;

    while (vertex_partition_file >> vertex_id){

        vertex_partition_file >> partition_id;
        total_vertices_count++;
    }

    vertex_partition_file.close();

    std::ifstream vtx_ptn_file(vtx_ptn_file_path);

    partition = new NodeID[total_vertices_count];

    while (vtx_ptn_file >> vertex_id)
    {
        vtx_ptn_file >> partition_id;
        partition[vertex_id] = partition_id;
    }

    vtx_ptn_file.close();

    ui line_count = 0, comment_line_count = 4;
    VertexID begin, end;
    std::string input_line;

    while (std::getline(infile, input_line)){

        if (input_line.rfind("#", 0) == 0){
            line_count++;
        }

        if (line_count >= comment_line_count){
            break;
        }
    }

    vertices_count = 0;

    while (infile >> begin){

        infile >> end;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count){
            if(partition[begin] != partition[end]){
                
                if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
                    vertex_idx_map[begin] = vertices_count;
                    vertices_count++;
                }

                if(vertex_idx_map.find(end) == vertex_idx_map.end()){
                    vertex_idx_map[end] = vertices_count;
                    vertices_count++;
                }                
            }
        }
    }

    infile.close();

    std::ifstream input_file(file_path);

    degrees = new ui[vertices_count];
    offsets = new ui[vertices_count + 1];
    std::fill(degrees, degrees + vertices_count, 0);

    ui  offset;
    line_count = 0, comment_line_count = 4;

    while (std::getline(input_file, input_line)){

        if (input_line.rfind("#", 0) == 0){
            line_count++;
        }

        if (line_count >= comment_line_count){
            break;
        }
    }

    edges_count = 0;

    std::vector<std::pair<VertexID, VertexID>> cut_edge_vtr;
    VertexID begin_idx, end_idx;

    while (input_file >> begin){

        input_file >> end;

        if (begin != end && begin < total_vertices_count && end < total_vertices_count){
            if(partition[begin] != partition[end]){
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end]; 
                degrees[begin_idx] += 1;
                degrees[end_idx] += 1;
                cut_edge_vtr.push_back(std::make_pair(begin_idx, end_idx));
                edges_count++;
            }
        }
    }

    input_file.close();
    
    offsets[0] = 0;
    neighbors = new VertexID[edges_count * 2];

    std::vector<ui> neighbors_offset(vertices_count, 0);

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    for(ui i = 0; i < cut_edge_vtr.size(); i++){

        begin = cut_edge_vtr[i].first;
        end = cut_edge_vtr[i].second;

        offset = offsets[begin] + neighbors_offset[begin];
        neighbors[offset] = end;

        neighbors_offset[begin] += 1;

        offset = offsets[end] + neighbors_offset[end];
        neighbors[offset] = begin;

        neighbors_offset[end] += 1;
    }

    for (ui i = 0; i < vertices_count; ++i){
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }
}


void Graph::loadCutGraphFromCutEdgeFile(const std::string& file_path, const std::string& cut_edge_file_path)
{

    NodeID partition_id;
    VertexID vertex_id;
    bool contains_in_partition = false;

    std::ifstream cut_edge_file(cut_edge_file_path);

    if (!cut_edge_file.is_open())
    {
        std::cout << "Can not open the graph file " << cut_edge_file_path << "." << std::endl;
        exit(-1);
    }

    ui line_count = 0, comment_line_count = 4;
    VertexID begin, end;
    std::string input_line;
    VertexID begin_idx, end_idx;
    std::vector<std::pair<VertexID, VertexID>> cut_edge_vtr;

    vertices_count = 0;
    edges_count = 0;

    while (cut_edge_file >> begin){
        cut_edge_file >> end;    
        
        if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
            vertex_idx_map[begin] = vertices_count;
            vertices_count++;
        }

        if(vertex_idx_map.find(end) == vertex_idx_map.end()){
            vertex_idx_map[end] = vertices_count;
            vertices_count++;
        }

        begin_idx = vertex_idx_map[begin];
        end_idx = vertex_idx_map[end];
        cut_edge_vtr.push_back(std::make_pair(begin_idx, end_idx));
        edges_count++;        
    }

    cut_edge_file.close();

    degrees = new ui[vertices_count];
    offsets = new ui[vertices_count + 1];
    std::fill(degrees, degrees + vertices_count, 0);

    std::ifstream input_file(cut_edge_file_path);    

    ui  offset;    

    while (input_file >> begin){

        input_file >> end;

        degrees[vertex_idx_map[begin]] += 1;
        degrees[vertex_idx_map[end]] += 1;
    }

    input_file.close();
    
    offsets[0] = 0;
    neighbors = new VertexID[edges_count * 2];

    std::vector<ui> neighbors_offset(vertices_count, 0);

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    line_count = 0;

    for(ui i = 0; i < cut_edge_vtr.size(); i++){

        begin = cut_edge_vtr[i].first;
        end = cut_edge_vtr[i].second;

        offset = offsets[begin] + neighbors_offset[begin];
        neighbors[offset] = end;

        neighbors_offset[begin] += 1;

        offset = offsets[end] + neighbors_offset[end];
        neighbors[offset] = begin;

        neighbors_offset[end] += 1;
    }

    for (ui i = 0; i < vertices_count; ++i){
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }
}


bool Graph::is_smaller(VertexID u, VertexID v){

    if(u == v){
        return false;
    }

    if(main_degrees[u] < main_degrees[v]){
        return true;
    }

    if((main_degrees[u] == main_degrees[v]) && u < v){
        return true;
    }

    return false; 

}


void Graph::transformToAugmentedGraph(Graph *augmented_graph){

    augmented_graph->vertices_count = vertices_count;
    augmented_graph->edges_count = edges_count;
    augmented_graph->offsets = new ui[vertices_count + 1];
    augmented_graph->neighbors = new VertexID[edges_count];

    augmented_graph->offsets[0] = 0;

    augmented_graph->main_degrees = new ui[augmented_graph->vertices_count];
    augmented_graph->degrees = new ui[augmented_graph->vertices_count];
    std::fill(augmented_graph->degrees, augmented_graph->degrees + augmented_graph->vertices_count, 0);

    for (ui i = 0; i < vertices_count; i++)
    {
        for (ui j = offsets[i]; j < offsets[i + 1]; j++)
        {
            if ((degrees[neighbors[j]] > degrees[i]) || ((degrees[neighbors[j]] == degrees[i]) && neighbors[j] > i))
            {
                augmented_graph->degrees[i] += 1;
            }
        }
    }

    for (ui i = 0; i < vertices_count; i++)
    {
        augmented_graph->offsets[i + 1] = augmented_graph->offsets[i] + augmented_graph->degrees[i];
        augmented_graph->main_degrees[i] = degrees[i];
    }

    std::vector<ui> neighbors_offset(vertices_count, 0);
    ui offset;

    for (ui i = 0; i < vertices_count; i++)
    {
        for (ui j = offsets[i]; j < offsets[i + 1]; j++)
        {

            if ((degrees[neighbors[j]] > degrees[i]) || ((degrees[neighbors[j]] == degrees[i]) && neighbors[j] > i))
            {

                offset = augmented_graph->offsets[i] + neighbors_offset[i];
                augmented_graph->neighbors[offset] = neighbors[j];
                neighbors_offset[i] += 1;
            }
        }
    }

    std::cout << "Transformation Finished ...... " << std::endl;    
}


void Graph::printGraphMetaData()
{
    std::cout << "|V|: " << vertices_count << ", |E|: " << edges_count << std::endl;
    std::cout << "Max Degree: " << max_degree << ", Max Label Frequency: " << std::endl;
}

long long Graph::sequential_count_exact_square()
{
    long long exact_count = 0;
    VertexID *nbrs_1, *nbrs_2, *nbrs_3, *nbrs_4;
    ui nbrs_1_cnt = 0, nbrs_2_cnt = 0, nbrs_3_cnt = 0, nbrs_4_cnt = 0;
    VertexID v2, v3, v4;

    // 1243 - Square
    long long count_1 = 0;

    for (VertexID v1 = 0; v1 < vertices_count; v1++)
    {
        nbrs_1 = getVertexNeighbors(v1, nbrs_1_cnt);
        for (VertexID j = 0; j < nbrs_1_cnt; j++)
        {
            v2 = nbrs_1[j];
            nbrs_2 = getVertexNeighbors(v2, nbrs_2_cnt);
            for (VertexID k = 0; k < nbrs_1_cnt; k++)
            {
                count_1 = 0;
                v3 = nbrs_1[k];
                if(!(is_smaller(v2, v3))){
                    continue;
                }

                //std::cout << "v1 : " << v1 << " v2 : " << v2 << " v3 : " << v3 << std::endl;
                nbrs_3 = getVertexNeighbors(v3, nbrs_3_cnt);
                count_1 = array_intersection(nbrs_2, nbrs_2_cnt, nbrs_3, nbrs_3_cnt);
                exact_count += count_1;
            }
        }
    }

    //std::cout << "Count 1 : " << exact_count << std::endl;

    // 1234 - Square

    long long count_2 = 0;

    for (VertexID v1 = 0; v1 < vertices_count; v1++)
    {
        nbrs_1 = getVertexNeighbors(v1, nbrs_1_cnt);
        for (VertexID j = 0; j < nbrs_1_cnt; j++)
        {
            v2 = nbrs_1[j];
            nbrs_2 = getVertexNeighbors(v2, nbrs_2_cnt);
            for (VertexID k = 0; k < nbrs_2_cnt; k++)
            {
                count_2 = 0;
                v3 = nbrs_2[k];
                nbrs_3 = getVertexNeighbors(v3, nbrs_3_cnt);
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

    for (VertexID v1 = 0; v1 < vertices_count; v1++)
    {
        nbrs_1 = getVertexNeighbors(v1, nbrs_1_cnt);

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

    for (auto const &[key, value] : wedge_map)
    {
        //std::cout << "Key : " << key.first << " " << key.second << " - " << value << std::endl;
        count_3 += (value * (value - 1)) / 2;
    }

    std::cout << "Count 3 : " << count_3 << std::endl;

    exact_count += count_3;

    return exact_count;
}

long long Graph::count_exact_square_parallel()
{

    long long exact_count = 0;

#pragma omp parallel reduction(+ : exact_count)
    {
        std::set<VertexID> intersection_set;

#pragma omp for schedule(dynamic)
        for (VertexID u = 0; u < vertices_count; u++)
        {

            ui u_nbr_cnt;
            VertexID *u_nbrs = getVertexNeighbors(u, u_nbr_cnt);

            for (VertexID i = 0; i < u_nbr_cnt; i++)
            {
                VertexID v = u_nbrs[i];
                if (u < v)
                {
                    ui v_nbr_cnt;
                    VertexID *v_nbrs = getVertexNeighbors(v, v_nbr_cnt);
                    std::set<VertexID> v_nbr_set(v_nbrs, v_nbrs + v_nbr_cnt);

                    for (VertexID j = 0; j < u_nbr_cnt; j++)
                    {
                        VertexID x = u_nbrs[j];
                        int x_valid_idx = 0;
                        if (v < x)
                        {
                            ui x_nbr_cnt;
                            VertexID *x_nbrs = getVertexNeighbors(x, x_nbr_cnt);
                            for (VertexID k = 0; k < x_nbr_cnt; k++)
                            {
                                if (x_nbrs[k] < u)
                                {
                                    x_valid_idx++;
                                }
                                else
                                {
                                    break;
                                }
                            }

                            if (x_valid_idx == 0)
                            {
                                break;
                            }

                            std::set<VertexID> x_nbr_set(x_nbrs, x_nbrs + x_valid_idx);

                            intersection_set.clear();

                            std::set_intersection(v_nbr_set.begin(), v_nbr_set.end(), x_nbr_set.begin(), x_nbr_set.end(),
                                                  std::inserter(intersection_set, intersection_set.begin()));

                            exact_count += intersection_set.size();
                        }
                    }
                }
            }
        }
    }

    return exact_count;
}
