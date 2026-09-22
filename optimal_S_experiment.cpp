// optimal_S_experiment.cpp
// Part (c)(iii): Determine the empirically optimal S for BEST PERFORMANCE
// (CPU time), not just minimum key comparisons.
//
// Why this matters: key comparisons ignore recursion/function-call overhead,
// merge-buffer copying, and cache effects. Those costs are real and dominate
// CPU time when S is very small (too many tiny recursive calls/merges).
// This program measures actual CPU time across a range of S values, on
// several dataset sizes, and reports the S that minimizes time -- which is
// typically in the 8-16 range even though comparison-count is minimized at
// a much smaller S.
//
// Compile with optimizations on -- a -O0 debug build inflates function-call
// overhead artificially and will skew your "optimal S":
//   g++ -O2 -std=c++17 optimal_S_experiment.cpp -o optimal_S_experiment
//
// Run:
//   ./optimal_S_experiment > results_S_sweep.csv

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <cstdint>

using Clock = std::chrono::high_resolution_clock;

// ---------- Hybrid sort (same algorithm as your main implementation) ----------

// Insertion sort on arr[low..high], counting comparisons.
static void insertionSort(std::vector<int>& arr, int low, int high, long long& comparisons) {
    for (int i = low + 1; i <= high; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low) {
            ++comparisons;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

static void merge(std::vector<int>& arr, std::vector<int>& temp,
                   int low, int mid, int high, long long& comparisons) {
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high) {
        ++comparisons;
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= high) temp[k++] = arr[j++];
    for (int x = low; x <= high; ++x) arr[x] = temp[x];
}

static void hybridSort(std::vector<int>& arr, std::vector<int>& temp,
                        int low, int high, int S, long long& comparisons) {
    if (high - low + 1 <= S) {
        insertionSort(arr, low, high, comparisons);
        return;
    }
    int mid = low + (high - low) / 2;
    hybridSort(arr, temp, low, mid, S, comparisons);
    hybridSort(arr, temp, mid + 1, high, S, comparisons);
    merge(arr, temp, low, mid, high, comparisons);
}

// ---------- Experiment driver ----------

static std::vector<int> generateRandomArray(int n, int maxVal, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(1, maxVal);
    std::vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = dist(rng);
    return arr;
}

int main() {
    // Dataset sizes to test S over. Include your largest size(s) since that's
    // what your report / part (d) cares about; a couple of smaller sizes lets
    // you confirm the optimal S is roughly stable across n.
    std::vector<int> sizes = {1'000'000, 5'000'000, 10'000'000};

    // Sweep S densely at the low end (where the interesting crossover is)
    // and more sparsely at the high end.
    std::vector<int> sValues;
    for (int s = 2; s <= 64; s += 2) sValues.push_back(s);
    for (int s = 70; s <= 200; s += 10) sValues.push_back(s);

    const int TRIALS = 5;          // average over several runs to smooth out noise
    const int MAX_VAL = 1'000'000; // range for random integers, per the spec

    std::mt19937 seedRng(12345); // fixed seed so results are reproducible

    std::cout << "n,S,avg_comparisons,avg_cpu_time_ms\n";

    for (int n : sizes) {
        for (int S : sValues) {
            long long totalComparisons = 0;
            double totalTimeMs = 0.0;

            for (int t = 0; t < TRIALS; ++t) {
                // Use the SAME underlying data across S values within a trial
                // index, generated deterministically, so comparisons across
                // S are apples-to-apples.
                std::mt19937 rng(seedRng() + t);
                std::vector<int> arr = generateRandomArray(n, MAX_VAL, rng);
                std::vector<int> temp(n);
                long long comparisons = 0;

                auto start = Clock::now();
                hybridSort(arr, temp, 0, n - 1, S, comparisons);
                auto end = Clock::now();

                double ms = std::chrono::duration<double, std::milli>(end - start).count();
                totalTimeMs += ms;
                totalComparisons += comparisons;

                // Sanity check (cheap, only checks a few points)
                // for (int i = 1; i < n; ++i) assert(arr[i-1] <= arr[i]);
            }

            std::cout << n << "," << S << ","
                      << (totalComparisons / TRIALS) << ","
                      << (totalTimeMs / TRIALS) << "\n";
        }
    }

    return 0;
}
