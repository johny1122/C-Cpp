/**
 * @file Dense.cpp
 * @author  Jonathan Birnbaum
 * @date 08/06/2020
 *
 * @brief Dense class implementation
 */

#include "Dense.h"

/**
 * @brief constructor
 */
Dense::Dense(const Matrix& w, const Matrix& bias, const ActivationType& actType) : _w(w), _bias(bias), _actType(actType)
{}

/**
 * @brief operator ()
 */
Matrix Dense::operator()(Matrix &mat_input)
{
    Matrix mat_layer = ((_w * mat_input) + _bias);
    Activation activator (_actType);
    return activator(mat_layer);
}
