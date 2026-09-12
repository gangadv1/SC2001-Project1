def insertion_sort(arr):
    """
    Sorts an array using Insertion Sort.

    Returns:
        sorted_arr: The sorted array.
        comparisons: Number of key comparisons performed.
    """

    sorted_arr = arr.copy()
    comparisons = 0

    for i in range(1, len(sorted_arr)):
        key = sorted_arr[i]
        j = i - 1

        while j >= 0:
            comparisons += 1

            if sorted_arr[j] > key:
                sorted_arr[j + 1] = sorted_arr[j]
                j -= 1
            else:
                break

        sorted_arr[j + 1] = key

    return sorted_arr, comparisons


def merge(left, right):
    """
    Merges two sorted arrays into one sorted array.

    Returns:
        merged: The merged sorted array.
        comparisons: Number of key comparisons performed.
    """

    merged = []
    i = 0
    j = 0
    comparisons = 0

    while i < len(left) and j < len(right):
        comparisons += 1

        if left[i] <= right[j]:
            merged.append(left[i])
            i += 1
        else:
            merged.append(right[j])
            j += 1

    while i < len(left):
        merged.append(left[i])
        i += 1

    while j < len(right):
        merged.append(right[j])
        j += 1

    return merged, comparisons


def merge_sort(arr):
    """
    Sorts an array using standard Merge Sort.

    Returns:
        sorted_arr: The sorted array.
        comparisons: Number of key comparisons performed.
    """

    if len(arr) <= 1:
        return arr.copy(), 0

    middle = len(arr) // 2

    left = arr[:middle]
    right = arr[middle:]

    sorted_left, left_comparisons = merge_sort(left)
    sorted_right, right_comparisons = merge_sort(right)

    merged, merge_comparisons = merge(sorted_left, sorted_right)

    total_comparisons = (
        left_comparisons
        + right_comparisons
        + merge_comparisons
    )

    return merged, total_comparisons


def hybrid_merge_sort(arr, S):
    """
    Sorts an array using the Hybrid Merge Sort algorithm.

    Merge Sort is used for large subarrays.
    Insertion Sort is used when the subarray size is <= S.

    Returns:
        sorted_arr: The sorted array.
        comparisons: Number of key comparisons performed.
    """

    if len(arr) <= 1:
        return arr.copy(), 0

    if len(arr) <= S:
        return insertion_sort(arr)

    middle = len(arr) // 2

    left = arr[:middle]
    right = arr[middle:]

    sorted_left, left_comparisons = hybrid_merge_sort(left, S)
    sorted_right, right_comparisons = hybrid_merge_sort(right, S)

    merged, merge_comparisons = merge(sorted_left, sorted_right)

    total_comparisons = (
        left_comparisons
        + right_comparisons
        + merge_comparisons
    )

    return merged, total_comparisons
