#ifndef H_MATRIX
#define H_MATRIX
void inverse(float mat [3][3]);
void multVect33(float mat [3][3], float vect [3]);
void multVect44(float mat [4][4], float vect [4]);
float det(float mat [3][3]);
float* solveAppend44(float mat [4][4], float vect [4]);
float* solveAppend33(float mat [3][3], float vect [3]);
float* solve(float** mat, int rows, int cols);
float* multiply44(float A[4][4], float B[4][4]);
void swapRow(float** mat, int row1, int row2);
void scaleRow(float** mat, int cols, float scaleBy, int rowSum);
#endif
