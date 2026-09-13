#include <iostream>
#include <vector>
#include <random>
#include <climits>
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

    vector<int> S_values = {
        2, 4, 8, 16, 32, 64, 128, 256, 512
    };

    mt19937 rng(12345);

    cout << "C(iii): Finding optimal S" << endl;
    cout << "n,Best S,Minimum Key Comparisons" << endl;

    for (int n : sizes) {

        vector<int> originalData = generateRandomArray(n, x, rng);

        int bestS = 0;
        unsigned long long bestComparisons = ULLONG_MAX;

        for (int S : S_values) {

            vector<int> data = originalData;
            vector<int> buffer(n);

            unsigned long long comparisons = 0;

            hybridSort(data, 0, n, S, buffer, comparisons);

            if (comparisons < bestComparisons) {
                bestComparisons = comparisons;
                bestS = S;
            }
        }

        cout << n << "," << bestS << "," << bestComparisons << endl;
    }

    return 0;
}
