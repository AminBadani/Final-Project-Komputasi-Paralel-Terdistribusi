#include <exception>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <omp.h>

void clearscreen() {
    std::cout << "\033[2J\033[1;1H";
}

int main(int argc, char** argv) {
    std::cin.exceptions(std::ios::failbit | std::ios::badbit);

    bool exit = false;
    int count = 10;
    int cores = 1;
    int workers = 2;

    try {
        if (argc > 1) count = std::atoi(argv[1]);
        if (argc > 2) cores = std::atoi(argv[2]) > 4 ? 4 : std::atoi(argv[2]);
        if (argc > 2) workers = std::atoi(argv[3]) > 2 ? 2 : std::atoi(argv[3]);
    } catch (const std::invalid_argument&) {
        std::cerr << "Invalid integer argument\n";
        return 0;
    }

    while (exit == false) {
        char pilihan = '\0';
        
        std::cout << "\n---- Program Menjumlahkan Bilangan Akar ----\n";
        std::cout << "Count = " << count << "\n";
        std::cout << "Cores = " << cores << "\n";
        std::cout << "Workers = " << workers << "\n";
        std::cout << "-- Menu --\n";
        std::cout << "1). Jumlahkan bilangan akar\n";
        std::cout << "2). Edit konfigurasi\n";
        std::cout << "3). Exit\n";

        std::cout << "Masukkan pilihan (1, 2, 3): ";
        std::cin >> pilihan;

        try {
            if (pilihan == '1') {
                #pragma omp parallel for num_threads(cores)
                for (int i = 0; i < count; i++) {
                    int thread_num = omp_get_thread_num();
                    std::cout << "From: " << thread_num << " - Hello world " << i << "\n";
                }

            } else if (pilihan == '2') {
                std::cout << "Masukkan count (saat ini: " <<  count << ") = "; std::cin >> count;
                std::cout << "Masukkan cores (saat ini: " <<  cores << ") = "; std::cin >> cores;
                std::cout << "Masukkan workers (saat ini: " <<  workers << ") = "; std::cin >> workers;

            } else if (pilihan == '3') {
                std::cout << "Exit\n";
                exit = true;

            } else {
                std::cout << "Pilihan tidak ada\n";
                continue;
            }

        } catch (const std::exception& e) {
            std::cin.clear();
            std::string junk;
            std::getline(std::cin, junk);
            std::cout << "Terjadi error '" << e.what() << "'\n";
            continue;
        }
    }

    return 1;
}