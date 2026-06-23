

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

    //std::cout << "############# Loading Graph With Edges ###############" << std::endl;

    std::ifstream infile(file_path);

    if (!infile.is_open())
    {
        std::cout << "Can not open the graph file " << file_path << " ." << std::endl;
        exit(-1);
    }

    char type;
    std::string input_line;
    ui label = 0;

    //std::cout << "Reading File............ " << std::endl;

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
                            //std::cout << "Vertex Count : " << vertices_count << std::endl;
                            degrees = new ui[vertices_count];
                            std::fill(degrees, degrees + vertices_count, 0);
                            count = 1;
                        }
                        else
                        {
                            edges_count = stoi(token);
                            count = 0;
                        }
                        //std::cout << "Vertices Count : " << vertices_count << " Edges Count : " << edges_count << std::endl;
                    }
                }
            }
        }

        if (line_count >= comment_line_count)
        {
            break;
        }
    }

    // Hard-coded vertices
    //vertices_count = 334863;

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

    //std::cout << "Initialization Finished" << std::endl;

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


void Graph::loadGraphAndPartitionFromFile(const std::string &file_path, const std::string& vtx_ptn_file_path, int partition_no)
{
    std::ifstream infile(file_path);
    std::ifstream vertex_partition_file(vtx_ptn_file_path);
    std::ifstream vtx_ptn_file(vtx_ptn_file_path);
    VertexID vertex_id;
    NodeID partition_id;


    if (!vertex_partition_file.is_open() || !infile.is_open()){
        std::cout << "Can not open the graph file " << vtx_ptn_file_path << " or " << file_path << "." << std::endl;
        exit(-1);
    }

    vertices_count = 0;

    while (vertex_partition_file >> vertex_id){

        vertex_partition_file >> partition_id;
        vertices_count++;
    }

    vertex_partition_file.close();

    partition = new NodeID[vertices_count];

    while (vtx_ptn_file >> vertex_id){
        vtx_ptn_file >> partition_id;
        partition[vertex_id] = partition_id;
    }

    vtx_ptn_file.close();


    char type;
    std::string input_line;
    ui label = 0;
    ui line_count = 0, count = 0, comment_line_count = 4;

    while (std::getline(infile, input_line)){

        if (input_line.rfind("#", 0) == 0){

            line_count++;

            if (input_line.rfind("# Nodes", 0) == 0)
            {

                std::stringstream ss(input_line);
                std::string token;
                int count = 0;

                while (!ss.eof()){

                    std::getline(ss, token, ' ');

                    if (!(token.rfind("#", 0) == 0 || token.rfind("Nodes:", 0) == 0 || token.rfind("Edges:", 0) == 0))
                    {
                        if (count == 0)
                        {
                            //ui total_vertices_count = stoi(token);
                            degrees = new ui[vertices_count];
                            std::fill(degrees, degrees + vertices_count, 0);
                            count = 1;
                        } else {
                            //edges_count = stoi(token);
                            count = 0;
                        }
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

    while (infile >> begin){

        infile >> end;

        if (begin != end && begin < vertices_count && end < vertices_count){
            degrees[begin] += 1;
            degrees[end] += 1;
            edges_count++;
        }
    }

    infile.close();

    std::ifstream input_file(file_path);

    offsets = new ui[vertices_count + 1];
    offsets[0] = 0;

    neighbors = new VertexID[edges_count * 2];
    std::vector<ui> neighbors_offset(vertices_count, 0);

    for (ui id = 0; id < vertices_count; id++){
        offsets[id + 1] = offsets[id] + degrees[id];
    }

    line_count = 0;

    while (std::getline(input_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count){
            break;
        }
    }

    ui offset = 0;

    while (input_file >> begin){

        input_file >> end;

        if (begin != end && begin < vertices_count && end < vertices_count)
        {
            offset = offsets[begin] + neighbors_offset[begin];
            neighbors[offset] = end;

            offset = offsets[end] + neighbors_offset[end];
            neighbors[offset] = begin;

            neighbors_offset[begin] += 1;
            neighbors_offset[end] += 1;
        }

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



void Graph::loadBidirectionalGraphAndPartitionFromFile(const std::string& file_path, const std::string& vtx_ptn_file_path, int partition_no)
{
    std::ifstream infile(file_path);
    std::ifstream vertex_partition_file(vtx_ptn_file_path);
    std::ifstream vtx_ptn_file(vtx_ptn_file_path);
    VertexID vertex_id;
    NodeID partition_id;


    if (!vertex_partition_file.is_open() || !infile.is_open()){
        std::cout << "Can not open the graph file " << vtx_ptn_file_path << " or " << file_path << "." << std::endl;
        exit(-1);
    }

    vertices_count = 0;

    while (vertex_partition_file >> vertex_id){

        vertex_partition_file >> partition_id;
        vertices_count++;
    }

    vertex_partition_file.close();

    partition = new NodeID[vertices_count];

    while (vtx_ptn_file >> vertex_id){
        vtx_ptn_file >> partition_id;
        partition[vertex_id] = partition_id;
    }

    vtx_ptn_file.close();


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
    cut_edges_count = 0;

    while (infile >> begin){

        infile >> end;
        contains_in_partition = false;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count))
        {
            if (partition[begin] == partition_no || partition[end] == partition_no){

                contains_in_partition = true;

                if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
                    vertex_idx_map[begin] = vertices_count;
                    vertices_count++;
                }

                if(vertex_idx_map.find(end) == vertex_idx_map.end()){
                    vertex_idx_map[end] = vertices_count;
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
    local_partition = new NodeID[vertices_count];
    std::fill(degrees, degrees + vertices_count, 0);
    offsets = new ui[vertices_count + 1];

    line_count = 0;

    std::ifstream input_file(file_path);

    while (std::getline(input_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count){
            break;
        }
    }

    VertexID begin_idx, end_idx;

    while (input_file >> begin)
    {
        input_file >> end;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if ((partition[begin] == partition_no) || (partition[end] == partition_no)){
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end]; 
                degrees[begin_idx] += 1;
                degrees[end_idx] += 1;
                local_partition[begin_idx] = partition[begin];
                local_partition[end_idx] = partition[end];

                if(partition[begin] != partition[end]){
                    cut_edges_count++;
                }                
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


    while (in_file >> begin)
    {
        in_file >> end;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if ((partition[begin] == partition_no) || (partition[end] == partition_no)){

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


void Graph::loadPartitionedLocalGraphWoCutEdgesFromFile(const std::string &file_path, const std::string &vertex_partition_file_path, int partition_no)
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
    ghost_vertices_count = 0;

    while (infile >> begin){

        infile >> end;
        contains_in_partition = false;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count))
        {
            if ((partition[begin] == partition_no) && (partition[end] == partition_no)){

                if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
                    vertex_idx_map[begin] = vertices_count;
                    vertices_count++;
                }

                if(vertex_idx_map.find(end) == vertex_idx_map.end()){
                    vertex_idx_map[end] = vertices_count;
                    vertices_count++;
                }

                edges_count++;              
            }else if((partition[begin] == partition_no) && (partition[end] != partition_no)){

                if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
                    vertex_idx_map[begin] = vertices_count;
                    vertices_count++;
                }

                if(other_ptn_vertex_idx_map.find(end) == other_ptn_vertex_idx_map.end()){
                    other_ptn_vertex_idx_map[end] = ghost_vertices_count;
                    ghost_vertices_count++;
                }

                cut_edges_count++;

            }else if ((partition[begin] != partition_no) && (partition[end] == partition_no)){

                if(vertex_idx_map.find(end) == vertex_idx_map.end()){
                    vertex_idx_map[end] = vertices_count;
                    vertices_count++;
                }

                if(other_ptn_vertex_idx_map.find(begin) == other_ptn_vertex_idx_map.end()){
                    other_ptn_vertex_idx_map[begin] = ghost_vertices_count;
                    ghost_vertices_count++;
                }

                cut_edges_count++;
            }
        }
    }

    infile.close();

    ui offset;

    degrees = new ui[vertices_count];
    ghost_degrees = new ui[ghost_vertices_count];
    local_partition = new NodeID[vertices_count];
    std::fill(degrees, degrees + vertices_count, 0);
    std::fill(ghost_degrees, ghost_degrees + ghost_vertices_count, 0);
    offsets = new ui[vertices_count + 1];
    ghost_offsets = new ui[ghost_vertices_count + 1];

    line_count = 0;

    std::ifstream input_file(file_path);

    while (std::getline(input_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count){
            break;
        }
    }

    VertexID begin_idx, end_idx;

    while (input_file >> begin)
    {
        input_file >> end;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if ((partition[begin] == partition_no) && (partition[end] == partition_no)){
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end]; 
                degrees[begin_idx] += 1;
                degrees[end_idx] += 1;
                local_partition[begin_idx] = partition[begin];
                local_partition[end_idx] = partition[end];                
            }else if ((partition[begin] == partition_no) && (partition[end] != partition_no)){
                begin_idx = vertex_idx_map[begin];
                end_idx = other_ptn_vertex_idx_map[end];
                ghost_degrees[end_idx] += 1;
                local_partition[begin_idx] = partition[begin];
            }else if ((partition[begin] != partition_no) && (partition[end] == partition_no)){
                begin_idx = other_ptn_vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];
                ghost_degrees[begin_idx] += 1;
                local_partition[end_idx] = partition[end];
            }
        }
    }

    input_file.close();

    offsets[0] = 0;
    ghost_offsets[0] = 0;

    neighbors = new VertexID[edges_count * 2];
    ghost_neighbors = new VertexID[cut_edges_count];

    std::vector<ui> neighbors_offset(vertices_count, 0);
    std::vector<ui> ghost_neighbors_offset(ghost_vertices_count, 0);

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    for (VertexID i = 0; i < ghost_vertices_count; i++){
        ghost_offsets[i + 1] = ghost_offsets[i] + ghost_degrees[i];
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


    while (in_file >> begin)
    {
        in_file >> end;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if ((partition[begin] == partition_no) && (partition[end] == partition_no)){

                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];

                offset = offsets[begin_idx] + neighbors_offset[begin_idx];
                neighbors[offset] = end_idx;
                neighbors_offset[begin_idx] += 1;

                offset = offsets[end_idx] + neighbors_offset[end_idx];
                neighbors[offset] = begin_idx;
                neighbors_offset[end_idx] += 1;

            }else if ((partition[begin] == partition_no) && (partition[end] != partition_no)){

                begin_idx = vertex_idx_map[begin];
                end_idx = other_ptn_vertex_idx_map[end];

                offset = ghost_offsets[end_idx] + ghost_neighbors_offset[end_idx];
                ghost_neighbors[offset] = begin_idx;
                ghost_neighbors_offset[end_idx] += 1;

            }else if ((partition[begin] != partition_no) && (partition[end] == partition_no)){

                begin_idx = other_ptn_vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];

                offset = ghost_offsets[begin_idx] + ghost_neighbors_offset[begin_idx];
                ghost_neighbors[offset] = end_idx;
                ghost_neighbors_offset[begin_idx] += 1;
            }
        }
    }

    in_file.close();

    for (ui i = 0; i < vertices_count; ++i){
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }

    for (ui i = 0; i < ghost_vertices_count; i++){
        std::sort(ghost_neighbors + ghost_offsets[i], ghost_neighbors + ghost_offsets[i + 1]);
    }
}


void Graph::loadPartitionedLocalGraphWoCutEdgesBidirection(const std::string &file_path, const std::string &vertex_partition_file_path, int partition_no)
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
    ghost_vertices_count = 0;

    while (infile >> begin){

        infile >> end;
        contains_in_partition = false;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count))
        {
            if ((partition[begin] == partition_no) && (partition[end] == partition_no)){

                if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
                    vertex_idx_map[begin] = vertices_count;
                    vertices_count++;
                }

                if(vertex_idx_map.find(end) == vertex_idx_map.end()){
                    vertex_idx_map[end] = vertices_count;
                    vertices_count++;
                }

                edges_count++;              
            }else if((partition[begin] == partition_no) && (partition[end] != partition_no)){

                if(vertex_idx_map.find(begin) == vertex_idx_map.end()){
                    vertex_idx_map[begin] = vertices_count;
                    vertices_count++;
                }

                if(other_ptn_vertex_idx_map.find(end) == other_ptn_vertex_idx_map.end()){
                    other_ptn_vertex_idx_map[end] = ghost_vertices_count;
                    ghost_vertices_count++;
                }

                cut_edges_count++;

            }else if ((partition[begin] != partition_no) && (partition[end] == partition_no)){

                if(vertex_idx_map.find(end) == vertex_idx_map.end()){
                    vertex_idx_map[end] = vertices_count;
                    vertices_count++;
                }

                if(other_ptn_vertex_idx_map.find(begin) == other_ptn_vertex_idx_map.end()){
                    other_ptn_vertex_idx_map[begin] = ghost_vertices_count;
                    ghost_vertices_count++;
                }

                cut_edges_count++;
            }
        }
    }

    infile.close();

    ui offset;

    degrees = new ui[vertices_count];
    ghost_degrees = new ui[ghost_vertices_count];
    local_partition = new NodeID[vertices_count];
    std::fill(degrees, degrees + vertices_count, 0);
    std::fill(ghost_degrees, ghost_degrees + ghost_vertices_count, 0);
    offsets = new ui[vertices_count + 1];
    ghost_offsets = new ui[ghost_vertices_count + 1];

    line_count = 0;

    std::ifstream input_file(file_path);

    while (std::getline(input_file, input_line))
    {
        line_count++;
        if (line_count >= comment_line_count){
            break;
        }
    }

    VertexID begin_idx, end_idx;

    while (input_file >> begin)
    {
        input_file >> end;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if ((partition[begin] == partition_no) && (partition[end] == partition_no)){
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end]; 
                degrees[begin_idx] += 1;
                local_partition[begin_idx] = partition[begin];                
            }else if ((partition[begin] != partition_no) && (partition[end] == partition_no)){
                begin_idx = other_ptn_vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];
                ghost_degrees[begin_idx] += 1;
                local_partition[begin_idx] = partition[begin]; 
                local_partition[end_idx] = partition[end];
            }
        }
    }

    input_file.close();

    offsets[0] = 0;
    ghost_offsets[0] = 0;

    neighbors = new VertexID[edges_count];
    ghost_neighbors = new VertexID[cut_edges_count];

    std::vector<ui> neighbors_offset(vertices_count, 0);
    std::vector<ui> ghost_neighbors_offset(ghost_vertices_count, 0);

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    for (VertexID i = 0; i < ghost_vertices_count; i++){
        ghost_offsets[i + 1] = ghost_offsets[i] + ghost_degrees[i];
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


    while (in_file >> begin)
    {
        in_file >> end;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if ((partition[begin] == partition_no) && (partition[end] == partition_no)){

                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];

                offset = offsets[begin_idx] + neighbors_offset[begin_idx];
                neighbors[offset] = end_idx;
                neighbors_offset[begin_idx] += 1;

            }else if ((partition[begin] != partition_no) && (partition[end] == partition_no)){

                begin_idx = other_ptn_vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];

                offset = ghost_offsets[begin_idx] + ghost_neighbors_offset[begin_idx];
                ghost_neighbors[offset] = end_idx;
                ghost_neighbors_offset[begin_idx] += 1;
            }
        }
    }

    in_file.close();

    for (ui i = 0; i < vertices_count; ++i){
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }

    for (ui i = 0; i < ghost_vertices_count; i++){
        std::sort(ghost_neighbors + ghost_offsets[i], ghost_neighbors + ghost_offsets[i + 1]);
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

    //std::cout << "######## Total Vertices Count : " << total_vertices_count << std::endl; 

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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((partition[begin] != partition[end]) && ((partition[begin] == partition_no) || (partition[end] == partition_no))){
                
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
    local_partition = new NodeID[vertices_count];
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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){
                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end]; 
                degrees[begin_idx] += 1;
                degrees[end_idx] += 1;
                local_partition[begin_idx] = partition[begin];
                local_partition[end_idx] = partition[end];
                edges_count++;

                if((partition_no == partition[begin]) && (partition[begin] == partition[end])){
                    interface_edges.push_back(std::make_pair(begin_idx, end_idx));
                }
            }
        }
    }

    //std::cout << "Partition - " << partition_no << " Edge Count : " << edges_count << std::endl; 

    input_file.close();

    offsets[0] = 0;
    neighbors = new VertexID[edges_count * 2];

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    std::ifstream in_file(file_path);

    line_count = 0, comment_line_count = 4;

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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
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


