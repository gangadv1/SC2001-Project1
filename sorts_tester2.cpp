#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <chrono>

// Insertion Sort
void insertionSort(std::vector<int>& A, int start, int end, unsigned long long& comparisons) {
    for (int i = start + 1; i < end; i++) {
        int key = A[i];
        int j = i - 1;
        while (j >= start) {
            comparisons++;
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

// Merge
void merge(std::vector<int>& A, int start, int mid, int end, 
           std::vector<int>& buffer, unsigned long long& comparisons) {
    int i = start;
    int j = mid;
    int k = start; // Fixed from 'end' to 'start'

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

    while (j < end) {
        buffer[k++] = A[j++];
    }

    for (int idx = start; idx < end; ++idx) {
        A[idx] = buffer[idx];
    }
}

// Original Merge Sort (Part d baseline)
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

// Verification helper
bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

int main() {
    // 1. Edge case tests
    std::vector<std::vector<int>> edgeCases = {
        {},
        {42},
        {1, 2, 3, 4, 5},
        {5, 4, 3, 2, 1},
        {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5},
        {2, 2, 2, 2, 2}
    };

    for (auto& arr : edgeCases) {
        int n = arr.size();
        std::vector<int> buf(n);
        unsigned long long cOrig = 0, cHyb = 0;

        std::vector<int> arrOrig = arr;
        originalMergeSort(arrOrig, 0, n, buf, cOrig);
        assert(isSorted(arrOrig));

        std::vector<int> arrHyb = arr;
        hybridSort(arrHyb, 0, n, 2, buf, cHyb);
        assert(isSorted(arrHyb));
    }
    std::cout << "[✓] Edge cases passed.\n";

    // 2. Randomized stress test (n = elements)
    int n = 10000000;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(1, n);

    std::vector<int> testData(n);
    for (int i = 0; i < n; ++i) {
        testData[i] = dist(rng);
    }

    std::vector<int> stdSorted = testData;
    std::sort(stdSorted.begin(), stdSorted.end());

    std::vector<int> buffer(n);

    // Test Original Merge Sort
    std::vector<int> origData = testData;
    unsigned long long origComps = 0;
    auto startOrig = std::chrono::high_resolution_clock::now();
    originalMergeSort(origData, 0, n, buffer, origComps);
    auto endOrig = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeOrig = endOrig - startOrig;
    assert(isSorted(origData));
    assert(origData == stdSorted);
    std::cout << "[✓] Original Merge Sort verified against std::sort.\n";

    // Test Hybrid Sort
    std::vector<int> hybridData = testData;
    unsigned long long hybridComps = 0;
    int S = 2;
    auto startHyb = std::chrono::high_resolution_clock::now();
    hybridSort(hybridData, 0, n, S, buffer, hybridComps);
    auto endHyb = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeHyb = endHyb - startHyb;
    assert(isSorted(hybridData));
    assert(hybridData == stdSorted);
    std::cout << "[✓] Hybrid Sort (S = " << S << ") verified against std::sort.\n";

    std::cout << "\nSummary for n = " << n << ":\n";
    std::cout << "Original Merge Sort Comparisons: " << origComps << "\n";
    std::cout << "Original Merge Sort Time:        " << timeOrig.count() << " s\n";
    std::cout << "Hybrid Sort (S=" << S << ") Comparisons:  " << hybridComps << "\n";
    std::cout << "Hybrid Sort (S=" << S << ") Time:        " << timeHyb.count() << " s\n";

    return 0;
}