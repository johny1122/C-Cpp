/**
 * @file MlpNetwork.cpp
 * @author  Jonathan Birnbaum
 * @date 08/06/2020
 *
 * @brief MlpNetwork class implementation
 */

#include "MlpNetwork.h"


/**
 * @brief constructor
 */
MlpNetwork::MlpNetwork(const Matrix* weights, const Matrix* biases) :
                        _layer1(weights[0], biases[0], Relu),
                        _layer2(weights[1], biases[1], Relu),
                        _layer3(weights[2], biases[2], Relu),
                        _layer4(weights[3], biases[3], Softmax) {}

/**
 * @brief operator ()
 */
Digit MlpNetwork::operator()(Matrix &input)
{
    Matrix mat_after_layer = _layer1(input);
    mat_after_layer = _layer2(mat_after_layer);
    mat_after_layer = _layer3(mat_after_layer);
    mat_after_layer = _layer4(mat_after_layer);


    float max_probability = 0;
    int max_value = 0;
    for (int index = 0; index < matrixSize(mat_after_layer); index++)
    {
        if (mat_after_layer[index] > max_probability)
        {
            max_probability = mat_after_layer[index];
            max_value = index;
        }
    }
    Digit final_result;
    final_result.value = max_value;
    final_result.probability = max_probability;
    return final_result;
}
