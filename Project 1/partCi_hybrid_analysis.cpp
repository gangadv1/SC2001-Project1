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
    const int S = 32;

    vector<int> sizes = {
        1000,
        2000,
        5000,
        10000,
        20000,
        50000,
        100000,
        200000,
        500000,
        1000000,
        2000000,
        5000000,
        10000000
    };

    mt19937 rng(12345);

    cout << "C(i): Fixed S = " << S << endl;
    cout << "n,Key Comparisons" << endl;

    for (int n : sizes) {
        vector<int> data = generateRandomArray(n, x, rng);
        vector<int> buffer(n);

        unsigned long long comparisons = 0;

        hybridSort(data, 0, n, S, buffer, comparisons);

        cout << n << "," << comparisons << endl;
    }

    return 0;
}
