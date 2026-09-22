#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include "sorts.cpp"

using namespace std;

// Generate an array of n random integers in the range [1, x]
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

    // Small arrays are used to determine when Insertion Sort
    // becomes more expensive than Merge Sort.
    const int minSize = 2;
    const int maxSize = 20;

    // Average over many random arrays so the result is not
    // determined by one unusually easy or difficult input.
    const int trials = 1000;

    mt19937 rng(12345);

    int optimalS = 0;

    cout << "C(iii): Determining optimal threshold S" << endl;
    cout << "n,Insertion Sort Avg Comparisons,Merge Sort Avg Comparisons"
         << endl;

    for (int n = minSize; n <= maxSize; n++) {
        unsigned long long totalInsertionComparisons = 0;
        unsigned long long totalMergeComparisons = 0;

        for (int trial = 0; trial < trials; trial++) {
            // Generate one array and give identical copies
            // to both sorting algorithms.
            vector<int> originalData = generateRandomArray(n, x, rng);

            vector<int> insertionData = originalData;
            vector<int> mergeData = originalData;

            unsigned long long insertionComparisons = 0;
            unsigned long long mergeComparisons = 0;

            // Run Insertion Sort
            insertionSort(
                insertionData,
                0,
                n,
                insertionComparisons
            );

            // Run Original Merge Sort
            vector<int> buffer(n);

            originalMergeSort(
                mergeData,
                0,
                n,
                buffer,
                mergeComparisons
            );

            totalInsertionComparisons += insertionComparisons;
            totalMergeComparisons += mergeComparisons;
        }

        double avgInsertion =
            static_cast<double>(totalInsertionComparisons) / trials;

        double avgMerge =
            static_cast<double>(totalMergeComparisons) / trials;

        cout << fixed << setprecision(2)
             << n << ","
             << avgInsertion << ","
             << avgMerge << endl;

        // Largest input size for which Insertion Sort
        // requires fewer key comparisons than Merge Sort.
        if (avgInsertion < avgMerge) {
            optimalS = n;
        }
    }

    cout << endl;

    if (optimalS > 0) {
        cout << "Selected optimal threshold S = "
             << optimalS << endl;
    } else {
        cout << "No crossover threshold found in the tested range."
             << endl;
    }

    return 0;
}
