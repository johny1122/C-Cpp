/**
 * @file Activation.h
 * @author  Jonathan Birnbaum
 * @date 08/06/2020
 *
 * @brief Activation class declaration and documentation
 */

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * @brief Activation class of function activating
 */
class Activation
{
private:
    ActivationType _actType;
public:

    /**
     * @brief constructor
     * @param actType: activation Type
     */
    explicit Activation(ActivationType actType);

    /**
     * @brief getter actType
     * @return activation type
     */
    ActivationType getActivationType() const {return _actType; }

    /**
     * @brief operator ()
     * @param mat_input: input matrix
     * @return copy of input matrix after activation
     */
    Matrix operator()(Matrix& mat_input) const;
};
#endif //ACTIVATION_H
