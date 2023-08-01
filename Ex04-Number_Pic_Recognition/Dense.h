#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

class Dense
{
 public:
  /**
   * the dense constructor.
   * @param weights -the weights matrix.
   * @param bias -the bias matrix.
   * @param ActivationFunction -the wanted activation matrix.
   */
  Dense (const Matrix &weights, const Matrix &bias,
         activation_fp ActivationFunction);
  /**
   * a getter function.
   * @return the weights matrix.
   */
  Matrix get_weights () const;
  /**
   * a getter function.
   * @return the bias matrix.
   */
  Matrix get_bias () const;
  /**
   * a getter function.
   * @return the activation function.
   */
  activation_fp get_activation () const;
  /**
   * applies the dense layer on an input matrix.
   * @param m- the input matrix.
   * @return the new matrix that the activation function returns.
   */
  Matrix operator() (const Matrix &m);
 private:
  Matrix _weights_matrix;
  Matrix _bias_matrix;
  activation_fp _activation_func;
};

#endif //DENSE_H
