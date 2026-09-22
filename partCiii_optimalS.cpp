#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
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

    const int n = 10000000;
    const int x = 10000000;

    // Fine-grained search over S
    vector<int> S_values;

    for (int S = 2; S <= 64; S += 2) {
        S_values.push_back(S);
    }

    // More repetitions for reliable timing
    const int repetitions = 20;

    mt19937 rng(12345);

    // Generate ONE dataset.
    // Every S is tested using exactly the same data.
    vector<int> originalData =
        generateRandomArray(n, x, rng);

    cout << fixed << setprecision(3);

    cout << "Fine Search for Optimal S" << endl;
    cout << "n = " << n << endl;
    cout << "Repetitions per S = "
         << repetitions << endl << endl;

    cout << "S,Key Comparisons,Average Time (ms),Median Time (ms)"
         << endl;

    double bestMedian = 1e100;
    double bestAverage = 1e100;

    int bestMedianS = -1;
    int bestAverageS = -1;

    unsigned long long minimumComparisons =
        ULLONG_MAX;

    int minimumComparisonS = -1;

    for (int S : S_values) {

        vector<double> times;

        double totalTime = 0.0;

        unsigned long long comparisonsForS = 0;

        for (int run = 0; run < repetitions; run++) {

            // Restore identical unsorted input
            vector<int> data = originalData;

            // Allocate buffer BEFORE timing
            vector<int> buffer(n);

            unsigned long long comparisons = 0;

            auto start = steady_clock::now();

            hybridSort(
                data,
                0,
                n,
                S,
                buffer,
                comparisons
            );

            auto end = steady_clock::now();

            double elapsed =
                duration<double, milli>(
                    end - start
                ).count();

            times.push_back(elapsed);

            totalTime += elapsed;

            if (run == 0) {
                comparisonsForS = comparisons;
            }

            if (!isSorted(data)) {
                cerr << "ERROR: Array not sorted."
                     << " S = " << S << endl;

                return 1;
            }
        }

        // Calculate average
        double averageTime =
            totalTime / repetitions;

        // Calculate median
        sort(times.begin(), times.end());

        double medianTime;

        if (repetitions % 2 == 0) {

            medianTime =
                (times[repetitions / 2 - 1]
                + times[repetitions / 2]) / 2.0;

        } else {

            medianTime =
                times[repetitions / 2];
        }

        cout << S << ","
             << comparisonsForS << ","
             << averageTime << ","
             << medianTime << endl;

        // Track lowest average time
        if (averageTime < bestAverage) {

            bestAverage = averageTime;
            bestAverageS = S;
        }

        // Track lowest median time
        if (medianTime < bestMedian) {

            bestMedian = medianTime;
            bestMedianS = S;
        }

        // Track fewest key comparisons
        if (comparisonsForS < minimumComparisons) {

            minimumComparisons =
                comparisonsForS;

            minimumComparisonS = S;
        }
    }

    cout << endl;
    cout << "======================================" << endl;
    cout << "FINAL RESULTS" << endl;
    cout << "======================================" << endl;

    cout << "Fewest key comparisons:"
         << " S = " << minimumComparisonS
         << " (" << minimumComparisons << ")"
         << endl;

    cout << "Lowest average execution time:"
         << " S = " << bestAverageS
         << " (" << bestAverage << " ms)"
         << endl;

    cout << "Lowest median execution time:"
         << " S = " << bestMedianS
         << " (" << bestMedian << " ms)"
         << endl;

    cout << endl;

    cout << "Recommended optimal S = "
         << bestMedianS << endl;

    return 0;
}