void Graph::loadPartitionedInterfaceGraphOptimized(const std::string& file_path, const std::string& vtx_ptn_file_path, int partition_no){

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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((partition[begin] != partition[end]) && ((partition[begin] == partition_no) || (partition[end] == partition_no))){
                
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
    local_partition = new NodeID[vertices_count];
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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){
                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }

                if((partition[begin] != partition[end]) && ((partition[begin] == partition_no) || (partition[end] == partition_no))){
                    if(partition[begin] != partition_no){

                        begin_idx = vertex_idx_map[begin];
                        end_idx = vertex_idx_map[end]; 
                        degrees[end_idx] += 1;
                        local_partition[begin_idx] = partition[begin];
                        local_partition[end_idx] = partition[end];

                    }else if (partition[end] != partition_no){

                        begin_idx = vertex_idx_map[begin];
                        end_idx = vertex_idx_map[end]; 
                        degrees[begin_idx] += 1;
                        local_partition[begin_idx] = partition[begin];
                        local_partition[end_idx] = partition[end];
                    }

                    edges_count++;

                } else if ((partition[begin] != partition[end]) && ((partition[begin] != partition_no) && (partition[end] != partition_no))) {

                    begin_idx = vertex_idx_map[begin];
                    end_idx = vertex_idx_map[end]; 
                    local_partition[begin_idx] = partition[begin];
                    local_partition[end_idx] = partition[end];
                    other_ptn_edges.insert(std::make_pair(std::min(begin_idx, end_idx), std::max(begin_idx, end_idx)));

                } else if ((partition_no != partition[begin]) && (partition[begin] == partition[end])) {

                    begin_idx = vertex_idx_map[begin];
                    end_idx = vertex_idx_map[end]; 
                    local_partition[begin_idx] = partition[begin];
                    local_partition[end_idx] = partition[end];
                    other_ptn_edges.insert(std::make_pair(std::min(begin_idx, end_idx), std::max(begin_idx, end_idx)));

                } else if((partition_no == partition[begin]) && (partition[begin] == partition[end])){
                    
                    begin_idx = vertex_idx_map[begin];
                    end_idx = vertex_idx_map[end];
                    local_partition[begin_idx] = partition[begin];
                    local_partition[end_idx] = partition[end]; 
                    interface_edges.push_back(std::make_pair(begin_idx, end_idx));
                }
            }
        }
    }

    //std::cout << "Partition - " << partition_no << " Edge Count : " << edges_count << std::endl; 

    input_file.close();

    offsets[0] = 0;
    neighbors = new VertexID[edges_count * 2];

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    std::ifstream in_file(file_path);

    line_count = 0, comment_line_count = 4;

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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){

                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }

                if((partition[begin] != partition[end]) && ((partition[begin] == partition_no) || (partition[end] == partition_no))){

                    begin_idx = vertex_idx_map[begin];
                    end_idx = vertex_idx_map[end]; 

                    if(partition[begin] != partition_no){

                        offset = offsets[end_idx] + neighbors_offset[end_idx];
                        neighbors[offset] = begin_idx;
                        neighbors_offset[end_idx] += 1;

                    }else if (partition[end] != partition_no){

                        offset = offsets[begin_idx] + neighbors_offset[begin_idx];
                        neighbors[offset] = end_idx;
                        neighbors_offset[begin_idx] += 1;
                    }
                }                
            }
        }    
    }

    in_file.close();
    
    for (ui i = 0; i < vertices_count; ++i){
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }    
}




