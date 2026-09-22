#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cfloat>
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

    vector<int> sizes = {
        100000,
        500000,
        1000000,
        5000000,
        10000000
    };

    // Test EVERY S from 2 to 128
    vector<int> S_values;

    for (int S = 2; S <= 128; S++) {
        S_values.push_back(S);
    }

    // Number of datasets used for each S
    const int TRIALS = 5;

    // Stores normalized performance of each S
    // across all input sizes
    vector<double> overallScores(S_values.size(), 0.0);

    cout << fixed << setprecision(3);

    cout << "C(iii): Finding Optimal S" << endl;
    cout << "============================================" << endl;

    for (int n : sizes) {

        cout << endl;
        cout << "n = " << n << endl;
        cout << "--------------------------------------------" << endl;
        cout << "S,Average Key Comparisons,Average Time (ms)" << endl;

        // -------------------------------------------------
        // Generate the datasets ONCE for this n.
        //
        // Every S will therefore be tested on exactly
        // the same input arrays.
        // -------------------------------------------------

        vector<vector<int>> datasets;

        for (int trial = 0; trial < TRIALS; trial++) {

            mt19937 rng(12345 + trial);

            datasets.push_back(
                generateRandomArray(n, x, rng)
            );
        }

        // Store results for this input size
        vector<double> averageTimes;
        vector<unsigned long long> averageComparisons;

        // -------------------------------------------------
        // Test every S
        // -------------------------------------------------

        for (int S : S_values) {

            double totalTime = 0.0;

            unsigned long long totalComparisons = 0;

            for (int trial = 0; trial < TRIALS; trial++) {

                // Copy happens BEFORE timing
                vector<int> data = datasets[trial];

                // Buffer allocation also happens BEFORE timing
                vector<int> buffer(n);

                unsigned long long comparisons = 0;

                // -----------------------------------------
                // Time ONLY Hybrid Sort
                // -----------------------------------------

                auto start =
                    chrono::steady_clock::now();

                hybridSort(
                    data,
                    0,
                    n,
                    S,
                    buffer,
                    comparisons
                );

                auto end =
                    chrono::steady_clock::now();

                double elapsed =
                    chrono::duration<double, milli>(
                        end - start
                    ).count();

                totalTime += elapsed;

                totalComparisons += comparisons;
            }

            double avgTime =
                totalTime / TRIALS;

            unsigned long long avgComparisons =
                totalComparisons / TRIALS;

            averageTimes.push_back(avgTime);
            averageComparisons.push_back(avgComparisons);

            cout << S << ","
                 << avgComparisons << ","
                 << avgTime << endl;
        }

        // -------------------------------------------------
        // Find fastest S for THIS input size
        // -------------------------------------------------

        int bestIndex = 0;

        for (int i = 1; i < (int)S_values.size(); i++) {

            if (averageTimes[i] < averageTimes[bestIndex]) {
                bestIndex = i;
            }
        }

        double bestTime = averageTimes[bestIndex];

        cout << endl;

        cout << "Fastest S for n = "
             << n
             << ": S = "
             << S_values[bestIndex]
             << " ("
             << bestTime
             << " ms)"
             << endl;

        // -------------------------------------------------
        // Normalize performance for this n.
        //
        // Fastest S gets score 1.0.
        // Others get > 1.0.
        //
        // This allows different n values to contribute
        // equally to the final optimal S.
        // -------------------------------------------------

        for (int i = 0; i < (int)S_values.size(); i++) {

            overallScores[i] +=
                averageTimes[i] / bestTime;
        }
    }

    // -------------------------------------------------
    // Find ONE overall optimal S
    // -------------------------------------------------

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

    cout << "Average normalized performance score = "
         << overallScores[overallBestIndex] / sizes.size()
         << endl;

    cout << "============================================" << endl;

    return 0;
}
