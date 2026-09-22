#include <iostream>
#include <vector>
#include <random>
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
    const int n = 1000000;
    const int x = 10000000;

    // Fixed seed for reproducibility
    mt19937 rng(12345);

    // Generate one dataset and reuse the exact same data for every S
    vector<int> originalData = generateRandomArray(n, x, rng);

    cout << "C(ii): Fixed n = " << n << endl;
    cout << "S,Key Comparisons" << endl;

    // Test consecutive S values to observe the staircase pattern
    for (int S = 1; S <= 128; S++) {
        vector<int> data = originalData;
        vector<int> buffer(n);

        unsigned long long comparisons = 0;

        hybridSort(data, 0, n, S, buffer, comparisons);

        cout << S << "," << comparisons << endl;
    }

    return 0;
}
