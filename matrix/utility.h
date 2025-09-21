#ifndef MATRIX_UTILITY_H
#define MATRIX_UTILITY_H

#include <iostream>
#include <iomanip>

// Matrix class - responsible for matrix data structure and basic operations
class Matrix {
private:
    int** matrix;
    int rows;
    int cols;

public:
    // Constructor
    Matrix(int r = 0, int c = 0) : rows(r), cols(c) {
        if (rows > 0 && cols > 0) {
            matrix = new int*[rows];
            for (int i = 0; i < rows; i++) {
                matrix[i] = new int[cols];
                // Initialize to zero
                for (int j = 0; j < cols; j++) {
                    matrix[i][j] = 0;
                }
            }
        } else {
            matrix = nullptr;
        }
    }

    // Copy constructor
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        if (rows > 0 && cols > 0) {
            matrix = new int*[rows];
            for (int i = 0; i < rows; i++) {
                matrix[i] = new int[cols];
                for (int j = 0; j < cols; j++) {
                    matrix[i][j] = other.matrix[i][j];
                }
            }
        } else {
            matrix = nullptr;
        }
    }

    // Destructor
    ~Matrix() {
        if (matrix != nullptr) {
            for (int i = 0; i < rows; i++) {
                delete[] matrix[i];
            }
            delete[] matrix;
        }
    }

    // Assignment operator
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            // Clean up existing memory
            if (matrix != nullptr) {
                for (int i = 0; i < rows; i++) {
                    delete[] matrix[i];
                }
                delete[] matrix;
            }

            // Copy new data
            rows = other.rows;
            cols = other.cols;
            
            if (rows > 0 && cols > 0) {
                matrix = new int*[rows];
                for (int i = 0; i < rows; i++) {
                    matrix[i] = new int[cols];
                    for (int j = 0; j < cols; j++) {
                        matrix[i][j] = other.matrix[i][j];
                    }
                }
            } else {
                matrix = nullptr;
            }
        }
        return *this;
    }

    // Getters
    int getRows() const { return rows; }
    int getCols() const { return cols; }

    // Set element at specific position
    void setElement(int row, int col, int value) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            matrix[row][col] = value;
        }
    }

    // Get element at specific position
    int getElement(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return matrix[row][col];
        }
        return 0; // Return 0 for invalid indices
    }

    // Input matrix values
    void inputMatrix() {
        std::cout << "Enter matrix elements (" << rows << "x" << cols << "):\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::cout << "Element [" << i << "][" << j << "]: ";
                std::cin >> matrix[i][j];
            }
        }
    }

    // Display matrix
    void displayMatrix() const {
        if (matrix == nullptr) {
            std::cout << "Empty matrix\n";
            return;
        }
        
        std::cout << "Matrix (" << rows << "x" << cols << "):\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::cout << std::setw(6) << matrix[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    // Check if matrix is empty
    bool isEmpty() const {
        return matrix == nullptr || rows == 0 || cols == 0;
    }

    // Friend class declaration to allow MatrixOperations to access private members
    friend class MatrixOperations;
};

// MatrixOperations class - responsible for mathematical operations on matrices
class MatrixOperations {
public:
    // Static method for matrix addition
    static Matrix add(const Matrix& matrix1, const Matrix& matrix2) {
        // Check if matrices can be added
        if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
            std::cout << "Error: Matrices must have same dimensions for addition!\n";
            std::cout << "Matrix 1: " << matrix1.rows << "x" << matrix1.cols << "\n";
            std::cout << "Matrix 2: " << matrix2.rows << "x" << matrix2.cols << "\n";
            return Matrix(); // Return empty matrix
        }

