#include <cstdio>
#include <iostream>
#include <omp.h>
#include <mpi.h>

#include "master.h"

void clearscreen() {
    std::cout << "\033[2J\033[1;1H";
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
    
    SystemConfig config = SystemConfig{100, 4, world_size, false};

    bool system_running = true;
    
    while (system_running == true) {
        int command = 0; // 0 = idle; 1 = execute; 2 = exit

        if (world_rank == 0) {
            // Master collects names from all other ranks
            char worker_node_name[MPI_MAX_PROCESSOR_NAME];
            for (int i = 1; i < world_size; i++) {
                MPI_Recv(worker_node_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Rank %d is running on container: %s\n", i, worker_node_name);
            }

            show_menu(config);
            if (config.exit_signal == true) command = 2;
            else command = 1; 

        } else {
            MPI_Send(current_node_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }

        // 2. Broadcast the command and config from Master (0) to ALL workers
        MPI_Bcast(&command, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&config, sizeof(SystemConfig), MPI_BYTE, 0, MPI_COMM_WORLD);

        if (command == 1) {
            continue;

        } else if (command == 2) {
            system_running = false;
            break;
        }
    }

    MPI_Finalize();
    return 0;
}