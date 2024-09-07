#include <iostream>

using namespace std;

int** AllocateMemory(int& rows, int& cols) {
    int** matrix = new int* [rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
    }
    return matrix;
}

void InputMatrix(int** matrix, const int rows, const int cols) {
    cout << "Enter the elements of the matrix:" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << "Enter element [" << i << "][" << j << "]: ";
            cin >> matrix[i][j];
        }
    }
}

void DisplayMatrix(int** matrix, const int& rows, const int& cols) {
    cout << "Matrix:" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void swapTriangular(int** matrix, const int& rows, const int& cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = i + 1; j < cols; ++j) {
            if (i < j) {
                int temp = matrix[i][j];
                matrix[i][j] = matrix[j][i];
                matrix[j][i] = temp;
            }
        }
    }
}

int main() {
    int rows, cols;
    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;
    // Allocate memory for matrix
    int** matrix = AllocateMemory(rows, cols);
    // Input matrix elements from the user
    InputMatrix(matrix, rows, cols);
    // Display the input matrix
    DisplayMatrix(matrix, rows, cols);
    // Swap the upper triangular halve with lower triangular halve
    swapTriangular(matrix, rows, cols);
    // Display the modified matrix
    DisplayMatrix(matrix, rows, cols);
    // Free allocated memory
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    return 0;
}
