// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

class Matrix
{
 public:
  /**
   * the matrix constructor, allocates an array of row*cols floats and
   * initialize it to contain zeros.
   * @param rows
   * @param cols
   */
  Matrix (int rows, int cols);
  /**
 * the default constructor, allocates an array of one float and initialize it
 * to zero.
 */
  Matrix ();
  /**
   * copy constructor.
   * @param m- the matrix
   */
  Matrix (const Matrix &m);
  /**
   * the class destructor- deletes the mat array.
   */
  ~Matrix ();
  /**
   * getter function.
   * @return number of rows.
   */
  int get_rows () const;
  /**
   * getter function.
   * @return number of cols.
   */
  int get_cols () const;

  /**
   * transpose the matrix.
   * @return reference to the transposed matrix.
   */
  Matrix &transpose ();
  /**
   * turns the matrix to a vector (one column).
   * @return reference to the vectorized matrix.
   */
  Matrix &vectorize ();
  /**
   * prints the matrix._mat values to cout.
   */
  void plain_print () const;
  /**
   * returns a matrix which is the element wise multiplication of the matrix
   * and the m matrix.
   * @param m- another matrix object.
   * @return the dot matrix.
   */
  Matrix dot (const Matrix &m) const;
  /**
   * calculates the norm of the Matrix.
   * @return the norm of the matrix.
   */
  float norm () const;
  /**
   * Matrix addition operator.
   * @param m - another matrix
   * @return a new matrix- the sum of the two matrix's.
   */
  Matrix operator+ (const Matrix &m) const;
  /**
   * assignment operator. with deep copy.
   * @param m - another matrix
   * @return reference to the matrix with the new data.
   */
  Matrix &operator= (const Matrix &m);
  /**
   * matrix multiplication operator.
   * @param m - another matrix.
   * @return a new matrix- the result of the multiplication.
   */
  Matrix operator* (const Matrix &m) const;
  /**
   * multiplication by scalar on the right.
   * @param c- the scalar.
   * @return the new matrix-result of the multiplication.
   */
  Matrix operator* (const float &c) const;
  /**
   * multiplication by scalar on the left.
   * @param c - the scalar.
   * @param m - the matrix we multiply by scalar.
   * @return the new matrix-result of the multiplication
   */
  friend Matrix operator* (float c, const Matrix &m);
  /**
   * matrix addition accumulation operator
   * @param m- another matrix
   * @return reference to the matrix with the new data.
   */
  Matrix &operator+= (const Matrix &m);
  /**
   * parenthesis indexing- const method- only allows 'access' to the entry
   * (without changing it).
   * @param i- row number
   * @param j-col number
   * @return the i,j entry in the matrix.
   */
  const float &operator() (int i, int j) const;
  /**
   * parenthesis indexing- non const method- allows to change the entry.
   * @param i-row number
   * @param j-col number
   * @return the i,j entry in the matrix.
   */
  float &operator() (int i, int j);
  /**
   * brackets indexing- const method-only allows 'access' to the element
   * (without changing it).
   * @param n- the wanted index
   * @return the n element in the matrix.
   */
  const float &operator[] (int n) const;
  /**
   * brackets indexing- non const method- allows to change the element.
   * @param n- the wanted index
   * @return the n element in the matrix.
   */
  float &operator[] (int n);
  /**
   * output stream operator, exports the matrix to the given output stream.
   * @param os-output stream.
   * @param matrix- the matrix.
   * @return the output stream.
   */
  friend std::ostream &operator<< (std::ostream &os, const Matrix &matrix);
  /**
   * input stream operator,reads input stream to the matrix fully.
   * @param is- input stream.
   * @param matrix- the matrix to fill.
   * @return the input stream.
   */
  friend std::istream &operator>> (std::istream &is, Matrix &matrix);

 private:
  int _rows;
  int _cols;
  float *_mat;
};

#endif //MATRIX_H