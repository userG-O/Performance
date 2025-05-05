#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to find the minimum element in an array
int parallelMin(const vector<int>& arr) {
    int min_val = INT_MAX;

    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < arr.size(); ++i) {
        min_val = min(min_val, arr[i]);
    }

    return min_val;
}

// Function to find the maximum element in an array
int parallelMax(const vector<int>& arr) {
    int max_val = INT_MIN;

    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < arr.size(); ++i) {
        max_val = max(max_val, arr[i]);
    }

    return max_val;
}

// Function to compute the sum of elements in an array
int parallelSum(const vector<int>& arr) {
    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }

    return sum;
}

// Function to compute the average of elements in an array
double parallelAverage(const vector<int>& arr) {
    int sum = parallelSum(arr);
    double avg = static_cast<double>(sum) / arr.size();
    return avg;
}

int main() {
    srand(time(0)); // Seed for random number generator

    int n;
    cout << "Enter number of random elements to generate: ";
    cin >> n;

    vector<int> arr(n);

    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100 + 1; // Random numbers between 1 and 100
    }

    // cout << "\nNumber of processors available: " << omp_get_num_procs() << endl;

    cout << "Randomly generated array:\n";
    for (int val : arr) {
        cout << val << " ";
    }
    cout << endl;

    // Compute using parallel functions
    int min_val = parallelMin(arr);
    int max_val = parallelMax(arr);
    int sum = parallelSum(arr);
    double avg = parallelAverage(arr);

    // Output the results
    cout << "\nParallel Reduction Results:\n";
    cout << "Minimum: " << min_val << endl;
    cout << "Maximum: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;

    return 0;
}
