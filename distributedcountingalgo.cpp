#include "countingalgo.h"
#include "distributedcountingalgo.h"
#include <iostream>
#include <mpi.h>


void DistributedCountingAlgorithm::optimized_db_count_square_in_whole_ptn_graph_parallel(const std::string& file_path, const std::string& vertex_partition_file_path, int partition_cnt){
  
    int world_size, world_rank, dest_rank = 0;
    int buffer_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    long long global_square_count = 0, ptn_square_count = 0, other_ptn_square_count = 0;
    long long local_square_count = 0, local_cut_edge_square_count = 0, local_interface_square_count = 0, cut_graph_square_count = 0;

    MPI_Status count_recv_status;

    if(world_rank == 0){

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, world_rank);

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, world_rank); 

        local_square_count = CountingAlgorithm::db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = CountingAlgorithm::count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = CountingAlgorithm::db_count_square_in_interface_graph(interface_graph);

        std::cout << "Partition - " << world_rank << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << world_rank << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << world_rank << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        ptn_square_count += local_square_count;
        ptn_square_count += local_cut_edge_square_count;
        ptn_square_count += local_interface_square_count;

        global_square_count += ptn_square_count;

        std::cout << "==============================================" << std::endl;
        std::cout << "Partition : " << world_rank << " Partition Square Count : " << ptn_square_count << std::endl;
        std::cout << "==============================================" << std::endl;

        for (ui i = 1; i < world_size; i++){
            MPI_Recv(&other_ptn_square_count, 1, MPI_LONG_LONG, i, TAG_COUNT, MPI_COMM_WORLD, &count_recv_status);
            global_square_count += other_ptn_square_count;
        }

        std::cout << "==============================================" << std::endl;
        std::cout << "Partition Count : " << partition_cnt << std::endl;
        std::cout << "Global Square Count : " << global_square_count << std::endl;
        std::cout << "==============================================" << std::endl;

    } else if (world_rank == (world_size - 1)) {

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, world_rank);

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, world_rank); 

        local_square_count = CountingAlgorithm::db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = CountingAlgorithm::count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = CountingAlgorithm::db_count_square_in_interface_graph(interface_graph);

        std::cout << "Partition - " << world_rank << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << world_rank << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << world_rank << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        ptn_square_count += local_square_count;
        ptn_square_count += local_cut_edge_square_count;
        ptn_square_count += local_interface_square_count;

        Graph* cut_graph = new Graph();
        cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);

        Graph* transformed_cut_graph = new Graph();
        cut_graph->transformToAugmentedGraph(transformed_cut_graph);

        cut_graph_square_count = CountingAlgorithm::db_count_square_in_cut_graph(transformed_cut_graph);
        std::cout << "Global Cut Graph Square Count : " << cut_graph_square_count << std::endl;
        
        ptn_square_count += cut_graph_square_count;

        MPI_Send(&ptn_square_count, 1, MPI_LONG_LONG, dest_rank, TAG_COUNT, MPI_COMM_WORLD);

        std::cout << "==============================================" << std::endl;
        std::cout << "Partition : " << world_rank << " Partition Square Count : " << ptn_square_count << std::endl;
        std::cout << "==============================================" << std::endl;

    } else {        

        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, world_rank);

        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, world_rank); 

        local_square_count = CountingAlgorithm::db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = CountingAlgorithm::count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = CountingAlgorithm::db_count_square_in_interface_graph(interface_graph);

        std::cout << "Partition - " << world_rank << " : Local Square Count - " << local_square_count << std::endl;
        std::cout << "Partition - " << world_rank << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        std::cout << "Partition - " << world_rank << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        ptn_square_count += local_square_count;
        ptn_square_count += local_cut_edge_square_count;
        ptn_square_count += local_interface_square_count;

        MPI_Send(&ptn_square_count, 1, MPI_LONG_LONG, dest_rank, TAG_COUNT, MPI_COMM_WORLD);

        std::cout << "==============================================" << std::endl;
        std::cout << "Partition : " << world_rank << " Partition Square Count : " << ptn_square_count << std::endl;
        std::cout << "==============================================" << std::endl;
    }

}