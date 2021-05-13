/**
 * @file Activation.cpp
 * @author  Jonathan Birnbaum
 * @date 08/06/2020
 *
 * @brief Activation class implementation
 */

#include "Activation.h"
#include <iostream>
#include <cmath>

using std::endl;
using std::cerr;

/**
 * @brief Exit code for error
 */
#define EXIT_ERROR (1)

/**
 * @brief error massage
 */
#define ERROR_MSG_ACT_TYPE_INVALID "Error: Activation type is invalid"

/**
 * @brief constructor
 */
Activation::Activation(const ActivationType actType) : _actType(actType)
{
    if ((actType != Relu) && (actType != Softmax))
    {
        cerr << ERROR_MSG_ACT_TYPE_INVALID << endl;
        exit(EXIT_ERROR);
    }
}

/**
 * @brief operator ()
 */
Matrix Activation::operator()(Matrix& mat_input) const
{
    Matrix mat_with_active (mat_input.getRows(), mat_input.getCols());
    if (_actType == Relu)
    {
        for (int index = 0; index < matrixSize(mat_input); index++)
        {
            if (mat_input[index] >= 0)
            {
                mat_with_active[index] = mat_input[index];
            }
            // else - do nothing because Matrix initiates with zeros
        }
    }
    else if (_actType == Softmax)
    {
        float sum_exp = 0;
        for (int index = 0; index < matrixSize(mat_input); index++)
        {
            float result = std::exp(mat_input[index]);
            mat_with_active[index] = result;
            sum_exp += result;
        }
        mat_with_active = (1 / sum_exp) * mat_with_active;
    }
    return mat_with_active;
}
