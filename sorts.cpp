#include <iostream>
#include <vector>
#include <cassert>

// Insertion Sort
void insertionSort(std::vector<int>& A, int start, int end, unsigned long long& comparisons) {
    for (int i = start + 1; i < end; i++) {
        int key = A[i];
        int j = i -1;
        while (j >= start) {
            comparisons ++;
            if (A[j] > key) {
                A[j + 1] = A[j];
                j--;
            } else {
                break;
            }
        }
        A[j + 1] = key;
    }
}

// Merge Sort
void merge(std::vector<int>& A, int start, int mid, int end, 
           std::vector<int>& buffer, unsigned long long& comparisons) {
            int i = start;
            int j = mid;
            int k = start;

            while (i < mid && j < end) {
                comparisons++;
                if (A[i] <= A[j]) {
                    buffer[k++] = A[i++];
                } else {
                    buffer[k++] = A[j++];
                
                }
           }

           while (i < mid) {
                buffer[k++] = A[i++];
           }

           while (j<end) {
                buffer[k++] = A[j++];
           }

           for (int idx = start; idx < end; ++idx) {
                A[idx] = buffer[idx];
           }
        }    


// Original Merge Sort(for Part(d), to be used for comparison)
void originalMergeSort(std::vector<int>& A, int start, int end, 
                      std::vector<int>& buffer, unsigned long long& comparisons) {
    if (end - start <= 1) {
        return;
    }

    int mid = start + (end - start) / 2;

    originalMergeSort(A, start, mid, buffer, comparisons);
    originalMergeSort(A, mid, end, buffer, comparisons);
    merge(A, start, mid, end, buffer, comparisons);
    }

// Hybrid Sort
void hybridSort(std::vector<int>& A, int start, int end, int S, 
                std::vector<int>& buffer, unsigned long long& comparisons) {
    if (end - start <= S) {
        insertionSort(A, start, end, comparisons);
        return;
    }

    int mid = start + (end - start) / 2;

    hybridSort(A, start, mid, S, buffer, comparisons);
    hybridSort(A, mid, end, S, buffer, comparisons);
    merge(A, start, mid, end, buffer, comparisons);
}

// Check if array is sorted in non-decreasing order
bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

Old B
#include <iostream>
#include <vector>
#include <random>
#include <fstream>

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

    // Dataset sizes required for the experiment
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

    // Fixed seed so that the experiments are reproducible
    mt19937 rng(12345);

    cout << "Generating datasets..." << endl;
    cout << "Values are in the range [1, " << x << "]" << endl;
    cout << endl;

    for (int n : sizes) {
        vector<int> data = generateRandomArray(n, x, rng);

        cout << "Generated dataset: n = " << n << endl;
    }

    cout << endl;
    cout << "All datasets generated successfully." << endl;

    return 0;
}
