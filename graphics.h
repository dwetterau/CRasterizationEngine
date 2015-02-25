#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <SDL/SDL.h>
#include "vector.h"
#define HEIGHT (600)
#define WIDTH (600)

float* getOrthagMatrix();
float* getPersepectiveMatrix();
float* getViewMatrix(vector3D* gaze, vector3D* t, vector3D* eye);
void DrawLine(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, float x0, float y0, float x1 ,float y1);
void drawTriangle(SDL_Surface* screen, float triangle [3][4], float* zBuffer, int zSize_x, int zSize_y, Uint8 R, Uint8 G, Uint8 B);
void DrawPixel(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, int x, int y);
char clip(float* x0,float* y0,float* x1,float* y1);
void rotateVectorY(float theta, int numVectors, vector3D* v);
void rotateVectorRodrigues(float theta, int numVectors, vector3D* vectors, vector3D* axis);
#endif
