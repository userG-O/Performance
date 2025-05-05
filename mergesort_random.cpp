#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>

using namespace std;

// Function to print the array
void printArray(const vector<int> &arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << "\n";
}


void merge(vector<int> &arr, int low, int mid, int high) {
    vector<int> temp; // temporary array
    int left = low;      // starting index of left half of arr
    int right = mid + 1;   // starting index of right half of arr

    //storing elements in the temporary array in a sorted manner//

    while (left <= mid && right <= high) {
        if (arr[left] <= arr[right]) {
            temp.push_back(arr[left]);
            left++;
        } else {
            temp.push_back(arr[right]);
            right++;
        }
    }

    // if elements on the left half are still left //

    while (left <= mid) {
        temp.push_back(arr[left]);
        left++;
    }

    //  if elements on the right half are still left //
    while (right <= high) {
        temp.push_back(arr[right]);
        right++;
    }

    // transfering all elements from temporary to arr //
    for (int i = low; i <= high; i++) {
        arr[i] = temp[i - low];
    }
}

void mergeSort(vector<int> &arr, int low, int high) {
    if (low >= high) return;
    int mid = (low + high) / 2;

    // Parallelize the sorting of left and right halves using OpenMP tasks
#pragma omp parallel sections
    {
#pragma omp section
        mergeSort(arr, low, mid);  // left half
#pragma omp section
        mergeSort(arr, mid + 1, high); // right half
    }
    merge(arr, low, mid, high);  // merging sorted halves
}

int main() {
    int n;
    cout<<"Enter the no of elements in the array:";
    cin>>n;
    srand(time(nullptr));

    vector<int> arr(n);
    for (int &num : arr)
        num = rand() % 10000;

    
    // Print array before sorting
    cout << "Original Array:\n";
    printArray(arr);

    vector<int> arr_seq = arr;
    vector<int> arr_par = arr;


    // Sequential Merge Sort
    clock_t start_seq = clock();
    mergeSort(arr_seq, 0, arr_seq.size() - 1);
    clock_t end_seq = clock();
    double seq_time = double(end_seq - start_seq) / CLOCKS_PER_SEC;

    // Print sorted array (sequential)
    cout << "Array after Sequential Quick Sort:\n";
    printArray(arr_seq);

    // Parallel Merge Sort
    clock_t start_par = clock();
#pragma omp parallel
    {
#pragma omp single
        mergeSort(arr_par, 0, arr_par.size() - 1);
    }
    clock_t end_par = clock();
    double par_time = double(end_par - start_par) / CLOCKS_PER_SEC;

    cout << "Sequential Merge Sort Time: " << seq_time << " seconds\n";
    cout << "Parallel Merge Sort Time: " << par_time << " seconds\n";

    return 0;
}
