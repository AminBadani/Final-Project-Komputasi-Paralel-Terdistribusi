#include <chrono>
#include <cmath>
#include <iostream>

#include "parallel.h"

void test_local_accuracy() {
    long long test_start = 1, test_end = 5;
    int test_cores = 2;
    
    double expected_result = 1 + std::sqrt(2) + std::sqrt(3) + std::sqrt(4) + std::sqrt(5);
    double actual_result = compute_parallel_sqrt_sum(test_start, test_end, test_cores);
    
    // Check if the result is close enough (handling floating point precision)
    if (std::abs(actual_result - expected_result) < 1e-5) {
        std::cout << "[PASS] Test 1: Math accuracy verified [1 to 5]. Result: " << actual_result << "\n";
    } else {
        std::cout << "[FAIL] Test 1: Math variance detected [1 to 5]! Expected: " << expected_result << " Got: " << actual_result << "\n";
    }
}

void test_local_scalability() {
    long long bench_start = 1;
    long long bench_end = 50000000; // 500 million numbers
    
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
    if (zero_start_end == 0) {
        std::cout << "[PASS] Test 2: For range [0 to 0]. Result: " << zero_start_end << "\n";
    } else {
        std::cout << "[FAIL] Test 2: Invalid zero value [0 to 0]. Expected: 0 Got: " << zero_start_end << "\n";
    }

    double negative_start_end = compute_parallel_sqrt_sum(-10, -5, 1);
    if (negative_start_end == 0) {
        std::cout << "[PASS] Test 3: For range [-10 to -5]. Result: " << zero_start_end << "\n";
    } else {
        std::cout << "[FAIL] Test 3: Invalid negative value [-10 to -5]. Expected: 0 Got: " << negative_start_end << "\n";
    }
    
    double less_start_end = compute_parallel_sqrt_sum(10, 5, 1);
    if (less_start_end == 0) {
        std::cout << "[PASS] Test 4: For range [10 to 5]. Result: " << less_start_end << "\n";
    } else {
        std::cout << "[FAIL] Test 4: Invalid start less than end [10 to 5]. Expected: 0 Got: " << less_start_end << "\n";
    }

    double zero_cores_result = compute_parallel_sqrt_sum(6, 10, 0);
    double zero_cores_expected = std::sqrt(6) + std::sqrt(7) + std::sqrt(8) + std::sqrt(9) + std::sqrt(10);
    if (std::abs(zero_cores_result - zero_cores_expected) < 1e-5) {
        std::cout << "[PASS] Test 5: No active cores, change to serial (cores = 1). Result: " << zero_cores_expected << "\n";
    } else {
        std::cout << "[FAIL] Test 5: Invalid zero cores, expected change to serial (cores = 1). Expected: " << zero_cores_expected << " Got: " << zero_cores_result << "\n";
    }

    double over_cores_result = compute_parallel_sqrt_sum(6, 10, 12);
    double over_cores_expected = std::sqrt(6) + std::sqrt(7) + std::sqrt(8) + std::sqrt(9) + std::sqrt(10);
    if (std::abs(over_cores_result - over_cores_expected) < 1e-5) {
        std::cout << "[PASS] Test 6: Use 12 cores, change to limit (cores = 8). Result: " << over_cores_result << "\n";
    } else {
        std::cout << "[FAIL] Test 6: Invalid 12 cores, expected change to limit (cores = 8). Expected: " << over_cores_expected << " Got: " << over_cores_result << "\n";
    }
}

int main() {
    test_local_accuracy();
    test_local_edge_case();
    test_local_scalability();
    return 0;
}