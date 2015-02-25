#include "graphics.h"
#include "matrix.h"
#include "vector.h"
#include <SDL/SDL.h>
#include <math.h>

const float LEFT_BOUNDARY = -5.0;
const float RIGHT_BOUNDARY = 5.0;
const float TOP_BOUNDARY = 5.0;
const float BOTTOM_BOUNDARY = -5.0;
const float FAR_BOUNDARY = -100.0;
const float NEAR_BOUNDARY = -5.0;
const float NUMBER_PIXELS_X = WIDTH-.5;//639.5;
const float NUMBER_PIXELS_Y = HEIGHT-.5;//479.5;

float* getOrthagMatrix(){
    float A [4][4] = {  {NUMBER_PIXELS_X/2,0,0,(NUMBER_PIXELS_X-1)/2},
                        {0,NUMBER_PIXELS_Y/2,0,(NUMBER_PIXELS_Y-1)/2},
                        {0,0,1,0},
                        {0,0,0,1}};
    float B [4][4] = {  {2/(RIGHT_BOUNDARY-LEFT_BOUNDARY),0,0,0},
                        {0,2/(TOP_BOUNDARY-BOTTOM_BOUNDARY),0,0},
                        {0,0,2/(NEAR_BOUNDARY-FAR_BOUNDARY),0},
                        {0,0,0,1}};
    float C [4][4] = {  {1,0,0,-(LEFT_BOUNDARY+RIGHT_BOUNDARY)/2},
                        {0,1,0,-(BOTTOM_BOUNDARY+TOP_BOUNDARY)/2},
                        {0,0,1,-(NEAR_BOUNDARY+FAR_BOUNDARY)/2},
                        {0,0,0,1}};
    float (*AB)[4] =  multiply44(A,B);
    float *ans = multiply44(AB,C);
    free(AB);
    return ans;
}

float* getViewMatrix(vector3D* gaze, vector3D* t, vector3D* eye){
    vector3D w,u,v;
    normalize(gaze);
    w.x = -gaze->x;
    w.y = -gaze->y;
    w.z = -gaze->z;

    u = cross(t,&w);
    normalize(&u);
    v = cross(&w,&u);
    float A [4][4] = {  {u.x,u.y,u.z,0},
                        {v.x,v.y,v.z,0},
                        {w.x,w.y,w.z,0},
                        {0,0,0,1}};
    float B [4][4] = {  {1,0,0,-eye->x},
                        {0,1,0,-eye->y},
                        {0,0,1,-eye->z},
                        {0,0,0,1}};
    return multiply44(A,B);
}
float* getPerspectiveMatrix(){
    float* A = (float*)malloc(16*sizeof(float));
    A[0]=NEAR_BOUNDARY;
    A[1]=0;
    A[2]=0;
    A[3]=0;
    A[4]=0;
    A[5]=NEAR_BOUNDARY;
    A[6]=0;
    A[7]=0;
    A[8]=0;
    A[9]=0;
    A[10]=NEAR_BOUNDARY+FAR_BOUNDARY;
    A[11]=-FAR_BOUNDARY*NEAR_BOUNDARY;
    A[12]=0;A[13]=0;A[14]=1;A[15]=0;
    return A;
}
//Needs improvments...Testing now
void drawTriangle(SDL_Surface* screen, float triangle [3][4], float* zBuffer, int zSize_x, int zSize_y, Uint8 R, Uint8 G, Uint8 B){
    int i;
    float min_y = NUMBER_PIXELS_Y;
    float max_y = 0;
    float min_x = NUMBER_PIXELS_X;
    float max_x = 0;
    for(i = 0; i < 3; i++){
        if(triangle[i][0]<min_x)
            min_x = triangle[i][0];
        if(triangle[i][0]>max_x)
            max_x = triangle[i][0];
        if(triangle[i][1]<min_y)
            min_y = triangle[i][1];
        if(triangle[i][1]>max_y)
            max_y = triangle[i][1];
    }
    //Derive the functions f01, f12, f02
    float f01_a,f01_b,f01_c,f12_a,f12_b,f12_c,f20_a,f20_b,f20_c;
    f01_a = triangle[0][1]-triangle[1][1];
    f01_b = triangle[1][0]-triangle[0][0];
    f01_c = triangle[0][0]*triangle[1][1]-triangle[1][0]*triangle[0][1];
    f12_a = triangle[1][1]-triangle[2][1];
    f12_b = triangle[2][0]-triangle[1][0];
    f12_c = triangle[1][0]*triangle[2][1]-triangle[2][0]*triangle[1][1];
    f20_a = triangle[2][1]-triangle[0][1];
    f20_b = triangle[0][0]-triangle[2][0];
    f20_c = triangle[2][0]*triangle[0][1]-triangle[0][0]*triangle[2][1];
    float f12_x0 = f12_a*triangle[0][0]+f12_b*triangle[0][1]+f12_c; 
    float f20_x1 = f20_a*triangle[1][0]+f20_b*triangle[1][1]+f20_c;
    float f01_x2 = f01_a*triangle[2][0]+f01_b*triangle[2][1]+f01_c;
    
    int coord,rx,ry,rny;
    float y,x,alpha,beta,gamma,z;
    rny = round(NUMBER_PIXELS_Y);
    for(y = min_y; y <= max_y; y++){
        for(x = min_x; x <= max_x; x++){
            alpha = (f12_a*x+f12_b*y+f12_c)/f12_x0;
            beta =  (f20_a*x+f20_b*y+f20_c)/f20_x1;
            gamma = (f01_a*x+f01_b*y+f01_c)/f01_x2;
            if(alpha > 0 && beta > 0 && gamma > 0){
                z = alpha*triangle[0][2]+beta*triangle[1][2]+gamma*triangle[2][2];
                rx = round(x);
                ry = round(y);
                ry = rny - ry;
                if(rx>=0&&rx<NUMBER_PIXELS_X&&ry>=0&&ry<NUMBER_PIXELS_Y){
                    coord = ry*zSize_x+rx;
                    if(z>=0 && z < zBuffer[coord]){
                        zBuffer[coord] = z;
                        DrawPixel(screen,R,G,B,rx,ry);
                    }
                }
            }
        }
    }
}

