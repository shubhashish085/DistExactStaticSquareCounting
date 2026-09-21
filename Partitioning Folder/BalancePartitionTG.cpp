#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <limits>

typedef unsigned int ui;
typedef uint32_t VertexID;
typedef short NodeID;

class Graph{

    public:

    ui vertices_count;
    ui edges_count;
    ui max_degree;

    ui* degrees;
    ui* main_degrees;

    ui* offsets;
    VertexID * neighbors;

    NodeID* partition;

    Graph(){
        
        vertices_count = 0;
        edges_count = 0;
        max_degree = 0;

        offsets = NULL;
        neighbors = NULL;
    }

    ~Graph() {
        delete[] offsets;
        delete[] neighbors;
    }

public:
    void loadGraphFromFile(const std::string& file_path);
    void loadKroneckerGraphFromFile(const std::string& file_path);
    void loadGraphFromFileForBothDirectionEdges(const std::string& file_path);

    void transformToAugmentedGraph(Graph* augmented_graph);       

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

};




void write_partitioned_vertices(ui& vertices_count, NodeID* part, std::string& output_file_path){

    std::ofstream temp_file;
    temp_file.open(output_file_path);

    for(ui i = 0; i < vertices_count; i++){
        temp_file << i << "	" << part[i] << std::endl;        
    }

    temp_file.close();

}


/*void loadGraphFromFileForBothDirectionEdges(const std::string& file_path, idx_t& vertices_count, idx_t& edges_count,  idx_t*& offsets, idx_t*& neighbors){

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
}*/


/*void Graph::loadKroneckerGraphFromFile(const std::string& file_path){

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
}*/


void Graph::loadGraphFromFile(const std::string& file_path){

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

    degrees = new ui[vertices_count];
    std::fill(degrees, degrees + vertices_count, 0);

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

    offsets = new ui[vertices_count +  1];
    offsets[0] = 0;

    neighbors = new VertexID[edges_count * 2];

    std::vector<ui> neighbors_offset(vertices_count, 0);// used for adjust neighbors with offset

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


    while(input_file >> begin){ // Read edge.

        input_file >> end;

        line_count++;
        if(begin >= vertices_count || end >= vertices_count || begin == end){
            continue;
        }

        ui offset = offsets[begin] + neighbors_offset[begin]; // adjusting the index of neighbor in neighbors array
        neighbors[offset] = end;

        offset = offsets[end] + neighbors_offset[end]; // adjusting the index of neighbor in neighbors array
        neighbors[offset] = begin;

        neighbors_offset[begin] += 1;
        neighbors_offset[end] += 1;

    }

    std::cout << "Total Vertices Count : " << vertices_count << "  Total Edge Count : " << (edges_count * 2) << std::endl;

    input_file.close();
}


void Graph::transformToAugmentedGraph(Graph* augmented_graph){

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
}



// Partitions the graph balancing wedge-workload and minimizing edge cuts.
void partitionWithDegreeAndCut(Graph* graph, int& num_partitions, NodeID*& partition, int& num_iterations) {
    
    // 1. Calculate workload weights based on wedge counts
    std::vector<double> weights(graph->vertices_count);
    double total_weight = 0.0, degree;
    
    for (ui i = 0; i < graph->vertices_count; ++i) {
        degree = static_cast<double>(graph->degrees[i]);
        weights[i] = (degree * (degree - 1.0) / 2.0) + 1.0;
        total_weight += weights[i];
    }
    
    double target_weight = total_weight / num_partitions;
    
    
    std::vector<VertexID> sorted_vertices(graph->vertices_count);
    std::iota(sorted_vertices.begin(), sorted_vertices.end(), 0);
    std::sort(sorted_vertices.begin(), sorted_vertices.end(),
              [&weights](VertexID a, VertexID b) {
                  return weights[a] > weights[b];
              });

    // 3. Greedy Assignment (LDG Heuristic)
    std::vector<double> partition_weights(num_partitions, 0.0);

    NodeID best_p = 0, part_u;
    double best_score, load_ratio = 0.0, locality = 0.0, balance_penalty = 0.0, score;
    VertexID u;
    
    for (VertexID v : sorted_vertices) {
        degree = static_cast<double>(graph->degrees[v]);
        
        // Count how many neighbors of 'v' are already in each partition
        std::vector<double> neighbor_counts(num_partitions, 0.0);
        for (ui e = graph->offsets[v]; e < graph->offsets[v+1]; ++e) {
            u = graph->neighbors[e];
            part_u = partition[u];
            if (part_u != -1) {
                neighbor_counts[part_u] += 1.0;
            }
        }
        
        best_p = 0;
        best_score = -std::numeric_limits<double>::max();
        
        for (NodeID p = 0; p < num_partitions; ++p) {
            load_ratio = partition_weights[p] / target_weight;

            // Quadratic penalty strongly enforces the target weight boundary
            locality = neighbor_counts[p];
            balance_penalty = std::pow(load_ratio, 2.0) * (degree + 1.0);
            
            score = locality - balance_penalty;
            
            if (score > best_score) {
                best_score = score;
                best_p = p;
            }
        }
        
        // Assign to the winning partition
        partition[v] = best_p;
        partition_weights[best_p] += weights[v];
    }

}


