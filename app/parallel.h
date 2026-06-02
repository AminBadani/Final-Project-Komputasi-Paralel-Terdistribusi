#pragma once

#include <cmath>
#include <omp.h>

inline double compute_parallel_sqrt_sum(long long start_range, long long end_range, int active_cores) {
    if (start_range > end_range || start_range < 0 || end_range < 0) return 0;

    double sum = 0;
    int used_cores = 1;
    if (active_cores > 0 && active_cores <= 8) {
        used_cores = active_cores;
    } else if (active_cores > 8) {
        used_cores = 8;
    }

    #pragma omp parallel for reduction(+:sum) num_threads(used_cores)
    for (long long i = start_range; i <= end_range; i++) {
        sum += std::sqrt(i);
    }
    return sum;
}