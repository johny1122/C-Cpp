/**
 * @file MlpNetwork.h
 * @author  Jonathan Birnbaum
 * @date 08/06/2020
 *
 * @brief MlpNetwork class declaration and documentation
 */

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE (4)

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784}, {64, 128}, {20, 64}, {10, 20}};
const MatrixDims biasDims[]    = {{128, 1}, {64, 1}, {20, 1},  {10, 1}};

/**
 * @brief MlpNetwork class - representing the neuron network
 */
class MlpNetwork
{
private:
    Dense _layer1;
    Dense _layer2;
    Dense _layer3;
    Dense _layer4;
public:

    /**
    * @brief constructor
    * @param weights: array of the 4 weight matrices
    * @param biases: array of the 4 bias matrices
    */
    MlpNetwork(const Matrix* weights, const Matrix* biases);

    /**
     * @brief operator ()
     * @param input: input matrix
     * @return digit struct with final result
     */
    Digit operator()(Matrix& input);

};

#endif // MLPNETWORK_H
