#ifndef MASTER_H    
#define MASTER_H    

#include <iostream>

struct SystemConfig {
    long long total_numbers;
    int active_cores;
    int active_workers;
    bool exit_signal;
};

inline void show_menu(SystemConfig &config) {
    std::cout << "\n---- Program Menjumlahkan Bilangan Akar ----\n";
    std::cout << "Target N: " << config.total_numbers << "\n";
    std::cout << "Cores: " << config.active_cores << "\n";
    std::cout << "Workers: " << config.active_workers << "\n";
    
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
        
        std::cout << "Enter active workers count: ";
        std::cin >> config.active_workers;
        
        config.exit_signal = false;
        
    } else if (pilihan == 3) {
        config.exit_signal = true;
    }
}

#endif // MASTER_H  