void Graph::loadPartitionedInterfaceGraphBidirection(const std::string& file_path, const std::string& vtx_ptn_file_path, int partition_no){

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

    //std::cout << "######## Total Vertices Count : " << total_vertices_count << std::endl; 

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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((partition[begin] != partition[end]) && ((partition[begin] == partition_no) || (partition[end] == partition_no))){
                
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
    local_partition = new NodeID[vertices_count];
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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){
                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];
                degrees[begin_idx] += 1;
                local_partition[begin_idx] = partition[begin];
                local_partition[end_idx] = partition[end];
                edges_count++;

                if((partition_no == partition[begin]) && (partition[begin] == partition[end]) && (begin < end)){
                    interface_edges.push_back(std::make_pair(begin_idx, end_idx));
                }
            }
        }
    }

    //std::cout << "Partition - " << partition_no << " Edge Count : " << edges_count << std::endl; 

    input_file.close();

    offsets[0] = 0;
    neighbors = new VertexID[edges_count];

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    std::ifstream in_file(file_path);

    line_count = 0, comment_line_count = 4;

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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){

                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }

                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end];        

                offset = offsets[begin_idx] + neighbors_offset[begin_idx];
                neighbors[offset] = end_idx;
                neighbors_offset[begin_idx] += 1;
            }
        }    
    }

    in_file.close();
    
    for (ui i = 0; i < vertices_count; ++i){
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }    
}


