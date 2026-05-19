#include <cstdio>
#include <ios>
#include <iostream>
#include <omp.h>
#include <mpi.h>

#include "parallel.h"

struct SystemConfig {
    long long total_numbers;
    int active_cores;
    bool exit_signal;
};

void clearscreen() {
    std::cout << "\033[2J\033[1;1H";
}

void show_menu(SystemConfig &config) {
    std::cout << "\n---- Program Menjumlahkan Bilangan Akar ----\n";
    std::cout << "Target N: " << config.total_numbers << "\n";
    std::cout << "Cores: " << config.active_cores << "\n";
    
    std::cout << "-- Menu --\n";
    std::cout << "1). Jumlahkan bilangan akar\n";
    std::cout << "2). Edit konfigurasi\n";
    std::cout << "3). Exit\n";

    std::cout << "Masukkan pilihan (1, 2, 3): ";

    int pilihan;
    std::cin >> pilihan;

    if (pilihan == 1) {
        config.exit_signal = false;

    } else if (pilihan == 2) {
        std::cout << "Enter target number count: ";
        std::cin >> config.total_numbers;
        
        std::cout << "Enter active cores count (OpenMP threads): ";
        std::cin >> config.active_cores;
        
        config.exit_signal = false;
        
    } else if (pilihan == 3) {
        config.exit_signal = true;
    }
}



int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int world_rank; 
    int world_size;
    int world_len;
    char current_node_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Get_processor_name( current_node_name, &world_len );
    current_node_name[world_len] = '\0';
    
    if (world_rank == 0) {
        printf("\n-- Containers count: %d\n", world_size);
        printf("Master node (rank %d) is running on container: %s\n", world_rank, current_node_name);

        // Master collects names from all other ranks
        char worker_node_name[MPI_MAX_PROCESSOR_NAME];
        for (int i = 1; i < world_size; i++) {
            MPI_Recv(worker_node_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Worker node (rank %d) is running on container: %s\n", i, worker_node_name);
        }
    } else {
        MPI_Send(current_node_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    SystemConfig current_config = SystemConfig{10000000000, 4, false};
    bool system_running = true;

    while (system_running == true) {
        int command = 0; // 0 = idle; 1 = execute; 2 = exit

        if (world_rank == 0) {
            show_menu(current_config);
            if (current_config.exit_signal == true) command = 2;
            else command = 1;
        }

        // 2. Broadcast the command and config from Master (0) to ALL workers
        MPI_Bcast(&command, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&current_config, sizeof(SystemConfig), MPI_BYTE, 0, MPI_COMM_WORLD);

        if (command == 1) {
            // Split the workload evenly among ACTIVE workers
            long long total = current_config.total_numbers;
            long long chunk = total / world_size;
            
            long long start_range = (world_rank * chunk) + 1;
            long long end_range = (world_rank == world_size - 1) ? total + 1 : start_range + chunk;
            
            double local_sum = compute_parallel_sqrt_sum(start_range, end_range, current_config.active_cores);
            double global_sum = 0;
            MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

            std::cout << "Container Rank " << world_rank << " computed range [" 
                          << start_range << " to " << (end_range - 1) 
                          << "] using " << current_config.active_cores << " cores. Local Sum: " << local_sum << "\n";

            if (world_rank == 0) {
                std::cout << "\n>>> TOTAL DISTRIBUTED SUM: " << std::fixed << global_sum << " <<<\n";
            }

            MPI_Barrier(MPI_COMM_WORLD);

        } else if (command == 2) {
            system_running = false;
            break;
        }
    }

    MPI_Finalize();
    return 0;
}