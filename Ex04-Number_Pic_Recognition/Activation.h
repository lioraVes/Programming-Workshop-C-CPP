#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

//typedef for pointer to an activation function. Used in Dense.
typedef Matrix (*activation_fp) (const Matrix &m);

namespace activation
{
    /**
     * relu function.
     * @param m - the matrix.
     * @return a new matrix that contains only positive (or zeros) elements.
     */
    Matrix relu (const Matrix &m);
    /**
     * softmax function.
     * @param m - the matrix.
     * @return a new matrix- the probability distribution matrix.
     */
    Matrix softmax (const Matrix &m);
}
#endif //ACTIVATION_H