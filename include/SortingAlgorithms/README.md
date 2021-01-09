# Sorting Algorithms

All sorting algorithms are templated, though certain sorting algorithms are restricted to numeric types, like `RadixSort`.

* BubbleSort
* BucketSort
* CountingSort
* HeapSort
* InsertionSort
* MergeSort
* QuickSort_Lomuto
* QuickSort_Hoare
* RadixSort
* SelectionSort
* TreeSort

All sorting algorithms sorts vector of elements in-place.

## Comparator

By default, the search is done using `operator<`. A custom binary predicate can be passed in as a template argument, which is used to compare the elements.

A custom binary predicate is required if

1. Elements are not sorted in ascending order
2. Custom type which do not overload `operator<`
