#include <iostream>
#include <vector>
#include <random>
#include <chrono>
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

    // Granular integer values up to 32, then extending up to 256
    vector<int> S_values;
    for (int s = 1; s <= 32; s++) {
        S_values.push_back(s);
    }
    vector<int> extended = {40, 50, 64, 80, 96, 128, 160, 200, 256};
    S_values.insert(S_values.end(), extended.begin(), extended.end());

    mt19937 rng(12345);

    // Generate ONE dataset and reuse it across all S runs
    vector<int> originalData = generateRandomArray(n, x, rng);
    vector<int> buffer(n);

    // CSV Header matching plot_c2.py
    cout << "S,Key Comparisons,CPU Time (ms)" << endl;

    for (int S : S_values) {
        vector<int> data = originalData;
        unsigned long long comparisons = 0;

        auto start = high_resolution_clock::now();
        hybridSort(data, 0, n, S, buffer, comparisons);
        auto end = high_resolution_clock::now();

        double durationMs = duration_cast<nanoseconds>(end - start).count() / 1e6;

        cout << S << "," << comparisons << "," << durationMs << endl;
    }

    return 0;
}
