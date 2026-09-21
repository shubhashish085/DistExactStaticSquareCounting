#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>
#include <metis.h>

typedef unsigned int ui;


void write_partitioned_vertices(idx_t& vertices_count, idx_t* part, std::string& output_file_path){

    std::ofstream temp_file;
    temp_file.open(output_file_path);

    for(idx_t i = 0; i < vertices_count; i++){
        temp_file << i << "	" << part[i] << std::endl;        
    }

    temp_file.close();

}

void distribute_graph(const std::string& dataset_path, idx_t& vertices_count, idx_t& part_vertices_count, idx_t& edges_count, 
                        idx_t* part, int nPart, int rank, std::string prefix_file_name){

    std::ifstream infile(dataset_path);
    std::ofstream temp_file;
    
    idx_t part_vtx_count = 0; 

    if (!infile.is_open()) {
        std::cout << "Can not open the graph file " << dataset_path << " ." << std::endl;
        exit(-1);
    }

    for(idx_t i = 0; i < vertices_count; i++){
        
        if(part[i] == rank){
            part_vtx_count++;
        }
    }

    std::string input_line;

    ui line_count = 0, begin, end, comment_line_count = 4;


    while (std::getline(infile, input_line)) {
        
        if (input_line.rfind("#", 0) == 0) {
            line_count++;                       
        }

        if(line_count >= comment_line_count){
            break;
        }
    }


    temp_file.open(std::to_string(nPart) + "_" + prefix_file_name + "_" +  std::to_string(rank) + ".graph");

    temp_file << "# Undirected graph: " << dataset_path << std::endl;
    temp_file << "# " << prefix_file_name  << " - " << rank << std::endl;
    temp_file << "# Nodes: " << part_vtx_count << std::endl;
    temp_file << "# FromNodeId	ToNodeId" << std::endl;


    while(infile >> begin){

        infile >> end;

        if(begin >= vertices_count || end >= vertices_count || begin == end){
                continue;
        }

        if(part[begin] == rank && part[end] == rank){
            temp_file << begin << "	" << end << std::endl;
        }
    }

    infile.close();
    temp_file.close();
}


void listing_partitioned_edges(const std::string& dataset_path, idx_t& vertices_count, idx_t& part_vertices_count, idx_t& edges_count, 
                        idx_t* part, int nPart, std::string prefix_file_name){

    std::ifstream infile(dataset_path);
    std::ofstream temp_file;

    std::map<idx_t, bool> vertex_map;
    std::map<std::pair<idx_t, idx_t>, bool> edge_map;
    
    idx_t part_vtx_count = 0; 

    if (!infile.is_open()) {
        std::cout << "Can not open the graph file " << dataset_path << " ." << std::endl;
        exit(-1);
    }


    std::string input_line;

    ui line_count = 0, begin, end, comment_line_count = 4;


    while (std::getline(infile, input_line)) {
        
        if (input_line.rfind("#", 0) == 0) {
            line_count++;                       
        }

        if(line_count >= comment_line_count){
            break;
        }
    }

    while(infile >> begin){

        infile >> end;

        if(begin >= vertices_count || end >= vertices_count || begin == end){
                continue;
        }

        if(part[begin] != part[end]){
            edge_map[std::make_pair(begin, end)] = true;

            if(vertex_map.find(begin) == vertex_map.end()){
                part_vtx_count++;
                vertex_map[begin] = true; 
            }

            if(vertex_map.find(end) == vertex_map.end()){
                part_vtx_count++;
                vertex_map[end] = true;
            }
        }
    }

    infile.close();

    std::cout << "Vertices in the Partition : " << part_vtx_count << std::endl;

    temp_file.open(std::to_string(nPart) + "_" + prefix_file_name + "_" +  "partition" + ".graph");

    temp_file << "# Undirected graph: " << dataset_path << std::endl;
    temp_file << "# " << prefix_file_name  << " -  Partition"<< std::endl;
    temp_file << "# Nodes: " << part_vtx_count << std::endl;
    temp_file << "# FromNodeId	ToNodeId" << std::endl;


    for(const auto edge: edge_map){
        temp_file << edge.first.first << "	" << edge.first.second << std::endl;   
    }

    temp_file.close();
}


