/**
 * @file Matrix.h
 * @author  Jonathan Birnbaum
 * @date 08/06/2020
 *
 * @brief Matrix class declaration and documentation
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;


/**
 * @brief Matrix class - creating, getting, editing and applying operators
 */
class Matrix
{
private:
    MatrixDims _matrixDims;
    float* _matrix;
public:

//----------------------- CONSTRUCTORS-DESTRUCTOR ---------------------------

    /**
     * @brief normal constructor
     * @param rows: number of rows in matrix
     * @param cols: number of columns in matrix
     */
    Matrix(const int& rows, const int& cols);

    /**
     * @brief default constructor
     */
    Matrix();

    /**
     * @brief copy constructor
     * @param rhs: matrix to copy from
     */
    Matrix(const Matrix& rhs);

    /**
     * @brief destructor
     */
    ~Matrix();

//---------------------- METHODS --------------------------

    /**
     * @brief getter rows
     * @return rows value
     */
    int getRows() const {return _matrixDims.rows; }

    /**
     * @brief getter columns
     * @return cols value
     */
    int getCols() const {return _matrixDims.cols; }


    /**
     * @brief change the matrix to column vector
     * @return: this matrix by reference
     */
    Matrix& vectorize();

    /**
     * @brief prints the matrix
     */
    void plainPrint() const;

//------------------- OPERATORS ------------------------
    /**
     * @brief operator = (assignment matrix)
     * @param rhs: matrix to copy from
     * @return this matrix by reference
     */
    Matrix& operator=(const Matrix& rhs);

    /**
     * @brief operator * (matrix multiplication)
     * @param rhs: matrix to multiply with
     * @return new matrix with the multiplication
     */
    Matrix operator*(const Matrix& rhs) const;

    /**
     * @brief operator * (scalar multiplication on the right)
     * @param c: scalar to multiply with
     * @return new matrix with the multiplication
     */
    Matrix operator*(const float& c) const;

    /**
     * @brief operator * (scalar multiplication on the left)
     * @param c: scalar to multiply with
     * @return new matrix with the multiplication
     */
    friend Matrix operator*(const float& c, const Matrix& rhs);

    /**
     * @brief operator + with 2 matrices
     * @param rhs: matrix to add from
     * @return new matrix after addition
     */
    Matrix operator+(const Matrix& rhs) const;

    /**
     * @brief operator +=
     * @param rhs: matrix to add from
     * @return new matrix after addition
     */
    Matrix& operator+=(const Matrix& rhs);

    /**
     * @brief operator () (just returning)
     * @param i: row index
     * @param j: column index
     * @return element in (i,j)
     */
    float operator()(int i, int j) const;

    /**
     * @brief operator () (with editing)
     * @param i: row index
     * @param j: column index
     * @return element in (i,j) by reference
     */
    float& operator()(int i, int j);

    /**
     * @brief operator [] (just returning)
     * @param i: i'th element index
     * @return element value
     */
    float operator[](int i) const;

    /**
     * @brief operator [] (with editing)
     * @param i: i'th element index
     * @return element value by reference
     */
    float& operator[](int i);

    /**
     * @brief operator >>
     * @param file: stream to read from
     * @param rhs: matrix to insert elements
     * @return file stream by reference
     */
    friend std::istream& operator>>(std::istream& file, Matrix &rhs);

    /**
     * @brief operator <<
     * @param screen: stream to print to
     * @param rhs: matrix to insert elements
     * @return out stream by reference
     */
    friend std::ostream& operator<<(std::ostream& output, Matrix &rhs);
};

/**
 * @brief calculate matrix total elements amount
 * @param m: matrix given
 * @return matrix zie
 */
int matrixSize(const Matrix &m);

#endif //MATRIX_H