void rotateVectorY(float theta, int numVectors, vector3D* v){
    float A [3][3] = {  {cos(theta),0,sin(theta)},
                        {0,1,0},
                        {-sin(theta),0,cos(theta)}};
    int i  = 0;
    for( i = 0; i < numVectors; i++){
        float temp [3] = {v->x,v->y,v->z};
        multVect33(A,temp);
        v->x = temp[0];
        v->y = temp[1];
        v->z = temp[2];
        v++;
    }
}
void rotateVectorRodrigues(float theta, int numVectors, vector3D* vectors, vector3D* axis){
    float mcos = 1-cos(theta);
    float axis_sin = sin(theta);
    float x = axis->x;
    float y = axis->y;
    float z = axis->z;
    float A [3][3] = {  
          {(1-mcos)+x*x*mcos,x*y*mcos-z*axis_sin,y*axis_sin+x*z*mcos},
          {z*axis_sin+x*y*mcos,(1-mcos)+y*y*mcos,-x*axis_sin+y*z*mcos},
          {-y*axis_sin+x*z*mcos,x*axis_sin+y*z*mcos,(1-mcos)+z*z*mcos}};
    int i;
    for( i = 0; i < numVectors; i++){
        float temp [3] = {vectors->x,vectors->y,vectors->z};
        multVect33(A,temp);
        vectors->x = temp[0];
        vectors->y = temp[1];
        vectors->z = temp[2];
        vectors++;
    }
}

