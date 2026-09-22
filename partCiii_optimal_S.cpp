#include <iostream>
#include <vector>
#include <random>
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

    mt19937 rng(12345);

    cout << "n,Insertion Comparisons,Merge Comparisons"
         << endl;

    int optimalS = 0;

    for (int n = 2; n <= 20; n++) {

        vector<int> originalData =
            generateRandomArray(n, x, rng);

        // Same input for both algorithms
        vector<int> insertionData = originalData;
        vector<int> mergeData = originalData;

        unsigned long long insertionComparisons = 0;
        unsigned long long mergeComparisons = 0;

        // Insertion Sort
        insertionSort(
            insertionData,
            0,
            n,
            insertionComparisons
        );

        // Original Merge Sort
        vector<int> buffer(n);

        originalMergeSort(
            mergeData,
            0,
            n,
            buffer,
            mergeComparisons
        );

        cout << n << ","
             << insertionComparisons << ","
             << mergeComparisons << endl;

        // Largest n where insertion sort
        // uses fewer key comparisons
        if (insertionComparisons < mergeComparisons) {
            optimalS = n;
        }
    }

    cerr << endl;
    cerr << "Selected optimal S = "
         << optimalS << endl;

    return 0;
}
