#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "sorts.cpp"

using namespace std;
using namespace std::chrono;

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

    // Test points matching the reference[cite: 5]
    vector<int> S_values;
    for (int s = 1; s <= 32; s++) {
        S_values.push_back(s);
    }
    S_values.push_back(40);
    S_values.push_back(50);
    S_values.push_back(64);

    mt19937 rng(12345);
    vector<int> originalData = generateRandomArray(n, x, rng);
    vector<int> buffer(n);

    cout << "S,Key Comparisons,CPU Time (ms)" << endl;

    for (int S : S_values) {
        const int TRIALS = 5;
        vector<double> times;
        unsigned long long comparisons = 0;

        for (int t = 0; t < TRIALS; t++) {
            vector<int> data = originalData;
            comparisons = 0;

            auto start = high_resolution_clock::now();
            hybridSort(data, 0, n, S, buffer, comparisons);
            auto end = high_resolution_clock::now();

            double durationMs = duration_cast<nanoseconds>(end - start).count() / 1e6;
            times.push_back(durationMs);
        }

        // Use median to completely ignore rogue OS spikes
        sort(times.begin(), times.end());
        double medianTime = times[TRIALS / 2];

        cout << S << "," << comparisons << "," << medianTime << endl;
    }

    return 0;
}
