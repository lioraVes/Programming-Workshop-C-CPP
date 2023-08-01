//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit
{
    unsigned int value;
    float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

class MlpNetwork
{
 public:
  /**
   * the mlpnetwork constructor.
   * @param weights - the array of weights matrix.
   * @param biases - the array of biases matrix.
   */
  MlpNetwork (Matrix weights[], Matrix biases[]);
  /**
   * applies the entire network on the input matrix.
   * @param img -the input matrix.
   * @return the digit- its value and its probability.
   */
  digit operator() (const Matrix &img);
 private:
  Matrix _weights[MLP_SIZE];
  Matrix _biases[MLP_SIZE];
};

#endif // MLPNETWORK_H