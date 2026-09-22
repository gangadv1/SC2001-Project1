#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
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
    const int n = 10000000;
    const int x = 10000000;
    
    // Use the optimal S determined from Part (c)(iii)
    const int optimalS = 12; // Typically between 8 and 16

    mt19937 rng(12345);

    cout << "==========================================================" << endl;
    cout << "Part (d): Original Merge Sort vs Hybrid Sort (n = " << n << ")" << endl;
    cout << "==========================================================" << endl;

    cout << "Generating dataset of size " << n << "..." << endl;
    vector<int> originalData = generateRandomArray(n, x, rng);

    // Create identical instances for a fair benchmark
    vector<int> arrOriginal = originalData;
    vector<int> arrHybrid = originalData;
    vector<int> buffer(n);

    // ----------------------------------------------------
    // 1. Benchmark Standard Merge Sort
    // ----------------------------------------------------
    unsigned long long originalComps = 0;
    cout << "Running Original Merge Sort..." << endl;

    auto startOrig = high_resolution_clock::now();
    originalMergeSort(arrOriginal, 0, n, buffer, originalComps);
    auto endOrig = high_resolution_clock::now();

    double origTimeMs = duration_cast<nanoseconds>(endOrig - startOrig).count() / 1e6;
    cout << "  Verification: " << (isSorted(arrOriginal) ? "PASSED" : "FAILED") << endl;

    // ----------------------------------------------------
    // 2. Benchmark Hybrid Sort (with optimal S)
    // ----------------------------------------------------
    unsigned long long hybridComps = 0;
    cout << "Running Hybrid Sort (S = " << optimalS << ")..." << endl;

    auto startHyb = high_resolution_clock::now();
    hybridSort(arrHybrid, 0, n, optimalS, buffer, hybridComps);
    auto endHyb = high_resolution_clock::now();

    double hybTimeMs = duration_cast<nanoseconds>(endHyb - startHyb).count() / 1e6;
    cout << "  Verification: " << (isSorted(arrHybrid) ? "PASSED" : "FAILED") << endl;

    // ----------------------------------------------------
    // 3. Print Comparison Table
    // ----------------------------------------------------
    double compDiffPercent = ((double)originalComps - (double)hybridComps) / (double)originalComps * 100.0;
    double timeDiffPercent = (origTimeMs - hybTimeMs) / origTimeMs * 100.0;

    cout << "\n---------------- Benchmark Summary ----------------\n";
    cout << left << setw(28) << "Algorithm" 
         << setw(20) << "Key Comparisons" 
         << setw(16) << "CPU Time (ms)" << endl;
    cout << string(64, '-') << endl;

    cout << left << setw(28) << "Original Merge Sort" 
         << setw(20) << originalComps 
         << setw(16) << fixed << setprecision(2) << origTimeMs << endl;

    cout << left << setw(28) << ("Hybrid Sort (S=" + to_string(optimalS) + ")") 
         << setw(20) << hybridComps 
         << setw(16) << fixed << setprecision(2) << hybTimeMs << endl;
    cout << string(64, '-') << endl;

    cout << "Comparison Reduction: " << fixed << setprecision(2) << compDiffPercent << "% fewer comparisons\n";
    cout << "Execution Speedup:    " << fixed << setprecision(2) << timeDiffPercent << "% faster\n";

    return 0;
}
