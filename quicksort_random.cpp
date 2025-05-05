#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

// Function to print the array
void printArray(const vector<int> &arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << "\n";
}


int partition(vector<int> &arr, int low, int high) {
    int pivot = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
        while (arr[i] <= pivot && i <= high - 1) {
            i++;
        }

        while (arr[j] > pivot && j >= low + 1) {
            j--;
        }
        if (i < j) swap(arr[i], arr[j]);
    }
    swap(arr[low], arr[j]);
    return j;
}

void sequentialQuickSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pIndex = partition(arr, low, high);
        sequentialQuickSort(arr, low, pIndex - 1);
        sequentialQuickSort(arr, pIndex + 1, high);
    }
}

void parallelQuickSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pIndex = partition(arr, low, high);

#pragma omp task shared(arr)
        {
            parallelQuickSort(arr, low, pIndex - 1);
        }

#pragma omp task shared(arr)
        {
            parallelQuickSort(arr, pIndex + 1, high);
        }
    }
}

int main() {
    int n;
    cout<<"Enter the no of elemets in the array:";
    cin>>n;
    // Initialize random seed
    srand(time(nullptr));

    // Generate a random array
    vector<int> arr(n); // Change the array size as needed
    for (int& num : arr) {
        num = rand() % 10000; // Generate random numbers between 0 and 9999
    }

    // Make a copy of the array for parallel sorting
    vector<int> arr_parallel = arr;

    // Print array before sorting
    cout << "Original Array:\n";
    printArray(arr);

    // Measure the execution time of sequential quick sort
    clock_t start_sequential = clock();
    sequentialQuickSort(arr, 0, arr.size() - 1);
    clock_t end_sequential = clock();
    double sequential_time = end_sequential - start_sequential;

    // Print sorted array (sequential)
    cout << "Array after Sequential Quick Sort:\n";
    printArray(arr);

    // Measure the execution time of parallel quick sort
    clock_t start_parallel = clock();
    #pragma omp parallel
    {
        #pragma omp single
        parallelQuickSort(arr_parallel, 0, arr_parallel.size() - 1);
    }
    clock_t end_parallel = clock();
    double parallel_time = end_parallel - start_parallel;

    // Print the sorted arrays
    cout << "Sequential Quick Sort: " << sequential_time << "\n";
    cout << "Parallel Quick Sort: " << parallel_time << "\n";

    return 0;
}
