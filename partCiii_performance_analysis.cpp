#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <climits>
#include <iomanip>
#include "sorts.cpp"

using namespace std;
using namespace chrono;

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

    // Different input sizes required for C(iii)
    vector<int> sizes = {
        100000,
        500000,
        1000000,
        5000000,
        10000000
    };

    // More detailed search around the expected useful threshold region
    vector<int> S_values = {
        2, 4, 6, 8, 10, 12, 14, 16,
        20, 24, 32, 48, 64
    };

    // Repeat each timing to reduce CPU timing noise
    const int repetitions = 5;

    mt19937 rng(12345);

    cout << fixed << setprecision(3);

    cout << "C(iii): Determining optimal S" << endl;
    cout << "Each configuration is timed "
         << repetitions << " times." << endl << endl;

    for (int n : sizes) {

        // Generate ONE original dataset for this n.
        // Every S gets exactly the same input data.
        vector<int> originalData =
            generateRandomArray(n, x, rng);

        double bestTime = 1e100;
        int bestTimeS = -1;

        unsigned long long bestComparisons = ULLONG_MAX;
        int bestComparisonS = -1;

        cout << "========================================" << endl;
        cout << "n = " << n << endl;
        cout << "S,Key Comparisons,Average CPU Time (ms)" << endl;

        for (int S : S_values) {

            double totalTime = 0.0;
            unsigned long long comparisonsForS = 0;

            for (int run = 0; run < repetitions; run++) {

                // Restore identical unsorted input before every run
                vector<int> data = originalData;
                vector<int> buffer(n);

                unsigned long long comparisons = 0;

                auto start = high_resolution_clock::now();

                hybridSort(
                    data,
                    0,
                    n,
                    S,
                    buffer,
                    comparisons
                );

                auto end = high_resolution_clock::now();

                double elapsed =
                    duration<double, milli>(end - start).count();

                totalTime += elapsed;

                // Comparison count should be identical across
                // repetitions because the input is identical.
                if (run == 0) {
                    comparisonsForS = comparisons;
                }

                if (!isSorted(data)) {
                    cerr << "ERROR: Array not sorted!"
                         << " n=" << n
                         << " S=" << S << endl;
                    return 1;
                }
            }

            double averageTime =
                totalTime / repetitions;

            cout << S << ","
                 << comparisonsForS << ","
                 << averageTime << endl;

            // Best S according to CPU time
            if (averageTime < bestTime) {
                bestTime = averageTime;
                bestTimeS = S;
            }

            // Best S according to key comparisons
            if (comparisonsForS < bestComparisons) {
                bestComparisons = comparisonsForS;
                bestComparisonS = S;
            }
        }

        cout << endl;

        cout << "Fewest key comparisons:"
             << " S = " << bestComparisonS
             << " (" << bestComparisons
             << " comparisons)" << endl;

        cout << "Fastest average CPU time:"
             << " S = " << bestTimeS
             << " (" << bestTime
             << " ms)" << endl;

        cout << endl;
    }

    return 0;
}
