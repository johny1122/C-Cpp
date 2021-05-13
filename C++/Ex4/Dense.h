/**
 * @file Dense.h
 * @author  Jonathan Birnbaum
 * @date 08/06/2020
 *
 * @brief Dense class declaration and documentation
 */

#ifndef EX4_DENSE_H
#define EX4_DENSE_H

#include "Matrix.h"
#include "Activation.h"

/**
 * @brief Dense class - representing a layer in the net
 */
class Dense
{
private:
    Matrix _w;
    Matrix _bias;
    ActivationType _actType;

public:

    /**
     * @brief constructor
     * @param w: matrix of weights
     * @param bias: matrix of bias
     * @param actType: Activation type
     */
    Dense(const Matrix& w, const Matrix& bias, const ActivationType& actType);

    /**
     * @brief Getter - Weights
     * @return weights matrix
     */
    Matrix getWeights() const {return _w; }

    /**
     * @brief Getter - Bias
     * @return bias matrix
     */
    Matrix getBias() const {return _bias; }

    /**
     * @brief Getter - Activation
     * @return Activation object
     */
    Activation getActivation() const {return Activation (_actType); }

    /**
     * @brief operator ()
     * @param mat_input: input matrix
     * @return new matrix after layer activation
     */
    Matrix operator() (Matrix& mat_input);
};

#endif //EX4_DENSE_H
