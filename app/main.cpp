#include <cstdio>
#include <ios>
#include <iostream>
#include <omp.h>
#include <mpi.h>
#include <string>
#include <chrono>

#include "parallel.h"

struct SystemConfig {
    long long total_numbers = 1000000000;
    int active_cores = 4;
};

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int world_rank; 
    int world_size;
    int world_len;
    char current_node_name[MPI_MAX_PROCESSOR_NAME];
    SystemConfig current_config;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Get_processor_name( current_node_name, &world_len );
    current_node_name[world_len] = '\0';
    
    if (world_rank != 0) {
        // Send name from worker
        MPI_Send(current_node_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    } else {
        printf("\n-- Containers count: %d\n", world_size);
        printf("Master node (rank %d) is running on container: %s\n", world_rank, current_node_name);

        // Master collects names from all other ranks (from other workers)
        char worker_node_name[MPI_MAX_PROCESSOR_NAME];
        for (int i = 1; i < world_size; i++) {
            MPI_Recv(worker_node_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Worker node (rank %d) is running on container: %s\n", i, worker_node_name);
        }

        // Initialize config on master
        current_config = SystemConfig{
            .total_numbers=( argc > 1 ? std::stoll(argv[1]) : 1000000000 ), 
            .active_cores=( argc > 2 ? ( std::stoi(argv[2]) > 8 ? 8 : std::stoi(argv[2]) ) : 4 )
        };
    }

    // Start time 
    auto start_time = std::chrono::high_resolution_clock::now();

    // 2. Broadcast the config from Master (0) to ALL workers
    MPI_Bcast(&current_config, sizeof(SystemConfig), MPI_BYTE, 0, MPI_COMM_WORLD);

    // Split the workload evenly to workers
    long long chunk = current_config.total_numbers / world_size;
    long long start = (world_rank * chunk) + 1;
    long long end = (world_rank == world_size - 1) ? current_config.total_numbers : start + chunk - 1;

    // Process local sum on eachy workers
    double local_sum = compute_parallel_sqrt_sum(start, end, current_config.active_cores);
    if (world_rank != 0) {
        long long local_range[2] = {start, end};
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        MPI_Send(local_range, 2, MPI_LONG_LONG, 0, 2, MPI_COMM_WORLD);
    }

    // Reduce to Master
    double global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // End time
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    if (world_rank == 0) {
        // Print from Master
        std::cout << "\nContainer Rank " << 0 << " computed range [" 
                << start << " to " << end
                << "] using " << current_config.active_cores << " cores. Local Sum: " << std::fixed << local_sum << "\n";

        // Send each workers result to Master
        double worker_sum;
        long long worker_range[2];
        for (int i = 1; i < world_size; i++) {
            MPI_Recv(&worker_sum, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(worker_range, 2, MPI_LONG_LONG, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            long long worker_start = worker_range[0];
            long long worker_end = worker_range[1];

            std::cout << "Container Rank " << i << " computed range [" 
                << worker_start << " to " << worker_end
                << "] using " << current_config.active_cores << " cores. Local Sum: " << std::fixed << worker_sum << "\n";
        }

        std::cout << "\n--- TOTAL DISTRIBUTED SUM: " << std::fixed << global_sum << "\n";
        std::cout << "--- TOTAL TIME:  " << duration.count() << " seconds.\n";
    }

    MPI_Finalize();
    return 0;
}