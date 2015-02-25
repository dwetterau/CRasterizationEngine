#include "matrix.h"
#include <math.h>
#include <stdlib.h>

void inverse(float mat [3][3]){
    float fdet = det(mat);
    float a11 = (mat[1][1]*mat[2][2]-mat[1][2]*mat[2][1])/fdet;
    float a12 = (mat[0][2]*mat[2][1]-mat[0][1]*mat[2][2])/fdet;
    float a13 = (mat[0][1]*mat[1][2]-mat[0][2]*mat[1][1])/fdet;
    float a21 = (mat[1][2]*mat[2][0]-mat[1][0]*mat[2][2])/fdet;
    float a22 = (mat[0][0]*mat[2][2]-mat[0][2]*mat[2][0])/fdet;
    float a23 = (mat[0][2]*mat[1][0]-mat[0][0]*mat[1][2])/fdet;
    float a31 = (mat[1][0]*mat[2][1]-mat[1][1]*mat[2][0])/fdet;
    float a32 = (mat[0][1]*mat[2][0]-mat[0][0]*mat[2][1])/fdet;
    float a33 = (mat[0][0]*mat[1][1]-mat[0][1]*mat[1][0])/fdet;
    mat[0][0] = a11;
    mat[0][1] = a12;
    mat[0][2] = a13;
    mat[1][0] = a21;
    mat[1][1] = a22;
    mat[1][2] = a23;
    mat[2][0] = a31;
    mat[2][1] = a32;
    mat[2][2] = a33;
}

float* solveAppend44(float mat [4][4], float vect [4]){
    float newMat [4][5]={{mat[0][0],mat[0][1],mat[0][2],mat[0][3],vect[0]},
                         {mat[1][0],mat[1][1],mat[1][2],mat[1][3],vect[1]},
                         {mat[2][0],mat[2][1],mat[2][2],mat[2][3],vect[2]},
                         {mat[3][0],mat[3][1],mat[3][2],mat[3][3],vect[3]}};
    
    float* mat_rows[4] = {newMat[0],newMat[1],newMat[2],newMat[3]};
    return solve(mat_rows,4,5);
}
float* solveAppend33(float mat [3][3], float vect [3]){
    float newMat [3][4] = { {mat[0][0],mat[0][1],mat[0][2],vect[0]},
                            {mat[1][0],mat[1][1],mat[1][2],vect[1]},
                            {mat[2][0],mat[2][1],mat[2][2],vect[2]}};
    float* mat_rows[3] = {newMat[0],newMat[1],newMat[2]};
    return solve(mat_rows,3,4);
}
float* solve(float** mat, int rows, int cols){
    int pivotCol,r,c;
    float* solution = (float *)malloc(rows*sizeof(float));
    for(pivotCol = 0; pivotCol<rows; pivotCol++){
        //find row to pivot on
        int pivotRow = -1;
        float pivotVal = 0.0;
        for(r = pivotVal; r < rows; r++){
            float val = fabs(mat[r][pivotCol]);
            if(pivotVal < val){
                pivotVal = val;
                pivotRow = r;
            }
        }
        if(pivotRow==-1)
            return solution;
        if(pivotRow!=pivotCol)
            swapRow(mat,pivotRow,pivotCol); 
        pivotRow = pivotCol;
        scaleRow(mat,cols,1/mat[pivotRow][pivotCol],pivotRow);
        float *pr = mat[pivotRow];
        for(r=0; r<rows; r++)
            if(r!=pivotRow){
                float *rest = mat[r];
                float scale = rest[pivotCol];
                if(scale == 0.0)
                    continue;
                for(c = pivotCol+1; c < cols; c++)
                    rest[c] -= pr[c]*scale;
                rest[pivotCol]=0;
            }
    }
    //get last column which is "solution"
    for(r=0;r<rows;r++){
            solution[r] = mat[r][cols-1];
    }
    return solution;
}

void swapRow(float** mat, int row1, int row2){
    float *tmp = mat[row1];
    mat[row1] = mat[row2];
    mat[row2] = tmp;
}

void scaleRow(float** mat, int cols, float scaleBy, int rowSum){
    float *rpSum = mat[rowSum];
    int c = 0;
    for(c = 0; c < cols; c++){
        rpSum[c]*=scaleBy;
    }
}

     //multVectMatrix
