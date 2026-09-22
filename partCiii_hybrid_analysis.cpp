#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>
#include <limits>
#include "sorts.cpp"

using namespace std;

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
    const int trials = 5;

    vector<int> sizes = {
        100000,
        500000,
        1000000,
        5000000,
        10000000
    };

    // Focus on the practical small-threshold region.
    vector<int> S_values = {
        2, 4, 8, 12, 16, 20, 24, 28,
        32, 36, 40, 44, 48, 52, 56, 60, 64
    };

    mt19937 rng(12345);

    cout << fixed << setprecision(6);

    cout << "C(iii): Hybrid Sort performance across S" << endl;
    cout << "Trials per configuration = " << trials << endl;
    cout << endl;

    cout << "n,S,Average Key Comparisons,Average CPU Time (s)"
         << endl;

    for (int n : sizes) {
        double bestTime = numeric_limits<double>::max();
        int bestS = 0;

        for (int S : S_values) {
            unsigned long long totalComparisons = 0;
            double totalCPUTime = 0.0;

            /*
             * Reset the RNG for every S.
             *
             * This ensures that S=2, S=4, S=8, etc. are all
             * tested on exactly the same sequence of arrays.
             */
            mt19937 testRng(12345 + n);

            for (int trial = 0; trial < trials; trial++) {
                vector<int> originalData =
                    generateRandomArray(n, x, testRng);

                vector<int> data = originalData;
                vector<int> buffer(n);

                unsigned long long comparisons = 0;

                clock_t start = clock();

                hybridSort(
                    data,
                    0,
                    n,
                    S,
                    buffer,
                    comparisons
                );

                clock_t end = clock();

                double cpuTime =
                    static_cast<double>(end - start) /
                    CLOCKS_PER_SEC;

                totalComparisons += comparisons;
                totalCPUTime += cpuTime;
            }

            double averageComparisons =
                static_cast<double>(totalComparisons) / trials;

            double averageCPUTime =
                totalCPUTime / trials;

            cout << n << ","
                 << S << ","
                 << averageComparisons << ","
                 << averageCPUTime << endl;

            if (averageCPUTime < bestTime) {
                bestTime = averageCPUTime;
                bestS = S;
            }
        }

        cout << "Best for n=" << n
             << ": S=" << bestS
             << ", Average CPU Time="
             << bestTime << " s"
             << endl;

        cout << endl;
    }

    return 0;
}
