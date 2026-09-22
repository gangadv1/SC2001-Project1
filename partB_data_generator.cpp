#include <iostream>
#include <vector>
#include <random>
#include <cassert>

using namespace std;

// In-place generation of n uniform random integers in [1, x] to minimize dynamic allocation overhead
void generateRandomArrayInPlace(vector<int>& data, int x, mt19937& rng) {
    uniform_int_distribution<int> dist(1, x);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = dist(rng);
    }
}

// Generate an array of n random integers in [1, x] (returns a new vector)[cite: 1]
vector<int> generateRandomArray(int n, int x, mt19937& rng) {
    vector<int> data(n);
    generateRandomArrayInPlace(data, x, rng);
    return data;
}

int main() {
    // Range constraint [1, x] where x = 10,000,000[cite: 1]
    const int x = 10000000;

    // Array sizes ranging from 1,000 up to 10,000,000[cite: 1]
    const vector<int> sizes = {
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

    // Fixed seed ensures completely reproducible datasets across test runs
    const unsigned int SEED = 12345;
    mt19937 rng(SEED);

    cout << "==========================================" << endl;
    cout << "  SC2001 Project 1: Dataset Generation   " << endl;
    cout << "==========================================" << endl;
    cout << "Value Range: [1, " << x << "]" << endl;
    cout << "Total dataset sizes to test: " << sizes.size() << endl << endl;

    for (int n : sizes) {
        // Generate the test vector
        vector<int> data = generateRandomArray(n, x, rng);

        // Verification checks
        assert((int)data.size() == n);
        assert(data.front() >= 1 && data.front() <= x);
        assert(data.back() >= 1 && data.back() <= x);

        cout << "Dataset generated successfully -> Size n = " << n 
             << " | Sample: [" << data[0] << ", " << data[1] << ", ..., " << data[n - 1] << "]" 
             << endl;
    }

    cout << endl << "All datasets generated and verified successfully." << endl;

    return 0;
}