void DrawLine(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, float x0, float y0, float x1, float y1){
    y0 = NUMBER_PIXELS_Y - y0;
    y1 = NUMBER_PIXELS_Y - y1;
    char ret = 1;
    if(!(x0>=0&&x1>=0&&x0<NUMBER_PIXELS_X&&x1<NUMBER_PIXELS_X&&y0>=0&&y1>=0&&y0<NUMBER_PIXELS_Y&&y1<NUMBER_PIXELS_Y)){
        ret = clip(&x0,&y0,&x1,&y1);
        //printf("clipped\n");
    }
    if(!ret)
        return; //both points are not in the screen
    //printf("%f %f\n",y0,y1);
    if(round(x0) == round(x1)){ //Vertical Line Case:
        float y;
        if(y0<y1){
            for(y = y0; y <= y1 ; y++){
                DrawPixel(screen,R,G,B,round(x0),round(y));
            }
        }
        else{
            for(y = y0; y > y1 ; y--){
                DrawPixel(screen,R,G,B,round(x0),round(y));
            }
        }
    }
    else{
        float slope = (y1-y0)/(x1-x0);
        if(fabs(slope) <= 1){ //increment x
            int inc = x0<x1 ? 1 : -1;
            float x;
            for(x = x0; fabs(x1-x) > .5; x += inc){
                float t = (x-x0)/(x1-x0);
                float y = y0 + t*(y1-y0);
                DrawPixel(screen,R,G,B,round(x),round(y));
            }
        }
        else{ //increment y
            int inc = y0<y1 ? 1 : -1;
            float y;
            for(y = y0; fabs(y1-y) > .5; y += inc){
                float t = (y-y0)/(y1-y0);
                float x = x0 + t*(x1-x0);
                DrawPixel(screen,R,G,B,round(x),round(y));
            }
        }
    }
}
char clip(float* x0, float* y0, float* x1, float*y1){
    char flag1,flag2;
    flag1 = (*x0>=0&&*x0<NUMBER_PIXELS_X&&*y0>=0&&*y0<NUMBER_PIXELS_Y);
    flag2 = (*x1>=0&&*x1<NUMBER_PIXELS_X&&*y1>=0&&*y1<NUMBER_PIXELS_Y);
    if(!flag1 && !flag2)
        return 0;
    if(!flag1 && flag2){
        float temp = *x0;
        *x0 = *x1;
        *x1 = temp;
        temp = *y0;
        *y0 = *y1;
        *y1 = temp;
    }
    //Now x0,y0 is valid and x1,y1 is invalid.
    *y0 = NUMBER_PIXELS_Y - *y0;
    *y1 = NUMBER_PIXELS_Y - *y1; //for mathematical convenience

    //clipping code yay!
    if(round(*x0)==round(*x1)){ //vertical line and y1 is out of bounds
        if(*y1 >= NUMBER_PIXELS_Y)
            *y1 = NUMBER_PIXELS_Y-.5;
        else
            *y1 = 0;
    }
    else{
        float m = (*y1-*y0)/(*x1-*x0);
        if(fabs(m) >= 1){ //step y
            int inc = *y0<*y1 ? 1 : -1;
            float x,y;
            x = 0;
            for(y = *y0; fabs(*y1-y) > .5  && x>=0 && x < NUMBER_PIXELS_X && y >= 0 && y < NUMBER_PIXELS_Y; y += inc){
                float t = (y-*y0)/(*y1-*y0);
                x = *x0 + t*(*x1-*x0);
            }
            *x1 = x;
            *y1 = y;
        }
        else{ //step x
            int inc = *x0<*x1 ? 1 : -1;
            float x,y;
            y = 0;
            for(x = *x0; fabs(*x1-x) > .5 && x>=0 && x < NUMBER_PIXELS_X && y >= 0 && y < NUMBER_PIXELS_Y; x += inc){
                float t = (x-*x0)/(*x1-*x0);
                y = *y0 + t*(*y1-*y0);
            }
            *x1 = x;
            *y1 = y;
        }
    }
    *y0 = NUMBER_PIXELS_Y - *y0;
    *y1 = NUMBER_PIXELS_Y - *y1; //returns the values to screen coordinates
    if(*y1>=NUMBER_PIXELS_Y)
        *y1 = NUMBER_PIXELS_Y-.5;
    return 1;
}

void DrawPixel(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, int x, int y)
{
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);
    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
    switch (screen->format->BytesPerPixel) {
        case 1: { /* Assuming 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* Probably 15-bpp or 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* Slow 24-bpp mode, usually not used */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *(bufp+screen->format->Rshift/8) = R;
            *(bufp+screen->format->Gshift/8) = G;
            *(bufp+screen->format->Bshift/8) = B;
        }
        break;

        case 4: { /* Probably 32-bpp */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
    //SDL_UpdateRect(screen, x, y, 1, 1);
}
