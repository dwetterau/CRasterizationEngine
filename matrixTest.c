#include "matrix.h"
#include <stdio.h>
#include <SDL/SDL.h>
void main(){
    float mat [3][3] = {{1,0,0},{0,2,0},{0,0,1}};
    float v[] = {1,1,1};
    
    float mat2 [3][3] = {{1,0,0},{0,2,0},{0,0,1}};
    float v2[] = {1,1,1};
   //printf("%f %f %f\n%f %f %f\n%f %f %f\n\n",mat[0][0],mat[0][1],mat[0][2],mat[1][0],mat[1][1],mat[1][2],mat[2][0],mat[2][1],mat[2][2]);
   //printf("%f\n",det(&mat));
 
    printf("%f\n",det(mat));
    int i = 0;
   //printf("%i\n",SDL_GetTicks());
    int time = SDL_GetTicks();
    while (i < 3072001){
         inverse(mat);
         multVect33(mat,v);
         i++;          
    }
    
   //printf("%f %f %f\n%f %f %f\n%f %f %f\n\n",mat[0][0],mat[0][1],mat[0][2],mat[1][0],mat[1][1],mat[1][2],mat[2][0],mat[2][1],mat[2][2]);
    printf("[%f %f %f]\n",v[0],v[1],v[2]);
   //printf("%i\n",SDL_GetTicks());
    time = SDL_GetTicks()-time;
    printf("Inverse + Multiply Time: %i\n",time);
    //float *m_rows[3] = {mat2[0],mat2[1],mat2[2]};
    //solve(m_rows,3,4);
    
    i = 0;
    time = SDL_GetTicks();
    while(i < 3072001){
        solveAppend33(mat2,v2); 
        i++; 
    }
    time = SDL_GetTicks()-time;
    printf("RREF Time: %i\n",time);
    
    //printf("[%f %f %f]\n",v3[0],v3[1],v3[2]);

    //printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",mat2[0][0],mat2[0][1],mat2[0][2], mat2[0][3],mat2[1][0],mat2[1][1],mat2[1][2], mat2[1][3],mat2[2][0],mat2[2][1],mat2[2][2], mat2[2][3]);

    float A [4][4] = {{1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4}};
    float B [4][4] = {{2,2,2,2},{1,1,1,1},{1,2,3,4},{1,2,3,4}};
    float C [4][4] = {{3,3,3,3},{1,2,3,4},{1,2,3,4},{1,2,3,4}};
    
    time = SDL_GetTicks();
    float (*AB)[4] = multiply44(A,B);
    float (*ans)[4] = multiply44(AB,C);
    free(AB);

    printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",ans[0][0], ans[0][1], ans[0][2], ans[0][3], ans[1][0], ans[1][1], ans[1][2], ans[1][3],ans[2][0], ans[2][1], ans[2][2], ans[2][3], ans[3][0], ans[3][1], ans[3][2], ans[3][3]);
    
    free(ans);
    time = SDL_GetTicks()-time;
    printf("%i\n",time);
}
