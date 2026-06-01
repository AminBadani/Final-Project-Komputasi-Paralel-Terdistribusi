#pragma once

#include <cmath>
#include <omp.h>

inline double compute_parallel_sqrt_sum(long long start_range, long long end_range, int active_cores) {
    double sum = 0;

    #pragma omp parallel for reduction(+:sum) num_threads(active_cores)
    for (long long i = start_range; i <= end_range; i++) {
        sum += std::sqrt(i);
    }

    return sum;
}