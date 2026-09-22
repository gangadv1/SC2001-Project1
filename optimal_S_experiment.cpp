#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>
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

    // Different input sizes required for C(iii)
    vector<int> sizes = {
        100000,
        500000,
        1000000,
        5000000,
        10000000
    };

    // Test every possible threshold in this range
    vector<int> S_values;

    for (int S = 2; S <= 128; S++) {
        S_values.push_back(S);
    }

    // Repeat each experiment to reduce timing variation
    const int TRIALS = 5;

    // Used to determine one overall optimal S
    vector<double> overallScores(S_values.size(), 0.0);

    cout << fixed << setprecision(3);

    cout << "C(iii): Finding Optimal S Using CPU Time" << endl;
    cout << "============================================" << endl;

    for (int n : sizes) {

        cout << endl;
        cout << "n = " << n << endl;
        cout << "--------------------------------------------" << endl;
        cout << "S,Average Key Comparisons,Average CPU Time (ms)" << endl;

        // --------------------------------------------------
        // Generate datasets ONCE for this n.
        //
        // Every S is tested using exactly the same datasets.
        // --------------------------------------------------

        vector<vector<int>> datasets;

        for (int trial = 0; trial < TRIALS; trial++) {

            mt19937 rng(12345 + trial);

            datasets.push_back(
                generateRandomArray(n, x, rng)
            );
        }

        vector<double> averageTimes;
        vector<unsigned long long> averageComparisons;

        // --------------------------------------------------
        // Test every S from 2 to 128
        // --------------------------------------------------

        for (int S : S_values) {

            double totalCPUTime = 0.0;
            unsigned long long totalComparisons = 0;

            for (int trial = 0; trial < TRIALS; trial++) {

                // Copy the original dataset BEFORE timing
                vector<int> data = datasets[trial];

                // Allocate buffer BEFORE timing
                vector<int> buffer(n);

                unsigned long long comparisons = 0;

                // ------------------------------------------
                // Measure CPU time ONLY for Hybrid Sort
                // ------------------------------------------

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
                    1000.0 *
                    static_cast<double>(end - start) /
                    CLOCKS_PER_SEC;

                totalCPUTime += cpuTime;
                totalComparisons += comparisons;
            }

            double avgCPUTime =
                totalCPUTime / TRIALS;

            unsigned long long avgComparisons =
                totalComparisons / TRIALS;

            averageTimes.push_back(avgCPUTime);
            averageComparisons.push_back(avgComparisons);

            cout << S << ","
                 << avgComparisons << ","
                 << avgCPUTime << endl;
        }

        // --------------------------------------------------
        // Find optimal S for THIS n
        // based on minimum average CPU time
        // --------------------------------------------------

        int bestIndex = 0;

        for (int i = 1; i < (int)S_values.size(); i++) {

            if (averageTimes[i] < averageTimes[bestIndex]) {
                bestIndex = i;
            }
        }

        double bestTime = averageTimes[bestIndex];

        cout << endl;

        cout << "Optimal S for n = "
             << n
             << ": S = "
             << S_values[bestIndex]
             << " ("
             << bestTime
             << " ms CPU time)"
             << endl;

        // --------------------------------------------------
        // Normalize CPU times.
        //
        // Best S for this n gets score 1.0.
        // Other S values get scores > 1.0.
        //
        // This allows every input size to contribute equally
        // when finding one overall S.
        // --------------------------------------------------

        for (int i = 0; i < (int)S_values.size(); i++) {

            overallScores[i] +=
                averageTimes[i] / bestTime;
        }
    }

    // --------------------------------------------------
    // Find ONE overall optimal S across all input sizes
    // --------------------------------------------------

    int overallBestIndex = 0;

    for (int i = 1; i < (int)S_values.size(); i++) {

        if (overallScores[i] <
            overallScores[overallBestIndex]) {

            overallBestIndex = i;
        }
    }

    cout << endl;
    cout << "============================================" << endl;

    cout << "OVERALL OPTIMAL S = "
         << S_values[overallBestIndex]
         << endl;

    cout << "Average Normalized CPU-Time Score = "
         << overallScores[overallBestIndex] / sizes.size()
         << endl;

    cout << "============================================" << endl;

    return 0;
}
