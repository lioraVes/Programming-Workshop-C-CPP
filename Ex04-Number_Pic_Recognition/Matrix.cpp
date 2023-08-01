//
// Created by liora on 24/08/2022.
//
#include "Matrix.h"
#include <cmath>
#include <cstring>

#define RUNTIME_ERROR_MSG "Invalid input!"
#define OUT_OF_RANGE_ERROR_MSG "Out of range error!"
#define LENGTH_ERROR_MSG "Length error!"
#define INVALID_ISTREAM_MSG "Invalid input-stream!"
#define INVALID_ISTREAM_SIZE "Invalid input-stream size!"
#define MIN_PIXEL_VAL 0.1

Matrix::Matrix (int rows, int cols) : _rows (rows), _cols (cols)
{
  if (rows <= 0 || cols <= 0)
  {
    throw std::length_error (LENGTH_ERROR_MSG);
  }
  _mat = new float[rows * cols];
  for (int i = 0; i < rows * cols; i++)
  {
    _mat[i] = 0;
  }
}

Matrix::Matrix () : _rows (1), _cols (1), _mat (new float[1])
{
  _mat[0] = 0;
}

Matrix::Matrix (const Matrix &m)
{
  _rows = m._rows;
  _cols = m._cols;
  _mat = new float[m._rows * m._cols];
  for (int i = 0; i < m._rows * m._cols; i++)
  {
    _mat[i] = m._mat[i];
  }
}

Matrix::~Matrix ()
{
  delete[] _mat;
}

int Matrix::get_rows () const
{
  return _rows;
}

int Matrix::get_cols () const
{
  return _cols;
}

Matrix &Matrix::transpose ()
{
  Matrix tmp = *this;
  _cols = tmp._rows;
  _rows = tmp._cols;

  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
    {
      (*this) (i, j) = tmp (j, i);
    }
  }
  return *this;
}

Matrix &Matrix::vectorize ()
{
  int old_cols = get_cols ();
  int old_rows = get_rows ();
  _cols = 1;
  _rows = old_cols * old_rows;
  return *this;
}

void Matrix::plain_print () const
{
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
    {
      std::cout << (*this) (i, j) << " ";
    }
    std::cout << std::endl;
  }
}

Matrix Matrix::dot (const Matrix &m) const
{
  if (_rows != m._rows || _cols != m._cols)
  {
    throw std::length_error (LENGTH_ERROR_MSG);
  }
  Matrix dot_matrix (_rows, _cols);
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
    {
      dot_matrix (i, j) = (*this) (i, j) * m (i, j);
    }
  }
  return dot_matrix;
}

float Matrix::norm () const
{
  float sum = 0;
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
    {
      sum += (*this) (i, j) * (*this) (i, j);
    }
  }
  return std::sqrt (sum);
}

const float &Matrix::operator() (int i, int j) const
{
  if (i < 0 || i >= _rows || j < 0 || j >= _cols)
  {
    throw std::out_of_range (OUT_OF_RANGE_ERROR_MSG);
  }
  return _mat[i * _cols + j];
}

float &Matrix::operator() (int i, int j)
{
  if (i < 0 || i >= _rows || j < 0 || j >= _cols)
  {
    throw std::out_of_range (OUT_OF_RANGE_ERROR_MSG);
  }
  return _mat[i * _cols + j];
}

const float &Matrix::operator[] (int n) const
{
  if (_rows * _cols <= n || n < 0)
  {
    throw std::out_of_range (OUT_OF_RANGE_ERROR_MSG);
  }
  return _mat[n];
}

float &Matrix::operator[] (int n)
{
  if (_rows * _cols <= n || n < 0)
  {
    throw std::out_of_range (OUT_OF_RANGE_ERROR_MSG);
  }
  return _mat[n];
}

Matrix Matrix::operator+ (const Matrix &m) const
{
  if (_cols != m._cols || _rows != m._rows)
  {
    throw std::length_error (LENGTH_ERROR_MSG);
  }
  Matrix c (_rows, _cols);
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
    {
      c (i, j) = (*this) (i, j) + (m) (i, j);
    }
  }
  return c;
}

Matrix &Matrix::operator= (const Matrix &m)
{
  if (this == &m)
  {
    return *this;
  }
  delete[] _mat;
  _cols = m._cols;
  _rows = m._rows;
  _mat = new float[m._rows * m._cols];
  for (int i = 0; i < _rows * _cols; i++)
  {
    std::memcpy (&_mat[i], &m._mat[i], sizeof (float));
  }
  return *this;
}

Matrix Matrix::operator* (const Matrix &m) const
{
  if (_cols != m._rows)
  {
    throw std::length_error (LENGTH_ERROR_MSG);
  }
  Matrix multiply (_rows, m._cols);
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < m._cols; j++)
    {
      float sum = 0;
      for (int k = 0; k < _cols; k++)
      {
        sum += (*this) (i, k) * m (k, j);
      }
      multiply (i, j) = sum;
    }
  }
  return multiply;
}

Matrix Matrix::operator* (const float &c) const
{
  Matrix multiply (_rows, _cols);
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
    {
      multiply (i, j) = (*this) (i, j) * c;
    }
  }
  return multiply;
}

Matrix &Matrix::operator+= (const Matrix &m)
{
  if (_rows != m._rows || _cols != m._cols)
  {
    throw std::length_error (LENGTH_ERROR_MSG);
  }
  (*this) = (*this) + m;
  return *this;
}

Matrix operator* (float c, const Matrix &m)
{
  return m * c;
}

std::ostream &operator<< (std::ostream &os, const Matrix &matrix)
{
  for (int i = 0; i < matrix.get_rows (); i++)
  {
    for (int j = 0; j < matrix.get_cols (); j++)
    {
      if (matrix (i, j) > MIN_PIXEL_VAL)
      {
        os << "**";
      }
      else
      {
        os << "  ";
      }
    }
    os << "\n";
  }
  return os;
}

std::istream &operator>> (std::istream &is, Matrix &matrix)
{
  if (!is.good ())
  {
    throw std::runtime_error (INVALID_ISTREAM_MSG);
  }
  auto matrix_size_bytes = (long int) (matrix.get_cols () * matrix.get_rows ()
                                       * sizeof (float));
  is.seekg (0, std::ios_base::end);
  long int file_size_bytes = is.tellg ();
  if (file_size_bytes != matrix_size_bytes)
  {
    throw std::runtime_error (INVALID_ISTREAM_SIZE);
  }
  is.seekg (0, std::ios_base::beg);
  if (!is.read ((char *) matrix._mat, matrix_size_bytes))
  {
    throw std::runtime_error (RUNTIME_ERROR_MSG);
  }
  return is;
}