        Matrix result(matrix1.rows, matrix1.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                result.matrix[i][j] = matrix1.matrix[i][j] + matrix2.matrix[i][j];
            }
        }
        return result;
    }

    // Static method for matrix multiplication
    static Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
        // Check if matrices can be multiplied
        if (matrix1.cols != matrix2.rows) {
            std::cout << "Error: First matrix columns must equal second matrix rows for multiplication!\n";
            std::cout << "Matrix 1: " << matrix1.rows << "x" << matrix1.cols << "\n";
            std::cout << "Matrix 2: " << matrix2.rows << "x" << matrix2.cols << "\n";
            return Matrix(); // Return empty matrix
        }

        Matrix result(matrix1.rows, matrix2.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix2.cols; j++) {
                result.matrix[i][j] = 0;
                for (int k = 0; k < matrix1.cols; k++) {
                    result.matrix[i][j] += matrix1.matrix[i][k] * matrix2.matrix[k][j];
                }
            }
        }
        return result;
    }

    // Static method for matrix subtraction
    static Matrix subtract(const Matrix& matrix1, const Matrix& matrix2) {
        // Check if matrices can be subtracted
        if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
            std::cout << "Error: Matrices must have same dimensions for subtraction!\n";
            std::cout << "Matrix 1: " << matrix1.rows << "x" << matrix1.cols << "\n";
            std::cout << "Matrix 2: " << matrix2.rows << "x" << matrix2.cols << "\n";
            return Matrix(); // Return empty matrix
        }

        Matrix result(matrix1.rows, matrix1.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                result.matrix[i][j] = matrix1.matrix[i][j] - matrix2.matrix[i][j];
            }
        }
        return result;
    }

    // Static method for scalar multiplication
    static Matrix scalarMultiply(const Matrix& matrix1, int scalar) {
        if (matrix1.isEmpty()) {
            std::cout << "Error: Cannot perform scalar multiplication on empty matrix!\n";
            return Matrix();
        }

        Matrix result(matrix1.rows, matrix1.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                result.matrix[i][j] = matrix1.matrix[i][j] * scalar;
            }
        }
        return result;
    }

    // Static method for matrix transpose
    static Matrix transpose(const Matrix& matrix1) {
        if (matrix1.isEmpty()) {
            std::cout << "Error: Cannot transpose empty matrix!\n";
            return Matrix();
        }

        Matrix result(matrix1.cols, matrix1.rows);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                result.matrix[j][i] = matrix1.matrix[i][j];
            }
        }
        return result;
    }

    // Static method to check if two matrices are equal
    static bool isEqual(const Matrix& matrix1, const Matrix& matrix2) {
        if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
            return false;
        }

        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                if (matrix1.matrix[i][j] != matrix2.matrix[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    // Static method to check if matrix is square
    static bool isSquare(const Matrix& matrix1) {
        return matrix1.rows == matrix1.cols && !matrix1.isEmpty();
    }

    // Static method to create identity matrix
    static Matrix createIdentityMatrix(int size) {
        if (size <= 0) {
            std::cout << "Error: Invalid size for identity matrix!\n";
            return Matrix();
        }

        Matrix result(size, size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == j) {
                    result.matrix[i][j] = 1;
                } else {
                    result.matrix[i][j] = 0;
                }
            }
        }
        return result;
    }

    // Display operation information
    static void displayOperationInfo(const std::string& operation, const Matrix& m1, const Matrix& m2) {
        std::cout << "\n--- " << operation << " Operation ---\n";
        std::cout << "Matrix 1 (" << m1.getRows() << "x" << m1.getCols() << "):\n";
        m1.displayMatrix();
        std::cout << "Matrix 2 (" << m2.getRows() << "x" << m2.getCols() << "):\n";
        m2.displayMatrix();
    }

    static void displayOperationInfo(const std::string& operation, const Matrix& m1) {
        std::cout << "\n--- " << operation << " Operation ---\n";
        std::cout << "Input Matrix (" << m1.getRows() << "x" << m1.getCols() << "):\n";
        m1.displayMatrix();
    }
};

#endif // MATRIX_UTILITY_H