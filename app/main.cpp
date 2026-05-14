#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <omp.h>

int main(int argc, char** argv) {
    int count = 10;
    int cores = 1;

    try {
        if (argc > 1) count = std::atoi(argv[1]);
        if (argc > 2) cores = std::atoi(argv[2]) > 4 ? 4 : std::atoi(argv[2]);
    } catch (const std::invalid_argument&) {
        std::cerr << "Invalid integer argument\n";
        return 0;
    }

    #pragma omp parallel for num_threads(cores)
    for (int i = 0; i < count; i++) {
        int thread_num = omp_get_thread_num();
        std::cout << "From: " << thread_num << " - Hello world " << i << "\n";
    }

    return 1;
}