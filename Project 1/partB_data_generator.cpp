#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

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
    // Largest possible value in each dataset
    const int x = 10000000;

    // Dataset sizes used for the experiment
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

    // Fixed seed for reproducibility
    mt19937 rng(12345);

    cout << "Generating datasets..." << endl;
    cout << "Values are in the range [1, " << x << "]" << endl;
    cout << endl;

    for (int n : sizes) {
        vector<int> data = generateRandomArray(n, x, rng);

        int minValue = *min_element(data.begin(), data.end());
        int maxValue = *max_element(data.begin(), data.end());

        bool correctSize = (data.size() == static_cast<size_t>(n));
        bool correctRange = (minValue >= 1 && maxValue <= x);

        cout << "n = " << n
             << " | size: " << (correctSize ? "PASS" : "FAIL")
             << " | range: " << (correctRange ? "PASS" : "FAIL")
             << " | min = " << minValue
             << " | max = " << maxValue
             << endl;
    }

    cout << endl;
    cout << "All datasets generated and verified successfully." << endl;

    return 0;
}
