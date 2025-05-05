#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Function to multiply matrix with vector using OpenMP
vector<int> vectorMatrixMult(const vector<vector<int>>& matrix, const vector<int>& vec) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    if (cols != vec.size()) {
        cout << "Incompatible dimensions! Cannot perform multiplication.\n";
        return {};
    }

    vector<int> result(rows, 0);

    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }

    return result;
}

// Function to print a matrix
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row)
            cout << val << " ";
        cout << endl;
    }
}

// Function to print a vector
void printVector(const vector<int>& vec) {
    for (int val : vec)
        cout << val << " ";
    cout << endl;
}

int main() {
    srand(time(0));

    int rows = 3 + rand() % 3;  // Rows between 3 and 5
    int cols = 3 + rand() % 3;  // Columns between 3 and 5

    cout << "Random Matrix Dimensions: " << rows << " x " << cols << endl;
    // cout << "Number of processors: " << omp_get_num_procs() << endl;

    // Generate random matrix
    vector<vector<int>> matrix(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            matrix[i][j] = rand() % 10;

    // Generate random vector
    vector<int> vec(cols);
    for (int i = 0; i < cols; ++i)
        vec[i] = rand() % 10;

    // Display inputs
    cout << "\nMatrix:\n";
    printMatrix(matrix);

    cout << "\nVector:\n";
    printVector(vec);

    // Perform multiplication
    vector<int> result = vectorMatrixMult(matrix, vec);

    // Display result
    cout << "\nResultant Vector (Matrix x Vector):\n";
    printVector(result);

    return 0;
}
