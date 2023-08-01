//
// Created by liora on 24/08/2022.
//

#include "Dense.h"

Dense::Dense (const Matrix &weights, const Matrix &bias,
              activation_fp ActivationFunction)
    : _weights_matrix (weights), _bias_matrix (bias),
      _activation_func (ActivationFunction)
{}

Matrix Dense::get_weights () const
{
  return _weights_matrix;
}

Matrix Dense::get_bias () const
{
  return _bias_matrix;
}

activation_fp Dense::get_activation () const
{
  return _activation_func;
}

Matrix Dense::operator() (const Matrix &m)
{
  return _activation_func (_weights_matrix * m + _bias_matrix);
}
