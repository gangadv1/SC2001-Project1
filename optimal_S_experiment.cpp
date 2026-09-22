// optimal_S_experiment.cpp
// Part (c)(iii): Determine the optimal threshold S
// based on average execution time of Hybrid Sort.

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>

using namespace std;

// --------------------------------------------------
// Insertion Sort
// Same [start, end) convention as sorts.cpp
// --------------------------------------------------

void insertionSort(vector<int>& A, int start, int end,
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

// --------------------------------------------------
// Merge
// --------------------------------------------------

void merge(vector<int>& A, int start, int mid, int end,
           vector<int>& buffer,
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

    while (i < mid) {
        buffer[k++] = A[i++];
    }

    while (j < end) {
        buffer[k++] = A[j++];
    }

    for (int i = start; i < end; i++) {
        A[i] = buffer[i];
    }
}

// --------------------------------------------------
// Hybrid Merge-Insertion Sort
// --------------------------------------------------

void hybridSort(vector<int>& A, int start, int end, int S,
                vector<int>& buffer,
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

// --------------------------------------------------
// Random dataset generator
// --------------------------------------------------

vector<int> generateRandomArray(int n, int maxValue,
                                mt19937& rng) {

    uniform_int_distribution<int> dist(1, maxValue);

    vector<int> data(n);

    for (int i = 0; i < n; i++) {
        data[i] = dist(rng);
    }

    return data;
}

// --------------------------------------------------
// Main C(iii) experiment
// --------------------------------------------------

int main() {

    // Different input sizes, as required by C(iii)
    vector<int> sizes = {
        100000,
        500000,
        1000000,
        5000000,
        10000000
    };

    // Reasonable range of thresholds to investigate
    vector<int> sValues = {
        2, 4, 8, 16, 32, 64, 128
    };

    // Repeat each experiment to reduce timing noise
    const int TRIALS = 5;

    // Same value range used in Part (b)
    const int MAX_VALUE = 10000000;

    // Fixed seed makes the experiment reproducible
    const unsigned int BASE_SEED = 12345;

    cout << fixed << setprecision(3);

    cout << "C(iii): Determining Optimal S\n";
    cout << "============================================\n";

    // Used to determine one overall S across input sizes
    vector<double> totalRelativeTime(sValues.size(), 0.0);

    for (int n : sizes) {

        cout << "\nn = " << n << "\n";
        cout << "--------------------------------------------\n";
        cout << "S,Average Key Comparisons,Average Time (ms)\n";

        // --------------------------------------------------
        // Generate the datasets ONCE.
        // Every S gets exactly the same datasets.
        // --------------------------------------------------

        vector<vector<int>> datasets;

        for (int trial = 0; trial < TRIALS; trial++) {

            mt19937 rng(BASE_SEED + trial);

            datasets.push_back(
                generateRandomArray(n, MAX_VALUE, rng)
            );
        }

        vector<double> timesForN;

        // --------------------------------------------------
        // Test each S
        // --------------------------------------------------

        for (int S : sValues) {

            unsigned long long totalComparisons = 0;
            double totalTime = 0.0;

            for (int trial = 0; trial < TRIALS; trial++) {

                // Same original input for every S
                vector<int> A = datasets[trial];

                vector<int> buffer(n);

                unsigned long long comparisons = 0;

                // Only time the sorting algorithm.
                // Dataset generation and copying are excluded.

                auto start =
                    chrono::high_resolution_clock::now();

                hybridSort(
                    A,
                    0,
                    n,
                    S,
                    buffer,
                    comparisons
                );

                auto end =
                    chrono::high_resolution_clock::now();

                double elapsed =
                    chrono::duration<double, milli>(
                        end - start
                    ).count();

                totalTime += elapsed;
                totalComparisons += comparisons;
            }

            double averageTime =
                totalTime / TRIALS;

            unsigned long long averageComparisons =
                totalComparisons / TRIALS;

            timesForN.push_back(averageTime);

            cout << S << ","
                 << averageComparisons << ","
                 << averageTime << "\n";
        }

        // --------------------------------------------------
        // Normalize the times for this n.
        //
        // This prevents the 10-million-element experiment
        // from dominating the smaller input sizes simply
        // because its absolute times are larger.
        // --------------------------------------------------

        double bestTime =
            *min_element(
                timesForN.begin(),
                timesForN.end()
            );

        int bestIndex =
            min_element(
                timesForN.begin(),
                timesForN.end()
            ) - timesForN.begin();

        cout << "\nFastest S for n = "
             << n
             << ": S = "
             << sValues[bestIndex]
             << "\n";

        for (size_t i = 0; i < sValues.size(); i++) {

            totalRelativeTime[i] +=
                timesForN[i] / bestTime;
        }
    }

    // --------------------------------------------------
    // Choose ONE S that performs well across all n.
    //
    // For every n:
    //     fastest S gets relative score 1.0
    //     slower S gets > 1.0
    //
    // Lowest average relative score = overall optimal S.
    // --------------------------------------------------

    int optimalIndex = 0;

    for (size_t i = 1; i < sValues.size(); i++) {

        if (totalRelativeTime[i] <
            totalRelativeTime[optimalIndex]) {

            optimalIndex = i;
        }
    }

    cout << "\n============================================\n";

    cout << "Overall Optimal S = "
         << sValues[optimalIndex]
         << "\n";

    cout << "============================================\n";

    return 0;
}