void read_graph_by_both_direction_edges(const std::string& file_path, idx_t& vertices_count, idx_t& edges_count,  idx_t*& offsets, idx_t*& neighbors){

    ui* degrees;


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

    ui begin, end;

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

    offsets = new idx_t[vertices_count + 1];
    offsets[0] = 0;

    neighbors = new idx_t[edges_count];

    std::cout << "Initialization Finished" << std::endl;

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
}


void read_knkr_graph(const std::string& file_path, idx_t& vertices_count, idx_t& edges_count,  idx_t*& offsets, idx_t*& neighbors){

    ui* degrees;
    
    std::cout << "############# Loading Kronecker Graph ###############" << std::endl;

    std::ifstream infile(file_path);

    idx_t max_vertex_id = 0;

    if (!infile.is_open()) {
        std::cout << "Can not open the graph file " << file_path << " ." << std::endl;
        exit(-1);
    }

    char type;

    std::cout << "Reading File............ " << std::endl;

    idx_t edge_count = 0, count = 0;
    idx_t begin, end;


    while(infile >> begin >> end) {

        if(begin == end){
            continue;
        }

        edge_count++;

        if(max_vertex_id < begin){
            max_vertex_id = begin;
        }

        if(max_vertex_id < end){
            max_vertex_id = end;
        }

    }

    infile.close();

    vertices_count = max_vertex_id + 1;
    edges_count = edge_count / 2;

    std::ifstream degree_infile(file_path);

    degrees = new ui[vertices_count];
    std::fill(degrees, degrees + vertices_count, 0);

    while(degree_infile >> begin >> end) {

        if(begin == end){
            continue;
        }

        degrees[begin] += 1;
    }

    degree_infile.close();

    
    offsets = new idx_t[vertices_count +  1];
    offsets[0] = 0;

    neighbors = new idx_t[edge_count];

    std::cout << "Vertices Count : " << vertices_count << " Edge Count : " << edges_count << std::endl;

    std::vector<ui> neighbors_offset(vertices_count, 0);// used for adjust neighbors with offset


    for(ui id = 0; id < vertices_count; id++){
        offsets[id + 1] = offsets[id] + degrees[id];
    }


    ui offset;

    std::ifstream input_file(file_path);

    while(input_file >> begin >> end){

        if(begin == end && begin >= vertices_count && end >= vertices_count){
            continue;
        }        

        offset = offsets[begin] + neighbors_offset[begin]; // adjusting the index of neighbor in neighbors array
        neighbors[offset] = end;
        neighbors_offset[begin] += 1;
    }

    input_file.close();
}




