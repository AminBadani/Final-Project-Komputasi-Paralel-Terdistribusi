#include <chrono>
#include <iostream>

#include "parallel.h"

void test_local_accuracy() {
    long long test_start = 1;
    long long test_end = 5; // reminder: loop stops before end_range
    int test_cores = 2;
    
    double expected_result = 1.0 + std::sqrt(2) + std::sqrt(3) + std::sqrt(4);
    double actual_result = compute_parallel_sqrt_sum(test_start, test_end, test_cores);
    
    // Check if the result is close enough (handling floating point precision)
    if (std::abs(actual_result - expected_result) < 1e-5) {
        std::cout << "[PASS] Test 1: Math accuracy verified. Result: " << actual_result << "\n";
    } else {
        std::cout << "[FAIL] Test 1: Math variance detected! Expected: " << expected_result << " Got: " << actual_result << "\n";
    }
}

void test_local_scalability() {
    long long bench_start = 1;
    long long bench_end = 500000000; // 500 million numbers
    
    // Benchmark 1 Core
    auto start_time = std::chrono::high_resolution_clock::now();
    compute_parallel_sqrt_sum(bench_start, bench_end, 1);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_1_core = end_time - start_time;
    std::cout << "[BENCH] Time with 1 Core:  " << duration_1_core.count() << " seconds.\n";

    // Benchmark 4 Cores
    start_time = std::chrono::high_resolution_clock::now();
    compute_parallel_sqrt_sum(bench_start, bench_end, 4);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_4_cores = end_time - start_time;
    std::cout << "[BENCH] Time with 4 Cores: " << duration_4_cores.count() << " seconds.\n";

    // Calculate Speedup Efficiency metric
    double speedup = duration_1_core.count() / duration_4_cores.count();
    std::cout << "[RESULT] Speedup Factor: " << speedup << "x\n";
    std::cout << "===============================\n\n";
}

void test_local_edge_case() {
    double zero_start_end = compute_parallel_sqrt_sum(0, 0, 1);
    double negative_start_end = compute_parallel_sqrt_sum(-10, -5, 1);
    double less_start_end = compute_parallel_sqrt_sum(10, 5, 1);

    double zero_active_cores = compute_parallel_sqrt_sum(10, 5, 0);
}

int main() {
    test_local_accuracy();
    test_local_scalability();
    return 0;
}