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

    // Exact tested values of S from the reference graph[cite: 5]
    vector<int> S_values;
    for (int s = 1; s <= 32; s++) {
        S_values.push_back(s);
    }
    S_values.push_back(40);
    S_values.push_back(50);
    S_values.push_back(64);

    mt19937 rng(12345);

    // Reuse the exact same array across all S values
    vector<int> originalData = generateRandomArray(n, x, rng);
    vector<int> buffer(n);

    cout << "S,Key Comparisons,CPU Time (ms)" << endl;

    for (int S : S_values) {
        // Average over 3 runs to stabilize runtime spikes while maintaining true behavior
        const int TRIALS = 3;
        double totalTime = 0;
        unsigned long long comparisons = 0;

        for (int t = 0; t < TRIALS; t++) {
            vector<int> data = originalData;
            comparisons = 0;

            auto start = high_resolution_clock::now();
            hybridSort(data, 0, n, S, buffer, comparisons);
            auto end = high_resolution_clock::now();

            totalTime += duration_cast<nanoseconds>(end - start).count() / 1e6;
        }

        cout << S << "," << comparisons << "," << (totalTime / TRIALS) << endl;
    }

    return 0;
}