void multVect33(float mat [3][3], float v [3]){
    float t1 = v[0]*mat[0][0] + v[1]*mat[0][1] + v[2]*mat[0][2];
    float t2 = v[0]*mat[1][0] + v[1]*mat[1][1] + v[2]*mat[1][2];
    float t3 = v[0]*mat[2][0] + v[1]*mat[2][1] + v[2]*mat[2][2];
    v[0] = t1;
    v[1] = t2;
    v[2] = t3;
}   
void multVect44(float mat [4][4], float v [4]){
    float t1 = v[0]*mat[0][0] + v[1]*mat[0][1] + v[2]*mat[0][2] + v[3]*mat[0][3];
    float t2 = v[0]*mat[1][0] + v[1]*mat[1][1] + v[2]*mat[1][2] + v[3]*mat[1][3];
    float t3 = v[0]*mat[2][0] + v[1]*mat[2][1] + v[2]*mat[2][2] + v[3]*mat[2][3];
    float t4 = v[0]*mat[3][0] + v[1]*mat[3][1] + v[2]*mat[3][2] + v[3]*mat[3][3];
    v[0] = t1;
    v[1] = t2;
    v[2] = t3;
    v[3] = t4;
}
float det(float mat [3][3]){
    return  (mat[0][0])*(mat[1][1])*(mat[2][2])+
            (mat[1][0])*(mat[2][1])*(mat[0][2])+
            (mat[2][0])*(mat[0][1])*(mat[1][2])-
            (mat[0][2])*(mat[1][1])*(mat[2][0])-
            (mat[0][1])*(mat[1][0])*(mat[2][2])-
            (mat[0][0])*(mat[1][2])*(mat[2][1]);
}

float* multiply44(float A [4][4], float B [4][4]){
    float vector1 [4] = {B[0][0], B[1][0], B[2][0], B[3][0]};
    multVect44(A,vector1);
    float vector2 [4] = {B[0][1], B[1][1], B[2][1], B[3][1]};
    multVect44(A,vector2);
    float vector3 [4] = {B[0][2], B[1][2], B[2][2], B[3][2]};
    multVect44(A,vector3);
    float vector4 [4] = {B[0][3], B[1][3], B[2][3], B[3][3]};
    multVect44(A,vector4);
    //float* row1 = (float*)malloc(4*sizeof(float));
    //float* row2 = (float*)malloc(4*sizeof(float));
    //float* row3 = (float*)malloc(4*sizeof(float));
    //float* row4 = (float*)malloc(4*sizeof(float));
    //row1 = {vector1[0],vector2[0],vector3[0],vector4[0]};
    //row2 = {vector1[1],vector2[1],vector3[1],vector4[1]};
    //row3 = {vector1[2],vector2[2],vector3[2],vector4[2]};
    //row4 = {vector1[3],vector2[3],vector3[3],vector4[3]};
    float* M = (float*)malloc(16*sizeof(float));
    M[0] = vector1[0];
    M[1] = vector2[0];
    M[2] = vector3[0];
    M[3] = vector4[0];
    M[4] = vector1[1];
    M[5] = vector2[1];
    M[6] = vector3[1];
    M[7] = vector4[1];
    M[8] = vector1[2];
    M[9] = vector2[2];
    M[10] = vector3[2];
    M[11] = vector4[2];
    M[12] = vector1[3];
    M[13] = vector2[3];
    M[14] = vector3[3];
    M[15] = vector4[3];
    
    /*row1[0] = vector1[0];
    row1[1] = vector2[0];
    row1[2] = vector3[0];
    row1[3] = vector4[0];
    row2[0] = vector1[1];
    row2[1] = vector2[1];
    row2[2] = vector3[1];
    row2[3] = vector4[1];
    row3[0] = vector1[2];
    row3[1] = vector2[2];
    row3[2] = vector3[2];
    row3[3] = vector4[2];
    row4[0] = vector1[3];
    row4[1] = vector2[3];
    row4[2] = vector3[3];
    row4[3] = vector4[3];
    M[0] = row1;
    M[1] = row2;
    M[2] = row3;
    M[3] = row4;

    //M = {row1, row2, row3, row4}; */

    return M;
}
