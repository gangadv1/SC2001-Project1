#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <chrono>
#include "sorts.cpp"

using namespace std;
using namespace std::chrono;

vector<int> generateRandomArray(int n, int x, mt19937& rng) {
    uniform_int_distribution<int> dist(1, x);

    vector<int> data(n);
    for (int i = 0; i < n; i++) {
        data[i] = dist(rng);
    }
    return data;
}

int main() {
    const int x = 10000000;

    // Tested input sizes spanning multiple orders of magnitude
    vector<int> sizes = {
        100000,
        500000,
        1000000,
        5000000,
        10000000
    };

    // An open, broad range of S values without pre-assuming the minimum
    // Covers 1 (standard merge sort baseline) through granular increments up to large values
    vector<int> S_values = {
        1, 2, 4, 6, 8, 10, 12, 14, 16, 20, 24, 32, 48, 64, 96, 128, 256
    };

    mt19937 rng(12345);

    cout << "C(iii): Study to Determine Optimal S" << endl;
    cout << "n,S,Key Comparisons,Time (ms)" << endl;

    for (int n : sizes) {
        vector<int> originalData = generateRandomArray(n, x, rng);
        vector<int> buffer(n); // Pre-allocated once to ensure memory overhead does not bias timing

        int bestS_comp = 0;
        unsigned long long minComparisons = ULLONG_MAX;

        int bestS_time = 0;
        double minTimeMs = 1e18;

        for (int S : S_values) {
            // Fresh clone of identical dataset for every tested S
            vector<int> data = originalData;
            unsigned long long comparisons = 0;

            auto start = high_resolution_clock::now();
            hybridSort(data, 0, n, S, buffer, comparisons);
            auto end = high_resolution_clock::now();

            double durationMs = duration_cast<nanoseconds>(end - start).count() / 1e6;

            cout << n << "," << S << "," << comparisons << "," << durationMs << endl;

            if (comparisons < minComparisons) {
                minComparisons = comparisons;
                bestS_comp = S;
            }

            if (durationMs < minTimeMs) {
                minTimeMs = durationMs;
                bestS_time = S;
            }
        }

        cout << ">>> Empirical Minimums for n = " << n << ":" << endl;
        cout << "    Optimal S (Key Comparisons): S = " << bestS_comp 
             << " (" << minComparisons << " comparisons)" << endl;
        cout << "    Optimal S (CPU Time):        S = " << bestS_time 
             << " (" << minTimeMs << " ms)" << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    return 0;
}
