// optimal_S_experiment.cpp
// Part (c)(iii): Determine optimal S based on execution time.
//
// For each input size:
// 1. Generate 5 fixed random datasets.
// 2. Test every S on the SAME datasets.
// 3. Measure Hybrid Sort execution time.
// 4. Record average key comparisons and average execution time.

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

using Clock = std::chrono::high_resolution_clock;

// ---------------- Insertion Sort ----------------

void insertionSort(std::vector<int>& A, int start, int end,
                   unsigned long long& comparisons) {

    for (int i = start + 1; i < end; i++) {
        int key = A[i];
        int j = i - 1;

        while (j >= start) {
            comparisons++;

            if (A[j] > key) {
                A[j + 1] = A[j];
                j--;
            } else {
                break;
            }
        }

        A[j + 1] = key;
    }
}

// ---------------- Merge ----------------

void merge(std::vector<int>& A, int start, int mid, int end,
           std::vector<int>& buffer,
           unsigned long long& comparisons) {

    int i = start;
    int j = mid;
    int k = start;

    while (i < mid && j < end) {
        comparisons++;

        if (A[i] <= A[j]) {
            buffer[k++] = A[i++];
        } else {
            buffer[k++] = A[j++];
        }
    }

    while (i < mid)
        buffer[k++] = A[i++];

    while (j < end)
        buffer[k++] = A[j++];

    for (int index = start; index < end; index++) {
        A[index] = buffer[index];
    }
}

// ---------------- Hybrid Sort ----------------

void hybridSort(std::vector<int>& A, int start, int end, int S,
                std::vector<int>& buffer,
                unsigned long long& comparisons) {

    if (end - start <= S) {
        insertionSort(A, start, end, comparisons);
        return;
    }

    int mid = start + (end - start) / 2;

    hybridSort(A, start, mid, S, buffer, comparisons);
    hybridSort(A, mid, end, S, buffer, comparisons);

    merge(A, start, mid, end, buffer, comparisons);
}

// ---------------- Generate Random Array ----------------

std::vector<int> generateRandomArray(int n, int maxValue,
                                     std::mt19937& rng) {

    std::uniform_int_distribution<int> dist(1, maxValue);

    std::vector<int> data(n);

    for (int i = 0; i < n; i++) {
        data[i] = dist(rng);
    }

    return data;
}

// ---------------- Main Experiment ----------------

int main() {

    // Different input sizes required for C(iii)
    std::vector<int> sizes = {
        1000000,
        5000000,
        10000000
    };

    // Candidate S values
    std::vector<int> sValues;

    for (int S = 2; S <= 64; S += 2) {
        sValues.push_back(S);
    }

    for (int S = 70; S <= 200; S += 10) {
        sValues.push_back(S);
    }

    const int DATASETS = 5;

    // Same maximum random value used in Part B
    const int MAX_VALUE = 10000000;

    // Fixed seed for reproducibility
    const unsigned int BASE_SEED = 12345;

    std::cout
        << "n,S,avg_comparisons,avg_execution_time_ms\n";

    for (int n : sizes) {

        // ------------------------------------------------
        // Generate the datasets ONCE for this n.
        //
        // Every S below will therefore be tested on
        // exactly the same five arrays.
        // ------------------------------------------------

        std::vector<std::vector<int>> datasets;

        datasets.reserve(DATASETS);

        for (int d = 0; d < DATASETS; d++) {

            std::mt19937 rng(BASE_SEED + d);

            datasets.push_back(
                generateRandomArray(n, MAX_VALUE, rng)
            );
        }

        // ------------------------------------------------
        // Test every S
        // ------------------------------------------------

        for (int S : sValues) {

            unsigned long long totalComparisons = 0;

            double totalTimeMs = 0.0;

            for (int d = 0; d < DATASETS; d++) {

                // Copy the SAME original dataset.
                // Copying happens BEFORE timing starts,
                // so it is not included in sorting time.

                std::vector<int> A = datasets[d];

                std::vector<int> buffer(n);

                unsigned long long comparisons = 0;

                // -------- Start timer --------

                auto start = Clock::now();

                hybridSort(
                    A,
                    0,
                    n,
                    S,
                    buffer,
                    comparisons
                );

                auto end = Clock::now();

                // -------- End timer --------

                double elapsedMs =
                    std::chrono::duration<double, std::milli>(
                        end - start
                    ).count();

                totalTimeMs += elapsedMs;

                totalComparisons += comparisons;
            }

            double averageTime =
                totalTimeMs / DATASETS;

            unsigned long long averageComparisons =
                totalComparisons / DATASETS;

            std::cout
                << n << ","
                << S << ","
                << averageComparisons << ","
                << averageTime
                << "\n";
        }
    }

    return 0;
}