void Graph::loadPartitionedInterfaceGraphBidirectionOptimized(const std::string& file_path, const std::string& vtx_ptn_file_path, int partition_no){

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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((partition[begin] != partition[end]) && ((partition[begin] == partition_no) || (partition[end] == partition_no))){
                
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
    local_partition = new NodeID[vertices_count];
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

        if ((begin < end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){
                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }

                if((partition[begin] != partition[end]) && ((partition[begin] == partition_no) || (partition[end] == partition_no))){
                    if(partition[begin] != partition_no){

                        begin_idx = vertex_idx_map[begin];
                        end_idx = vertex_idx_map[end]; 
                        degrees[end_idx] += 1;
                        local_partition[begin_idx] = partition[begin];
                        local_partition[end_idx] = partition[end];

                    }else if (partition[end] != partition_no){

                        begin_idx = vertex_idx_map[begin];
                        end_idx = vertex_idx_map[end]; 
                        degrees[begin_idx] += 1;
                        local_partition[begin_idx] = partition[begin];
                        local_partition[end_idx] = partition[end];
                    }

                    edges_count++;

                } else if ((partition[begin] != partition[end]) && ((partition[begin] != partition_no) && (partition[end] != partition_no))) {

                    begin_idx = vertex_idx_map[begin];
                    end_idx = vertex_idx_map[end]; 
                    local_partition[begin_idx] = partition[begin];
                    local_partition[end_idx] = partition[end];
                    other_ptn_edges.insert(std::make_pair(std::min(begin_idx, end_idx), std::max(begin_idx, end_idx)));

                } else if ((partition_no != partition[begin]) && (partition[begin] == partition[end])) {

                    begin_idx = vertex_idx_map[begin];
                    end_idx = vertex_idx_map[end]; 
                    local_partition[begin_idx] = partition[begin];
                    local_partition[end_idx] = partition[end];
                    other_ptn_edges.insert(std::make_pair(std::min(begin_idx, end_idx), std::max(begin_idx, end_idx)));

                } else if((partition_no == partition[begin]) && (partition[begin] == partition[end])){
                    
                    begin_idx = vertex_idx_map[begin];
                    end_idx = vertex_idx_map[end];
                    local_partition[begin_idx] = partition[begin];
                    local_partition[end_idx] = partition[end]; 
                    interface_edges.push_back(std::make_pair(begin_idx, end_idx));
                }
            }
        }
    }

    input_file.close();

    offsets[0] = 0;
    neighbors = new VertexID[edges_count];

    for (VertexID i = 0; i < vertices_count; i++){
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    std::ifstream in_file(file_path);

    line_count = 0, comment_line_count = 4;

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

        if ((begin < end) && (begin < total_vertices_count) && (end < total_vertices_count)){
            if((vertex_idx_map.find(begin) != vertex_idx_map.end()) && (vertex_idx_map.find(end) != vertex_idx_map.end())){

                if((partition_no > partition[begin]) && (partition[begin] == partition[end])){
                    continue;
                }

                if((partition[begin] != partition[end]) && ((partition[begin] == partition_no) || (partition[end] == partition_no))){

                    begin_idx = vertex_idx_map[begin];
                    end_idx = vertex_idx_map[end]; 

                    if(partition[begin] != partition_no){

                        offset = offsets[end_idx] + neighbors_offset[end_idx];
                        neighbors[offset] = begin_idx;
                        neighbors_offset[end_idx] += 1;

                    }else if (partition[end] != partition_no){

                        offset = offsets[begin_idx] + neighbors_offset[begin_idx];
                        neighbors[offset] = end_idx;
                        neighbors_offset[begin_idx] += 1;
                    }
                }
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

    vertices_count = 0;
    vertex_idx_map.clear();

    std::cout << "Cut Graph - Partition Graph Loading Finished" << std::endl;

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


    while (infile >> begin){

        infile >> end;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
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
    local_partition = new NodeID[vertices_count];
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

    std::vector<VertexID> vec = {538, 156632, 240108, 240110};

    while (input_file >> begin){

        input_file >> end;      

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){

            if(partition[begin] != partition[end]){
                
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end]; 
                degrees[begin_idx] += 1;
                degrees[end_idx] += 1;
                local_partition[begin_idx] = partition[begin];
                local_partition[end_idx] = partition[end];
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

    std::cout << "Cut Graph : Neighbor Loading Finished" << std::endl;

    for (ui i = 0; i < vertices_count; ++i){
        std::sort(neighbors + offsets[i], neighbors + offsets[i + 1]);
    }
}


void Graph::loadCutGraphBidirection(const std::string& file_path, const std::string& vtx_ptn_file_path)
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

    vertices_count = 0;
    vertex_idx_map.clear();

    //std::cout << "Cut Graph - Partition Graph Loading Finished" << std::endl;

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


    while (infile >> begin){

        infile >> end;

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){
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
    local_partition = new NodeID[vertices_count];
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

        if ((begin != end) && (begin < total_vertices_count) && (end < total_vertices_count)){

            if(partition[begin] != partition[end]){
                
                begin_idx = vertex_idx_map[begin];
                end_idx = vertex_idx_map[end]; 
                degrees[begin_idx] += 1;
                local_partition[begin_idx] = partition[begin];
                local_partition[end_idx] = partition[end];
                edges_count++;

                if(begin < end){
                    cut_edge_vtr.push_back(std::make_pair(begin_idx, end_idx));
                }
            }
        }
    }

   

    input_file.close();
    
    offsets[0] = 0;
    neighbors = new VertexID[edges_count];

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

    //std::cout << "Cut Graph : Neighbor Loading Finished" << std::endl;

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

    if((main_degrees[u] == main_degrees[v]) && (u < v)){
        return true;
    }

    return false;
}


void Graph::transformToAugmentedGraph(Graph* augmented_graph){

    augmented_graph->vertices_count = vertices_count;
    augmented_graph->edges_count = edges_count;
    augmented_graph->offsets = new ui[vertices_count + 1];
    augmented_graph->neighbors = new VertexID[edges_count];
    augmented_graph->partition = new NodeID[vertices_count];

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

    for (ui i = 0; i < vertices_count; i++){
        augmented_graph->partition[i] = partition[i];
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
}


long long Graph::get_wedge_cnt_by_two_vertices(VertexID v1, VertexID v2){

    long long count = 0;
    VertexID* nbrs_1, *nbrs_2;
    ui nbrs_1_cnt = 0, nbrs_2_cnt = 0;
    std::map<std::pair<VertexID, VertexID>, ui> ptn_wedge_map;
    std::pair<VertexID, VertexID> search_pair;


    nbrs_1 = getVertexNeighbors(v1, nbrs_1_cnt);
    for (ui m = 0; m < nbrs_1_cnt; m++)
    {
        for (ui n = m+1; n < nbrs_1_cnt; n++)
        {
            search_pair = std::make_pair(std::min(nbrs_1[m], nbrs_1[n]), std::max(nbrs_1[m], nbrs_1[n]));
            auto search = ptn_wedge_map.find(search_pair);
            if (search == ptn_wedge_map.end()){
                ptn_wedge_map[search_pair] = 1;
            }
            else{
                ptn_wedge_map[search_pair] = ptn_wedge_map[search_pair] + 1;
            }
        }
    }

    nbrs_2 = getVertexNeighbors(v2, nbrs_2_cnt);

    for (ui m = 0; m < nbrs_2_cnt; m++)
    {
        for (ui n = m+1; n < nbrs_2_cnt; n++)
        {
            search_pair = std::make_pair(std::min(nbrs_2[m], nbrs_2[n]), std::max(nbrs_2[m], nbrs_2[n]));
            auto search = ptn_wedge_map.find(search_pair);
            if (search == ptn_wedge_map.end()){
                ptn_wedge_map[search_pair] = 1;
            }
            else{
                ptn_wedge_map[search_pair] = ptn_wedge_map[search_pair] + 1;
            }
        }
    }

    for (auto const &[key, value] : ptn_wedge_map){
        count += (value * (value - 1)) / 2;
    }

    return count;
}


void Graph::printGraphMetaData()
{
    std::cout << "|V|: " << vertices_count << ", |E|: " << edges_count << std::endl;
    std::cout << "Max Degree: " << max_degree << ", Max Label Frequency: " << std::endl;
}

//Shared Memory Parallel Counting Square
/*long long Graph::count_exact_square_parallel()
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
}*/
