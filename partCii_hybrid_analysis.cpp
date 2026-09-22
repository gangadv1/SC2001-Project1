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

    const int n = 1000000;
    const int x = 10000000;

    mt19937 rng(12345);

    // Generate ONE original dataset.
    // Every S is tested on exactly the same data.
    vector<int> originalData =
        generateRandomArray(n, x, rng);

    cout << "S,Key Comparisons,CPU Time (ms)" << endl;

    // Consecutive S values reveal the staircase pattern.
    for (int S = 1; S <= 64; S++) {

        vector<int> data = originalData;
        vector<int> buffer(n);

        unsigned long long comparisons = 0;

        // Measure CPU time only for Hybrid Sort.
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

        cout << S << ","
             << comparisons << ","
             << fixed << setprecision(3)
             << cpuTime << endl;
    }

    return 0;
}