void read_graph(const std::string& file_path, idx_t& vertices_count, idx_t& edges_count,  idx_t*& offsets, idx_t*& neighbors){

    ui* degrees;

    std::cout << "Loading Graph With Edges ...." << std::endl;

    std::ifstream infile(file_path);

    if (!infile.is_open()) {
        std::cout << "Can not open the graph file " << file_path << " ." << std::endl;
        exit(-1);
    }

    char type;
    std::string input_line;
    ui label = 0;

    std::cout << "Reading File ............ " << std::endl;

    ui line_count = 0, count = 0, comment_line_count = 4;

    while (std::getline(infile, input_line)) {

        if (input_line.rfind("#", 0) == 0) {

            line_count++;

            if (input_line.rfind("# Nodes", 0) == 0) {
                
                std::stringstream ss(input_line);
                std::string token;
                int count = 0;
                while (!ss.eof()) {
                    std::getline(ss, token, ' ');
                    if (!(token.rfind("#", 0) == 0 || token.rfind("Nodes:", 0) == 0 || token.rfind("Edges:", 0) == 0)) {
                        if (count == 0) {
                            vertices_count = stoi(token);
                            degrees = new ui[vertices_count];
                            std::fill(degrees, degrees + vertices_count, 0);
                            count = 1;
                        } else {
                            count = 0;
                        }
                    }
                }
            }
        }

        if(line_count >= comment_line_count){
            break;
        }
    }

    ui begin, end;

    edges_count = 0;


    while(infile >> begin) {

        infile >> end;

        if (begin != end && begin < vertices_count && end < vertices_count) {
            degrees[begin] += 1;
            degrees[end] += 1;
            edges_count++;
        }
    }

    infile.close();

    std::ifstream input_file(file_path);



    offsets = new idx_t[vertices_count +  1];
    offsets[0] = 0;

    std::cout << "Offset Array Created " << std::endl;

    neighbors = new idx_t[edges_count * 2];

    std::cout << "Neighbor Array Created " << std::endl;

    std::vector<ui> neighbors_offset(vertices_count, 0);// used for adjust neighbors with offset

    std::cout << "Neighbor Offset Array Created " << std::endl;

    for(ui id = 0; id < vertices_count; id++){
        offsets[id + 1] = offsets[id] + degrees[id];
    }

    line_count = 0;

    while (std::getline(input_file, input_line)) {
        line_count++;
        if(line_count >= comment_line_count){
            break;
        }
    }

    idx_t total_edge_count = 0;

    while(input_file >> begin){ // Read edge.

        input_file >> end;

        line_count++;
        if(begin >= vertices_count || end >= vertices_count || begin == end){
            continue;
        }

        total_edge_count++;

        ui offset = offsets[begin] + neighbors_offset[begin]; // adjusting the index of neighbor in neighbors array
        neighbors[offset] = end;

        offset = offsets[end] + neighbors_offset[end]; // adjusting the index of neighbor in neighbors array
        neighbors[offset] = begin;

        neighbors_offset[begin] += 1;
        neighbors_offset[end] += 1;

    }

    std::cout << "Total Vertices Count : " << vertices_count << "  Total Edge Count : " << total_edge_count * 2 << std::endl; 
    input_file.close();
}


int main(int argc, char** argv){

    std::string input_graph_file = argv[1];
    std::string dataset_name = argv[2];

    int numberOfParts[8] = {2, 4, 8, 16, 32, 64, 128, 256};
    //int numberOfParts[2] = {128, 256};

    idx_t vertices_count = 0, edges_count = 0, comment_line_count;
    idx_t* offsets;
    idx_t* neighbors;

    read_graph(input_graph_file, vertices_count, edges_count, offsets, neighbors);
    //read_knkr_graph(input_graph_file, vertices_count, edges_count, offsets, neighbors);
    //read_graph_by_both_direction_edges(input_graph_file, vertices_count, edges_count, offsets, neighbors);

    idx_t* part = new idx_t[vertices_count];

    for (int i = 0; i < 8; i++){

        idx_t nParts = numberOfParts[i];

        std::cout << std::endl;
        std::cout << "Name of the dataset : " << input_graph_file << std::endl;
        std::cout << "Number of partitions : " << nParts << std::endl;
        std::cout << std::endl;

                
        idx_t ncon = 1;
         
        for(idx_t j = 0; j < vertices_count; j++){
            part[j] = -1;
        }
        
        idx_t objval;


        // Call the METIS graph partitioning function
        int ret = METIS_PartGraphKway(&vertices_count, &ncon, offsets, neighbors, NULL, NULL, 
                                      NULL, &nParts, NULL, NULL, NULL, &objval, part);

        if (ret != METIS_OK) {
            std::cerr << "METIS partitioning failed!" << std::endl;
            return 1;
        }

        std::cout << "Metis Partitioning Done for n : " << nParts << std::endl;

        idx_t part_vertices_count = 0;

        //for (idx_t j = 0; j < nParts; j++){
            //distribute_graph(input_graph_file, vertices_count, part_vertices_count, edges_count, part, nParts, j, dataset_name);
        //}

        std::string final_output_file =  std::to_string(nParts) + "_" + dataset_name;

        write_partitioned_vertices(vertices_count, part, final_output_file);
        
        //listing_partitioned_edges(input_graph_file, vertices_count, part_vertices_count, edges_count, part, nParts, "dblp");    

        std::cout << "Number of Edge Cuts : " << objval << std::endl;
    }

    delete[] part;  

    return 0;
}
