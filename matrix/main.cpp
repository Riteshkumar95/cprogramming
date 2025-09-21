#include "utility.h"
#include <iostream>

using namespace std;

void printMenu() {
    cout << "\n=== Matrix Operations Menu ===\n";
    cout << "1. Matrix Addition\n";
    cout << "2. Matrix Subtraction\n";
    cout << "3. Matrix Multiplication\n";
    cout << "4. Scalar Multiplication\n";
    cout << "5. Matrix Transpose\n";
    cout << "6. Create Identity Matrix\n";
    cout << "7. Check if Matrices are Equal\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

void performAddition() {
    int rows, cols;
    
    cout << "\n--- Matrix Addition ---\n";
    cout << "Enter dimensions for matrices (rows cols): ";
    cin >> rows >> cols;
    
    if (rows <= 0 || cols <= 0) {
        cout << "Invalid dimensions!\n";
        return;
    }
    
    Matrix matrix1(rows, cols);
    Matrix matrix2(rows, cols);
    
    cout << "\nFor First Matrix:\n";
    matrix1.inputMatrix();
    
    cout << "\nFor Second Matrix:\n";
    matrix2.inputMatrix();
    
    // Display operation info
    MatrixOperations::displayOperationInfo("Addition", matrix1, matrix2);
    
    // Perform addition using MatrixOperations class
    Matrix result = MatrixOperations::add(matrix1, matrix2);
    
    if (!result.isEmpty()) {
        cout << "Result of Addition:\n";
        result.displayMatrix();
    }
}

void performSubtraction() {
    int rows, cols;
    
    cout << "\n--- Matrix Subtraction ---\n";
    cout << "Enter dimensions for matrices (rows cols): ";
    cin >> rows >> cols;
    
    if (rows <= 0 || cols <= 0) {
        cout << "Invalid dimensions!\n";
        return;
    }
    
    Matrix matrix1(rows, cols);
    Matrix matrix2(rows, cols);
    
    cout << "\nFor First Matrix:\n";
    matrix1.inputMatrix();
    
    cout << "\nFor Second Matrix:\n";
    matrix2.inputMatrix();
    
    // Display operation info
    MatrixOperations::displayOperationInfo("Subtraction", matrix1, matrix2);
    
    // Perform subtraction using MatrixOperations class
    Matrix result = MatrixOperations::subtract(matrix1, matrix2);
    
    if (!result.isEmpty()) {
        cout << "Result of Subtraction (Matrix1 - Matrix2):\n";
        result.displayMatrix();
    }
}

void performMultiplication() {
    int rows1, cols1, rows2, cols2;
    
    cout << "\n--- Matrix Multiplication ---\n";
    cout << "Enter dimensions for first matrix (rows cols): ";
    cin >> rows1 >> cols1;
    
    cout << "Enter dimensions for second matrix (rows cols): ";
    cin >> rows2 >> cols2;
    
    if (rows1 <= 0 || cols1 <= 0 || rows2 <= 0 || cols2 <= 0) {
        cout << "Invalid dimensions!\n";
        return;
    }
    
    Matrix matrix1(rows1, cols1);
    Matrix matrix2(rows2, cols2);
    
    cout << "\nFor First Matrix (" << rows1 << "x" << cols1 << "):\n";
    matrix1.inputMatrix();
    
    cout << "\nFor Second Matrix (" << rows2 << "x" << cols2 << "):\n";
    matrix2.inputMatrix();
    
    // Display operation info
    MatrixOperations::displayOperationInfo("Multiplication", matrix1, matrix2);
    
    // Perform multiplication using MatrixOperations class
    Matrix result = MatrixOperations::multiply(matrix1, matrix2);
    
    if (!result.isEmpty()) {
        cout << "Result of Multiplication:\n";
        result.displayMatrix();
    }
}

void performScalarMultiplication() {
    int rows, cols, scalar;
    
    cout << "\n--- Scalar Multiplication ---\n";
    cout << "Enter dimensions for matrix (rows cols): ";
    cin >> rows >> cols;
    
    if (rows <= 0 || cols <= 0) {
        cout << "Invalid dimensions!\n";
        return;
    }
    
    cout << "Enter scalar value: ";
    cin >> scalar;
    
    Matrix matrix1(rows, cols);
    
    cout << "\nFor Matrix:\n";
    matrix1.inputMatrix();
    
    // Display operation info
    MatrixOperations::displayOperationInfo("Scalar Multiplication", matrix1);
    cout << "Scalar value: " << scalar << "\n";
    
    // Perform scalar multiplication using MatrixOperations class
    Matrix result = MatrixOperations::scalarMultiply(matrix1, scalar);
    
    if (!result.isEmpty()) {
        cout << "Result of Scalar Multiplication:\n";
        result.displayMatrix();
    }
}

void performTranspose() {
    int rows, cols;
    
    cout << "\n--- Matrix Transpose ---\n";
    cout << "Enter dimensions for matrix (rows cols): ";
    cin >> rows >> cols;
    
    if (rows <= 0 || cols <= 0) {
        cout << "Invalid dimensions!\n";
        return;
    }
    
    Matrix matrix1(rows, cols);
    
    cout << "\nFor Matrix:\n";
    matrix1.inputMatrix();
    
    // Display operation info
    MatrixOperations::displayOperationInfo("Transpose", matrix1);
    
    // Perform transpose using MatrixOperations class
    Matrix result = MatrixOperations::transpose(matrix1);
    
    if (!result.isEmpty()) {
        cout << "Result of Transpose:\n";
        result.displayMatrix();
    }
}

void createIdentityMatrix() {
    int size;
    
    cout << "\n--- Create Identity Matrix ---\n";
    cout << "Enter size of identity matrix: ";
    cin >> size;
    
    if (size <= 0) {
        cout << "Invalid size!\n";
        return;
    }
    
    // Create identity matrix using MatrixOperations class
    Matrix identity = MatrixOperations::createIdentityMatrix(size);
    
    if (!identity.isEmpty()) {
        cout << "Identity Matrix (" << size << "x" << size << "):\n";
        identity.displayMatrix();
    }
}

void checkMatrixEquality() {
    int rows, cols;
    
    cout << "\n--- Check Matrix Equality ---\n";
    cout << "Enter dimensions for matrices (rows cols): ";
    cin >> rows >> cols;
    
    if (rows <= 0 || cols <= 0) {
        cout << "Invalid dimensions!\n";
        return;
    }
    
    Matrix matrix1(rows, cols);
    Matrix matrix2(rows, cols);
    
    cout << "\nFor First Matrix:\n";
    matrix1.inputMatrix();
    
    cout << "\nFor Second Matrix:\n";
    matrix2.inputMatrix();
    
    // Display matrices
    cout << "\nFirst Matrix:\n";
    matrix1.displayMatrix();
    
    cout << "Second Matrix:\n";
    matrix2.displayMatrix();
    
    // Check equality using MatrixOperations class
    bool areEqual = MatrixOperations::isEqual(matrix1, matrix2);
    
    cout << "Matrices are " << (areEqual ? "EQUAL" : "NOT EQUAL") << "\n";
}

void demonstrateClassSeparation() {
    cout << "\n=== Demonstration of Class Separation ===\n";
    
    // Matrix class handles data structure
    Matrix m1(2, 2);
    Matrix m2(2, 2);
    
    // Set some sample values using Matrix class methods
    m1.setElement(0, 0, 1); m1.setElement(0, 1, 2);
    m1.setElement(1, 0, 3); m1.setElement(1, 1, 4);
    
    m2.setElement(0, 0, 5); m2.setElement(0, 1, 6);
    m2.setElement(1, 0, 7); m2.setElement(1, 1, 8);
    
    cout << "Sample Matrix 1 (created using Matrix class):\n";
    m1.displayMatrix();
    
    cout << "Sample Matrix 2 (created using Matrix class):\n";
    m2.displayMatrix();
    
    // MatrixOperations class handles mathematical operations
    cout << "Operations performed using MatrixOperations class:\n\n";
    
    Matrix sum = MatrixOperations::add(m1, m2);
    cout << "Addition Result:\n";
    sum.displayMatrix();
    
    Matrix product = MatrixOperations::multiply(m1, m2);
    cout << "Multiplication Result:\n";
    product.displayMatrix();
    
    Matrix transpose = MatrixOperations::transpose(m1);
    cout << "Transpose of Matrix 1:\n";
    transpose.displayMatrix();
    
    Matrix identity = MatrixOperations::createIdentityMatrix(3);
    cout << "3x3 Identity Matrix:\n";
    identity.displayMatrix();
    
    cout << "Is Matrix 1 square? " << (MatrixOperations::isSquare(m1) ? "Yes" : "No") << "\n";
    cout << "Are matrices equal? " << (MatrixOperations::isEqual(m1, m2) ? "Yes" : "No") << "\n\n";
}

int main() {
    int choice;
    
    cout << "Matrix Operations Program with Separated Classes\n";
    cout << "===============================================\n";
    
    // Demonstrate class separation first
    demonstrateClassSeparation();
    
    while (true) {
        printMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                performAddition();
                break;
                
            case 2:
                performSubtraction();
                break;
                
            case 3:
                performMultiplication();
                break;
                
            case 4:
                performScalarMultiplication();
                break;
                
            case 5:
                performTranspose();
                break;
                
            case 6:
                createIdentityMatrix();
                break;
                
            case 7:
                checkMatrixEquality();
                break;
                
            case 8:
                cout << "Exiting program. Goodbye!\n";
                return 0;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    
    return 0;
}