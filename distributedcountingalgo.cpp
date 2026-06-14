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

        clock_t read_begin_clock = clock();
        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, world_rank);

        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, world_rank);
        double input_read_time = (double(clock() - read_begin_clock)) / CLOCKS_PER_SEC;  

        clock_t transformation_begin_clock = clock();
        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);
        double transformation_time = (double(clock() - transformation_begin_clock)) / CLOCKS_PER_SEC;  
      

        clock_t counting_begin_clock = clock();
        local_square_count = CountingAlgorithm::db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = CountingAlgorithm::count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = CountingAlgorithm::db_count_square_in_interface_graph(interface_graph);
        double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

        //std::cout << "Partition - " << world_rank << " : Local Square Count - " << local_square_count << std::endl;
        //std::cout << "Partition - " << world_rank << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        //std::cout << "Partition - " << world_rank << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        ptn_square_count += local_square_count;
        ptn_square_count += local_cut_edge_square_count;
        ptn_square_count += local_interface_square_count;

        global_square_count += ptn_square_count;

        double total_time = input_read_time + transformation_time + counting_time;   

        std::cout << "=================================================================" << std::endl;
        std::cout << "Partition : " << world_rank << " Partition Square Count : " << ptn_square_count << std::endl;         
        std::cout << "Partition : " << world_rank << " Input File Reading Time : " << input_read_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Transformation Time : " << transformation_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Counting Time : " << counting_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Total Time : " <<  total_time <<  " seconds" << std::endl;
        std::cout << "=================================================================" << std::endl;

        clock_t comm_recv_begin_clock = clock();
        for (ui i = 1; i < world_size; i++){
            MPI_Recv(&other_ptn_square_count, 1, MPI_LONG_LONG, i, TAG_COUNT, MPI_COMM_WORLD, &count_recv_status);
            global_square_count += other_ptn_square_count;
        }
        double comm_recv_time = (double(clock() - comm_recv_begin_clock)) / CLOCKS_PER_SEC;
        double counting_comm_time = counting_time + comm_recv_time;

        std::cout << "===================================================================" << std::endl;
        std::cout << "Input Graph File : " << file_path << std::endl; 
        std::cout << "Partition Count : " << partition_cnt << std::endl;
        std::cout << "Communication Receive Time : " << comm_recv_time << std::endl;
        std::cout << "Counting & Communication Receive Time : " << counting_comm_time << std::endl;
        std::cout << "Entire Time : " << (total_time + comm_recv_time) << std::endl;
        std::cout << "Global Square Count : " << global_square_count << std::endl;
        std::cout << "===================================================================" << std::endl;

    } else if (world_rank == (world_size - 1)) {

        clock_t read_begin_clock = clock();
        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, world_rank);
        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, world_rank);
        double input_read_time = (double(clock() - read_begin_clock)) / CLOCKS_PER_SEC; 

        clock_t transformation_begin_clock = clock();
        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);
        double transformation_time = (double(clock() - transformation_begin_clock)) / CLOCKS_PER_SEC;  
         
        clock_t counting_begin_clock = clock();
        local_square_count = CountingAlgorithm::db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = CountingAlgorithm::count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = CountingAlgorithm::db_count_square_in_interface_graph(interface_graph);
        double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

        //std::cout << "Partition - " << world_rank << " : Local Square Count - " << local_square_count << std::endl;
        //std::cout << "Partition - " << world_rank << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        //std::cout << "Partition - " << world_rank << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        ptn_square_count += local_square_count;
        ptn_square_count += local_cut_edge_square_count;
        ptn_square_count += local_interface_square_count;

        double total_time = input_read_time + transformation_time + counting_time;   

        std::cout << "=================================================================" << std::endl;
        std::cout << "Partition : " << world_rank << " Partition Square Count : " << ptn_square_count << std::endl;         
        std::cout << "Partition : " << world_rank << " Input File Reading Time : " << input_read_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Transformation Time : " << transformation_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Counting Time : " << counting_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Total Time : " <<  total_time <<  " seconds" << std::endl;
        std::cout << "=================================================================" << std::endl;


        read_begin_clock = clock();
        Graph* cut_graph = new Graph();
        cut_graph->loadCutGraphFromFile(file_path, vertex_partition_file_path);
        input_read_time = (double(clock() - read_begin_clock)) / CLOCKS_PER_SEC;

        transformation_begin_clock = clock();
        Graph* transformed_cut_graph = new Graph();
        cut_graph->transformToAugmentedGraph(transformed_cut_graph);

        counting_begin_clock = clock();
        cut_graph_square_count = CountingAlgorithm::db_count_square_in_cut_graph(transformed_cut_graph);
        counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

        total_time = input_read_time + transformation_time + counting_time;

        std::cout << "=================================================================" << std::endl;
        std::cout << "Partition : " << world_rank << " Cut Graph Square Count : " << ptn_square_count << std::endl;         
        std::cout << "Partition : " << world_rank << " Cut Graph Input File Reading Time : " << input_read_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Cut Graph Transformation Time : " << transformation_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Cut Graph Counting Time : " << counting_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Cut Graph Total Time : " <<  total_time <<  " seconds" << std::endl;
        std::cout << "=================================================================" << std::endl;
        
        
        ptn_square_count += cut_graph_square_count;

        MPI_Send(&ptn_square_count, 1, MPI_LONG_LONG, dest_rank, TAG_COUNT, MPI_COMM_WORLD);

        // std::cout << "==============================================" << std::endl;
        // std::cout << "Partition : " << world_rank << " Partition Square Count : " << ptn_square_count << std::endl;
        // std::cout << "==============================================" << std::endl;

    } else {        

        clock_t read_begin_clock = clock();
        Graph* local_graph = new Graph();
        local_graph->loadPartitionedLocalGraphWoCutEdgesFromFile(file_path, vertex_partition_file_path, world_rank);
        Graph* interface_graph = new Graph();
        interface_graph->loadPartitionedInterfaceGraphFromFile(file_path, vertex_partition_file_path, world_rank);
        double input_read_time = (double(clock() - read_begin_clock)) / CLOCKS_PER_SEC; 

        clock_t transformation_begin_clock = clock();
        Graph* local_augmented_graph = new Graph();
        local_graph->transformToAugmentedGraph(local_augmented_graph);
        double transformation_time = (double(clock() - transformation_begin_clock)) / CLOCKS_PER_SEC; 
         
        clock_t counting_begin_clock = clock();
        local_square_count = CountingAlgorithm::db_count_square_in_local_graph(local_augmented_graph);
        local_cut_edge_square_count = CountingAlgorithm::count_square_from_other_ptn_per_vertex(local_graph);
        local_interface_square_count = CountingAlgorithm::db_count_square_in_interface_graph(interface_graph);
        double counting_time = (double(clock() - counting_begin_clock)) / CLOCKS_PER_SEC;

        double total_time = input_read_time + transformation_time + counting_time; 

        // std::cout << "Partition - " << world_rank << " : Local Square Count - " << local_square_count << std::endl;
        // std::cout << "Partition - " << world_rank << " : Local Cut Edge Square Count - " << local_cut_edge_square_count << std::endl;
        // std::cout << "Partition - " << world_rank << " : Local Interface Square Count - " << local_interface_square_count << std::endl;

        ptn_square_count += local_square_count;
        ptn_square_count += local_cut_edge_square_count;
        ptn_square_count += local_interface_square_count;

        MPI_Send(&ptn_square_count, 1, MPI_LONG_LONG, dest_rank, TAG_COUNT, MPI_COMM_WORLD);

        std::cout << "=================================================================" << std::endl;
        std::cout << "Partition : " << world_rank << " Partition Square Count : " << ptn_square_count << std::endl;         
        std::cout << "Partition : " << world_rank << " Input File Reading Time : " << input_read_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Transformation Time : " << transformation_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Counting Time : " << counting_time << " seconds" << std::endl;
        std::cout << "Partition : " << world_rank << " Total Time : " <<  total_time <<  " seconds" << std::endl;
        std::cout << "=================================================================" << std::endl;
    }

}