#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <climits>
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

double median(vector<double> values) {
    sort(values.begin(), values.end());

    int size = values.size();

    if (size % 2 == 1) {
        return values[size / 2];
    }

    return (values[size / 2 - 1] + values[size / 2]) / 2.0;
}

int main() {

    const int x = 10000000;

    // Different input sizes required by C(iii)
    vector<int> sizes = {
        100000,
        500000,
        1000000,
        5000000,
        10000000
    };

    // Candidate thresholds.
    // No assumed "correct" region.
    vector<int> S_values = {
        2, 4, 6, 8, 10, 12, 14, 16,
        20, 24, 32, 48, 64
    };

    // Multiple independent datasets for each n
    const int datasets = 5;

    // Each dataset/S combination is timed multiple times
    const int repetitions = 5;

    mt19937 dataRng(12345);
    mt19937 orderRng(54321);

    cout << fixed << setprecision(3);

    cout << "C(iii): Determining Optimal S" << endl;
    cout << "Datasets per input size: " << datasets << endl;
    cout << "Timing repetitions per dataset: "
         << repetitions << endl << endl;

    for (int n : sizes) {

        cout << "============================================" << endl;
        cout << "n = " << n << endl;
        cout << "============================================" << endl;

        // Store all timings for each S
        vector<vector<double>> allTimes(S_values.size());

        // Store total comparisons so that we can obtain
        // an average comparison count across datasets
        vector<unsigned long long> totalComparisons(
            S_values.size(), 0
        );

        for (int datasetIndex = 0;
             datasetIndex < datasets;
             datasetIndex++) {

            // Generate one independent dataset
            vector<int> originalData =
                generateRandomArray(n, x, dataRng);

            // Randomize S testing order for this dataset.
            // This avoids always testing small S first
            // and large S last.
            vector<int> order(S_values.size());

            iota(order.begin(), order.end(), 0);

            shuffle(
                order.begin(),
                order.end(),
                orderRng
            );

            for (int index : order) {

                int S = S_values[index];

                unsigned long long
                    comparisonsForDataset = 0;

                for (int run = 0;
                     run < repetitions;
                     run++) {

                    // Restore identical input
                    vector<int> data = originalData;

                    // Allocate outside timed region
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

                    allTimes[index].push_back(elapsed);

                    if (run == 0) {
                        comparisonsForDataset =
                            comparisons;
                    }

                    if (!isSorted(data)) {
                        cerr
                            << "ERROR: Sorting failed."
                            << " n=" << n
                            << " S=" << S
                            << endl;

                        return 1;
                    }
                }

                totalComparisons[index] +=
                    comparisonsForDataset;
            }
        }

        double bestMedianTime = 1e100;
        int bestS = -1;

        cout
            << "S,Average Key Comparisons,"
            << "Median CPU Time (ms)"
            << endl;

        for (int i = 0;
             i < static_cast<int>(S_values.size());
             i++) {

            int S = S_values[i];

            unsigned long long averageComparisons =
                totalComparisons[i] / datasets;

            double medianTime =
                median(allTimes[i]);

            cout
                << S << ","
                << averageComparisons << ","
                << medianTime
                << endl;

            // Selection rule defined independently:
            // minimum measured median execution time
            if (medianTime < bestMedianTime) {

                bestMedianTime = medianTime;
                bestS = S;
            }
        }

        cout << endl;

        cout
            << "Fastest S for n = "
            << n
            << ": S = "
            << bestS
            << " (median = "
            << bestMedianTime
            << " ms)"
            << endl;

        cout << endl;
    }

    return 0;
}
