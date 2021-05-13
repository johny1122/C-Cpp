/**
 * @file Matrix.cpp
 * @author  Jonathan Birnbaum
 * @date 08/06/2020
 *
 * @brief Matrix class implementation
 */


#include <iostream>
#include "Matrix.h"

using std::cout;
using std::endl;
using std::cerr;
using std::ostream;

/**
 * @brief Exit code for error
 */
#define EXIT_ERROR (1)

/**
 * @brief size and value matrix in default constructor
 */
#define SIZE_OF_DEFAULT_MATRIX (1)
#define VALUE_OF_DEFAULT_MATRIX (0)

/**
 * @brief limit value to print "**" int << operator
 */
#define PRINT_LIMIT_VALUE (0.1f)

/**
 * @brief symbols to print in << operator
 */
#define PRINT_SYMBOL ("**")
#define PRINT_SPACES ("  ")

/**
 * @brief error massages
 */
#define ERROR_MSG_INVALID_ROW_COL "Error: Invalid input of rows or columns"
#define ERROR_MSG_NULL_ALLOC_POINTER "Error: Allocation didn't Succeed"
#define ERROR_MSG_MATRIX_SIZE_MULTIPLICATION "Error: Matrices size invalid for matrix multiplication"
#define ERROR_MSG_MATRIX_SIZE_ADDITION "Error: Matrices size invalid for matrix addition"
#define ERROR_MSG_INDEX_OUT_OF_RANGE "Error: Input indexes are out of range"
#define ERROR_MSG_INPUT_FILE "Error: There was a problem with the input file"
#define ERROR_MSG_MISSING_EOF "Error: Didn't finish to read all data from input file"



//----------------------- CONSTRUCTORS-DESTRUCTOR ---------------------------

/**
 * @brief normal constructor
 */
Matrix::Matrix(const int& rows, const int& cols) : _matrixDims{}, _matrix{}
{
    if ((rows <= 0) || (cols <= 0))
    {
        cerr << ERROR_MSG_INVALID_ROW_COL << endl;
        exit(EXIT_ERROR);
    }
    _matrixDims.rows = rows;
    _matrixDims.cols = cols;
    _matrix = new float [rows * cols];
    if (_matrix == nullptr)
    {
        cerr << ERROR_MSG_NULL_ALLOC_POINTER << endl;
        exit(EXIT_ERROR);
    }
    for (int index = 0; index < matrixSize(*this); index++)
    {
        _matrix[index] = 0; // init matrix elements with zeros
    }
}

/**
 * @brief default constructor
 */
Matrix::Matrix() : _matrixDims{}, _matrix{}
{
    _matrixDims.rows = SIZE_OF_DEFAULT_MATRIX;
    _matrixDims.cols = SIZE_OF_DEFAULT_MATRIX;
    _matrix = new float [SIZE_OF_DEFAULT_MATRIX];
    if (_matrix == nullptr)
    {
        cerr << ERROR_MSG_NULL_ALLOC_POINTER << endl;
        exit(EXIT_ERROR);
    }
    _matrix[0] = VALUE_OF_DEFAULT_MATRIX; // init the single element with zero
}

/**
 * @brief copy constructor
 */
Matrix::Matrix(const Matrix &rhs) : _matrixDims{}, _matrix{}
{
    _matrixDims.rows = rhs._matrixDims.rows;
    _matrixDims.cols = rhs._matrixDims.cols;
    _matrix = new float [matrixSize(rhs)];
    if (_matrix == nullptr)
    {
        cerr << ERROR_MSG_NULL_ALLOC_POINTER << endl;
        exit(EXIT_ERROR);
    }
    for (int index = 0; index < matrixSize(*this); index++)
    {
        _matrix[index] = rhs._matrix[index];
    }
}

/**
 * @brief destructor
 */
Matrix::~Matrix()
{
    delete [] _matrix;
}

//------------------------ GENERAL -----------------------------

/**
 * @brief Matrix total size
 */
int matrixSize(const Matrix &m)
{
    return (m.getRows() * m.getCols());
}

//------------------------- METHODS -----------------------------

/**
 * @brief  change matrix to vector
 */
Matrix& Matrix::vectorize()
{
    _matrixDims.rows = (matrixSize(*this));
    _matrixDims.cols = 1;
    return *this;
}

/**
 * @brief print matrix
 */
void Matrix::plainPrint() const
{
    for (int row = 0; row < _matrixDims.rows; row++)
    {
        for (int col = 0; col < _matrixDims.cols; col++)
        {
            cout << _matrix[(row * _matrixDims.cols) + col] << " ";
        }
        cout << endl;
    }
}

//----------------------- OPERATORS ---------------------------

/**
 * @brief operator = (rhs matrix)
 */
Matrix& Matrix::operator=(const Matrix& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }
    delete [] _matrix;
    _matrixDims.rows = rhs._matrixDims.rows;
    _matrixDims.cols = rhs._matrixDims.cols;
    _matrix = new float [matrixSize(*this)];
    if (_matrix == nullptr)
    {
        cerr << ERROR_MSG_NULL_ALLOC_POINTER << endl;
        exit(EXIT_ERROR);
    }
    for (int index = 0; index < matrixSize(*this); index++)
    {
        _matrix[index] = rhs._matrix[index];
    }
    return *this;
}

/**
 * @brief operator * (matrix multiplication)
 */
