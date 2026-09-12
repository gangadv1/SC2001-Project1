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

int main() {
    std::vector<int> testData = {38, 27, 43, 3, 9, 82, 10, 19, 50, 12};
    int n = testData.size();

    // Verify Original Merge Sort
    std::vector<int> arrMerge = testData;
    std::vector<int> buffer(n);
    unsigned long long mergeComps = 0;
    originalMergeSort(arrMerge, 0, n, buffer, mergeComps);
    assert(isSorted(arrMerge));

    // Verify Hybrid Sort with S = 4
    std::vector<int> arrHybrid = testData;
    unsigned long long hybridComps = 0;
    int S = 4;
    hybridSort(arrHybrid, 0, n, S, buffer, hybridComps);
    assert(isSorted(arrHybrid));

    std::cout << "Verification Passed: Both algorithms sort correctly.\n";
    std::cout << "Original Merge Sort Key Comparisons: " << mergeComps << "\n";
    std::cout << "Hybrid Sort (S=" << S << ") Key Comparisons: " << hybridComps << "\n";

    return 0;
}
