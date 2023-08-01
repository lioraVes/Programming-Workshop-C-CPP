//
// Created by liora on 24/08/2022.
//
#include "Activation.h"
#include <cmath>

Matrix activation::relu (const Matrix &m)
{
  Matrix new_mat (m.get_rows (), m.get_cols ());
  for (int i = 0; i < m.get_rows (); i++)
  {
    for (int j = 0; j < m.get_cols (); j++)
    {
      if (m (i, j) < 0)
      {
        new_mat (i, j) = 0.0;
      }
      else
      {
        new_mat (i, j) = m (i, j);
      }
    }
  }
  return new_mat;
}

Matrix activation::softmax (const Matrix &m)
{
  Matrix new_mat (m.get_rows (), m.get_cols ());
  float exp_sum = 0;
  for (int i = 0; i < m.get_rows (); i++)
  {
    for (int j = 0; j < m.get_cols (); j++)
    {
      exp_sum += std::exp (m (i, j));
      new_mat (i, j) = std::exp (m (i, j));
    }
  }
  new_mat = (1 / exp_sum) * new_mat;
  return new_mat;
}
