#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Function for matrix multiplication using OpenMP
vector<vector<int>> matrixMultiplication(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int m = A.size();
    int p = A[0].size();
    int n = B[0].size();

    vector<vector<int>> result(m, vector<int>(n, 0));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < p; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

// Function to print a matrix
void printMatrix(const vector<vector<int>>& matrix, const string& name) {
    cout << name << ":\n";
    for (const auto& row : matrix) {
        for (int val : row)
            cout << val << " ";
        cout << endl;
    }
    cout << endl;
}

int main() {
    srand(time(0));

    int m = 3 + rand() % 3; // Rows of A: 3 to 5
    int n = 3 + rand() % 3; // Columns of A / Rows of B: 3 to 5
    int p = 3 + rand() % 3; // Columns of B: 3 to 5

    cout << "Random Matrix Dimensions:\n";
    cout << "Matrix A: " << m << " x " << n << endl;
    cout << "Matrix B: " << n << " x " << p << endl;
    // cout << "Number of processors: " << omp_get_num_procs() << endl;

    // Generate random Matrix A
    vector<vector<int>> A(m, vector<int>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            A[i][j] = rand() % 10;

    // Generate random Matrix B
    vector<vector<int>> B(n, vector<int>(p));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < p; ++j)
            B[i][j] = rand() % 10;

    // Print matrices
    printMatrix(A, "Matrix A");
    printMatrix(B, "Matrix B");

    // Multiply matrices
    vector<vector<int>> result = matrixMultiplication(A, B);

    // Print result
    printMatrix(result, "Matrix A x B");

    return 0;
}
