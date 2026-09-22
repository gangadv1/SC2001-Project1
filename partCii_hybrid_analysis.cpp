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
    const int n = 1000000;
    const int x = 10000000;

    vector<int> S_values = {
        2, 4, 8, 16, 32, 64, 128, 256, 512
    };

    mt19937 rng(12345);

    // Generate ONE dataset and reuse it for every S
    vector<int> originalData = generateRandomArray(n, x, rng);

    cout << "C(ii): Fixed n = " << n << endl;
    cout << "S,Key Comparisons" << endl;

    for (int S : S_values) {
        vector<int> data = originalData;
        vector<int> buffer(n);

        unsigned long long comparisons = 0;

        hybridSort(data, 0, n, S, buffer, comparisons);

        cout << S << "," << comparisons << endl;
    }

    return 0;
}

