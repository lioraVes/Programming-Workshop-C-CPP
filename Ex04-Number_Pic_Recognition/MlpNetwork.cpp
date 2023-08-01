//
// Created by liora on 24/08/2022.
//

#include "MlpNetwork.h"

#define FINAL_VECTOR_SIZE 10

MlpNetwork::MlpNetwork (Matrix weights[], Matrix biases[])
{
  for (int i = 0; i < MLP_SIZE; i++)
  {
    _weights[i] = weights[i];
  }
  for (int i = 0; i < MLP_SIZE; i++)
  {
    _biases[i] = biases[i];
  }
}

digit MlpNetwork::operator() (const Matrix &img)
{
  Dense first_layer (_weights[0], _biases[0], activation::relu);
  Dense second_layer (_weights[1], _biases[1], activation::relu);
  Dense third_layer (_weights[2], _biases[2], activation::relu);
  Dense fourth_layer (_weights[3], _biases[3], activation::softmax);

  Matrix first_output = first_layer (img);
  Matrix second_output = second_layer (first_output);
  Matrix third_output = third_layer (second_output);
  Matrix fourth_output = fourth_layer (third_output);

  float max_prob = 0;
  unsigned int max_val = 0;
  for (int i = 0; i < FINAL_VECTOR_SIZE; i++)
  {
    if (fourth_output[i] > max_prob)
    {
      max_prob = fourth_output[i];
      max_val = i;
    }
  }
  digit final_digit = {max_val, max_prob};
  return final_digit;
}