void partitionWithDegreeAndCutForInterations(Graph* graph, int& num_partitions, NodeID*& partition, int& num_iterations) {
    
    // 1. Calculate workload weights based on wedge counts
    std::vector<double> weights(graph->vertices_count);
    double total_weight = 0.0, degree;
    
    for (ui i = 0; i < graph->vertices_count; ++i) {
        degree = static_cast<double>(graph->degrees[i]);
        weights[i] = (degree * (degree - 1.0) / 2.0) + 1.0;
        total_weight += weights[i];
    }
    
    double target_weight = total_weight / num_partitions;
    
    
    std::vector<VertexID> sorted_vertices(graph->vertices_count);
    std::iota(sorted_vertices.begin(), sorted_vertices.end(), 0);
    std::sort(sorted_vertices.begin(), sorted_vertices.end(),
              [&weights](VertexID a, VertexID b) {
                  return weights[a] > weights[b];
              });

    // 3. Greedy Assignment (LDG Heuristic)
    std::vector<double> partition_weights(num_partitions, 0.0);

    NodeID best_p = 0, part_u, prev_ptn;
    double best_score, load_ratio = 0.0, locality = 0.0, balance_penalty = 0.0, score;
    VertexID u;
    
    for (ui i = 0; i < num_iterations; i++){

        for (VertexID v : sorted_vertices) {
            prev_ptn = partition[v];
            degree = static_cast<double>(graph->degrees[v]);
            
            // Count how many neighbors of 'v' are already in each partition
            std::vector<double> neighbor_counts(num_partitions, 0.0);
            for (ui e = graph->offsets[v]; e < graph->offsets[v+1]; ++e) {
                u = graph->neighbors[e];
                part_u = partition[u];
                if (part_u != -1) {
                    neighbor_counts[part_u] += 1.0;
                }
            }
            
            best_p = 0;
            best_score = -std::numeric_limits<double>::max();
            
            
            for (NodeID p = 0; p < num_partitions; ++p) {
                load_ratio = partition_weights[p] / target_weight;

                // Quadratic penalty strongly enforces the target weight boundary
                locality = neighbor_counts[p];
                balance_penalty = std::pow(load_ratio, 2.0) * (degree + 1.0);
                
                score = locality - balance_penalty;
                
                if (score > best_score) {
                    best_score = score;
                    best_p = p;
                }
            }
            
            // Assign to the winning partition
            partition[v] = best_p;
            if(prev_ptn != best_p){
                partition_weights[best_p] += weights[v];
                if(prev_ptn != -1){
                    partition_weights[prev_ptn] -= weights[v];
                }
            }
            
        }

    }    

}



int main(int argc, char** argv){

    std::string input_graph_file = argv[1];
    std::string dataset_name = argv[2];
    std::string iteration_str = argv[3];

    int num_iterations = std::stoi(iteration_str);

    int numberOfParts[8] = {2, 4, 8, 16, 32, 64, 128, 256};
        
    Graph* graph = new Graph();
    Graph* augmented_graph = new Graph();

    graph->loadGraphFromFile(input_graph_file);
    graph->transformToAugmentedGraph(augmented_graph);

    

    NodeID* part = new NodeID[augmented_graph->vertices_count];

    for (int i = 0; i < 8; i++){

        int nParts = numberOfParts[i];

        std::cout << std::endl;
        std::cout << "Name of the dataset : " << input_graph_file << std::endl;
        std::cout << "Number of partitions : " << nParts << std::endl;
        std::cout << std::endl;

         
        for(ui j = 0; j < augmented_graph->vertices_count; j++){
            part[j] = -1;
        }

        //partitionWithDegreeAndCut(graph, nParts, part, num_iterations);
        partitionWithDegreeAndCutForInterations(augmented_graph, nParts, part, num_iterations);
          
        std::string final_output_file =  std::to_string(nParts) + "_i" + std::to_string(num_iterations) + "_" +  dataset_name;

        write_partitioned_vertices(augmented_graph->vertices_count, part, final_output_file);

        std::cout << "Written to the Output File" << std::endl;
        std::cout << "--------------------------" << std::endl;

    }

    delete[] part;  

    return 0;
}