Matrix Matrix::operator*(const Matrix &rhs) const
{
    if (_matrixDims.cols != rhs._matrixDims.rows)
    {
        cerr << ERROR_MSG_MATRIX_SIZE_MULTIPLICATION << endl;
        exit(EXIT_ERROR);
    }

    Matrix multi_mat(_matrixDims.rows, rhs._matrixDims.cols);

    for (int row = 0; row < _matrixDims.rows; row++) // for each row in this matrix
    {
        for (int col = 0; col < rhs._matrixDims.cols; col++) // for each col in rhs matrix
        {
            float index_sum = 0;
            for (int index = 0; index < _matrixDims.cols; index++) // for each element in multiplication
            {
                index_sum += _matrix[(row * _matrixDims.cols) + index] * rhs._matrix[(index * rhs._matrixDims.cols) +
                                                                                     col];
            }
            multi_mat._matrix[(row * rhs._matrixDims.cols) + col] = index_sum;
        }
    }
    return multi_mat;
}

/**
 * @brief operator * (scalar multiplication on the right)
 */
Matrix Matrix::operator*(const float& c) const
{
    Matrix multi_mat(_matrixDims.rows, _matrixDims.cols);
    for (int index = 0; index < matrixSize(*this); index++)
    {
        multi_mat._matrix[index] = _matrix[index] * c;
    }
    return multi_mat;
}

/**
 * @brief operator * (scalar multiplication on the left)
 */
Matrix operator*(const float &c, const Matrix &rhs)
{
    Matrix multi_mat(rhs._matrixDims.rows, rhs._matrixDims.cols);
    for (int index = 0; index < matrixSize(rhs); index++)
    {
        multi_mat._matrix[index] = rhs._matrix[index] * c;
    }
    return multi_mat;
}

/**
 * @brief operator +
 */
Matrix Matrix::operator+(const Matrix &rhs) const
{
    if ((_matrixDims.rows != rhs._matrixDims.rows) || (_matrixDims.cols != rhs._matrixDims.cols))
    {
        cerr << ERROR_MSG_MATRIX_SIZE_ADDITION << endl;
        exit(EXIT_ERROR);
    }
    Matrix mat_add (_matrixDims.rows, _matrixDims.cols);
    for (int index = 0; index < matrixSize(*this); index++)
    {
        mat_add._matrix[index] = _matrix[index] + rhs._matrix[index];
    }
    return mat_add;
}

/**
 * @brief operator +=
 */
Matrix& Matrix::operator+=(const Matrix &rhs)
{
    if ((_matrixDims.rows != rhs._matrixDims.rows) || (_matrixDims.cols != rhs._matrixDims.cols))
    {
        cerr << ERROR_MSG_MATRIX_SIZE_ADDITION << endl;
        exit(EXIT_ERROR);
    }
    for (int index = 0; index < matrixSize(*this); index++)
    {
        _matrix[index] += rhs._matrix[index];
    }
    return *this;
}

/**
 * @brief operator () (just returning)
 */
float Matrix::operator()(const int i, const int j) const
{
    if ((i < 0) || (i >= _matrixDims.rows) || (j < 0) || (j >= _matrixDims.cols))
    {
        cerr << ERROR_MSG_INDEX_OUT_OF_RANGE << endl;
        exit(EXIT_ERROR);
    }
    return _matrix[(i * _matrixDims.cols) + j];
}

/**
 * @brief operator () (with editing)
 */
float& Matrix::operator()(int i, int j)
{
    if ((i < 0) || (i >= _matrixDims.rows) || (j < 0) || (j >= _matrixDims.cols))
    {
        cerr << ERROR_MSG_INDEX_OUT_OF_RANGE << endl;
        exit(EXIT_ERROR);
    }
    return _matrix[(i * _matrixDims.cols) + j];
}

/**
 * @brief operator [] (with editing)
 */
float& Matrix::operator[](const int i)
{
    if ((i < 0) || (i >= matrixSize(*this)))
    {
        cerr << ERROR_MSG_INDEX_OUT_OF_RANGE << endl;
        exit(EXIT_ERROR);
    }
    return _matrix[i];
}

/**
 * @brief operator [] (just returning)
 */
float Matrix::operator[](const int i) const
{
    if ((i < 0) || (i >= matrixSize(*this)))
    {
        cerr << ERROR_MSG_INDEX_OUT_OF_RANGE << endl;
        exit(EXIT_ERROR);
    }
    return _matrix[i];
}

/**
 * @brief operator >>
 */
std::istream& operator>>(std::istream &file, Matrix &rhs)
{
    unsigned long size_file = rhs._matrixDims.rows * rhs._matrixDims.cols * sizeof(float);
    file.read((char*) rhs._matrix, size_file);
    if (!file.good())
    {
        cerr << ERROR_MSG_INPUT_FILE << endl;
        exit(EXIT_ERROR);
    }
    if (file.peek() != EOF)
    {
        cerr << ERROR_MSG_MISSING_EOF << endl;
        exit(EXIT_ERROR);
    }
    return file;
}

/**
 * @brief operator <<
 */
std::ostream& operator<<(std::ostream& output, Matrix &rhs)
{
    for (int row = 0; row < rhs._matrixDims.rows; row++)
    {
        for (int col = 0; col < rhs._matrixDims.cols; col++)
        {
            if (rhs(row, col) <= PRINT_LIMIT_VALUE)
            {
                output << PRINT_SPACES;
            }
            else
            {
                output << PRINT_SYMBOL;
            }
        }
        output << endl;
    }
    return output;
}
