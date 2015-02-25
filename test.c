#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include <math.h>
#include "graphics.h"
#include "vector.h"



int main(){
/*/FILE INPUT============================================
FILE *fp;
fp = fopen("box","r");
if(fp == NULL){
    printf("Error reading input file\n");
    exit(1);
}
int numTriangles = 0;
    if(fscanf(fp,"%i",&numTriangles) != EOF){
        printf("Reading in %i triangle(s).\n",numTriangles);
    }
    else{ 
        printf("Error reading input file\n");
        exit(1);
    }
float mutable_triangles [numTriangles][3][4];
float triangles [numTriangles][3][4];
float triangle_colors[numTriangles][3];
float tempTriangle [9];
int index = 0; int red = 0; int green = 0; int blue = 0;
int i = 0;
while(fscanf(fp, "%f %f %f %f %f %f %f %f %f %i %i %i",&tempTriangle[0],
    &tempTriangle[1],&tempTriangle[2],&tempTriangle[3],&tempTriangle[4],
    &tempTriangle[5],&tempTriangle[6],&tempTriangle[7],&tempTriangle[8],
    &red,&blue,&green) != EOF){
    triangle_colors[index][0] = red;
    triangle_colors[index][1] = green;
    triangle_colors[index][2] = blue;

    for(i = 0; i < 9; i++){
        triangles[index][i/3][i%3] = tempTriangle[i];
        if(i%3 == 2)
            triangles[index][i/3][3] = 1;
        }    
    index++;
}
fclose(fp);

*/
/*
int numTriangles = 1;
float mutable_triangles [1][3][4];
float triangles [1][3][4] = {{
        {-2, -2, -14, 1},
        {2, -2, -14, 1},
        {0, 2, -14, 1}}};
float triangle_colors[1][3] = {
        {255, 0, 0}};
*/


//WINDOW FRAME SETUP=====================================
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(WIDTH,HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if ( screen == NULL ) {
        fprintf(stderr, "Unable to set WIDTHxHEIGHT video: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_Surface *background;
    int quit = 0;
    SDL_Event event; 
    float zBuffer [WIDTH*HEIGHT];
    int x,y;
//POINT SETUP====================================================================================================
 
    float (*Mv)[4], (*M)[4];
    float (*Mo)[4] = getOrthagMatrix();
    float (*Mp)[4] = getPerspectiveMatrix();
    Mo = multiply44(Mo,Mp);
    //printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",Mo[0][0], Mo[0][1], Mo[0][2], Mo[0][3], Mo[1][0], Mo[1][1], Mo[1][2], Mo[1][3],Mo[2][0], Mo[2][1], Mo[2][2], Mo[2][3], Mo[3][0], Mo[3][1], Mo[3][2], Mo[3][3]);
    srandom(SDL_GetTicks());
    float vertices [8][4]= {{-2,-2,-10,1},
                            {2,-2,-10,1},
                            {2,2,-10,1},
                            {-2,2,-10,1},
                            {-2,-2,-14,1},
                            {2,-2,-14,1},
                            {2,2,-14,1},
                            {-2,2,-14,1}};
    float mutable_vertices [8][4];
    
    int numTriangles = 7;
    float triangles [7][3][4] = {
                                {{-2,-2,-14,1},{2,-2,-14,1},{0,2,-14,1}},  //This is a single triangle
                                  {{-2,-2,-16,1},{2,-2,-16,1},{0,2,-16,1}}, 
                                {{-2,-2,-18,1},{2,-2,-18,1},{0,2,-18,1}},  //This is a single triangle
                                {{-2,-2,-20,1},{2,-2,-20,1},{0,2,-20,1}},  //This is a single triangle
                                {{-2,-2,-22,1},{2,-2,-22,1},{0,2,-22,1}},  //This is a single triangle
                                {{-2,-2,-24,1},{2,-2,-24,1},{0,2,-24,1}},  //This is a single triangle
                                {{-2,-2,-26,1},{2,-2,-26,1},{0,2,-26,1}}  //This is a single triangle
                                };
    float mutable_triangles [7][3][4];
    float triangle_colors[7][3] = {
        {255, 0, 0},
        {180, 36, 0},
        {144, 72, 0},
        {108, 108, 0},
        {72, 144, 0},
        {36, 180, 0},
        {0, 255, 0}
    };
   
    
    
    int row,col,triangle;
//CAMERA VECTOR SETUP============================================================================================
    float latChange, longChange;
    latChange = 0;
    longChange = 0;
    vector3D gaze,eye,t;
    gaze.x = -1;
    gaze.y = -1;
    gaze.z = -1;
    t.x = -1;
    t.y = 1;
    t.z = -1;
    eye.x = 12;
    eye.y = 12;
    eye.z = 0;
    normalize(&gaze);
    normalize(&t);
    vector3D view_vectors [2] = {gaze,t};
//MAIN ANIMATION LOOP============================================================================================
    float c = 0;
    int start = SDL_GetTicks();
    while(!quit){
        SDL_Flip(screen);
        SDL_FillRect(screen,NULL,0);       
        rotateVectorY(longChange,2,view_vectors);
        vector3D side = cross(&view_vectors[1],&view_vectors[0]);
        rotateVectorRodrigues(latChange,2,view_vectors,&side);
        latChange = 0;
        longChange = 0;
        Mv  = getViewMatrix(&view_vectors[0],&view_vectors[1],&eye);
        M = multiply44(Mo,Mv);

        //Resetting the zBuffer   =(
        for(x = 0; x < WIDTH; x++)
            for(y = 0; y < HEIGHT; y++){
                zBuffer[y*WIDTH+x] = 2;
        }

//Triangular polygon testing
        for(triangle = 0; triangle < numTriangles; triangle++){
            for(col = 0; col < 3; col++){
                for(row = 0; row < 4; row++)
                    mutable_triangles[triangle][col][row] = 
                    triangles[triangle][col][row];                 
                multVect44(M,mutable_triangles[triangle][col]);
                mutable_triangles[triangle][col][0] /= 
                    mutable_triangles[triangle][col][3];
                mutable_triangles[triangle][col][1] /= 
                    mutable_triangles[triangle][col][3];
                mutable_triangles[triangle][col][2] /= 
                    mutable_triangles[triangle][col][3];
                mutable_triangles[triangle][col][2] += 1;
                
                mutable_triangles[triangle][col][2] = 
                    2-mutable_triangles[triangle][col][2];
            }
            drawTriangle(screen,mutable_triangles[triangle],zBuffer,
            WIDTH,HEIGHT,triangle_colors[triangle][0],triangle_colors[triangle][1],
            triangle_colors[triangle][2]);
        }



        // WireFrame Cube
        for(row= 0; row < 8; row++){
            for(col = 0; col<4; col++){
                mutable_vertices[row][col] = vertices[row][col];
            }
        }
        int i;
        for(i = 0; i < 8; i++){
            multVect44(M,mutable_vertices[i]);
            mutable_vertices[i][0] /= mutable_vertices[i][3]; 
            mutable_vertices[i][1] /= mutable_vertices[i][3]; 
            mutable_vertices[i][2] /= mutable_vertices[i][3]; 
        //printf("[%f %f %f]\n",mutable_vertices[i][0],mutable_vertices[i][1],mutable_vertices[i][2]);
        }

       //Clock Line Drawing Test
        //DrawLine(screen,255,0,0,320,240,320+round(200*cos(c)),HEIGHT-(240+round(200*sin(c))));
        

        //Drawing of the Cube edges
        DrawLine(screen,255,0,0,mutable_vertices[0][0],mutable_vertices[0][1],mutable_vertices[1][0],mutable_vertices[1][1]);
        DrawLine(screen,255,0,0,mutable_vertices[2][0],mutable_vertices[2][1],mutable_vertices[1][0],mutable_vertices[1][1]);
        DrawLine(screen,255,0,0,mutable_vertices[2][0],mutable_vertices[2][1],mutable_vertices[3][0],mutable_vertices[3][1]);
        DrawLine(screen,255,0,0,mutable_vertices[2][0],mutable_vertices[2][1],mutable_vertices[6][0],mutable_vertices[6][1]);
        DrawLine(screen,255,0,0,mutable_vertices[1][0],mutable_vertices[1][1],mutable_vertices[5][0],mutable_vertices[5][1]);
        DrawLine(screen,255,0,0,mutable_vertices[0][0],mutable_vertices[0][1],mutable_vertices[4][0],mutable_vertices[4][1]);
        DrawLine(screen,255,0,0,mutable_vertices[3][0],mutable_vertices[3][1],mutable_vertices[7][0],mutable_vertices[7][1]);
        DrawLine(screen,255,0,0,mutable_vertices[7][0],mutable_vertices[7][1],mutable_vertices[6][0],mutable_vertices[6][1]);
        DrawLine(screen,255,0,0,mutable_vertices[6][0],mutable_vertices[6][1],mutable_vertices[5][0],mutable_vertices[5][1]);
        DrawLine(screen,255,0,0,mutable_vertices[5][0],mutable_vertices[5][1],mutable_vertices[4][0],mutable_vertices[4][1]);
        DrawLine(screen,255,0,0,mutable_vertices[4][0],mutable_vertices[4][1],mutable_vertices[7][0],mutable_vertices[7][1]);
        DrawLine(screen,255,0,0,mutable_vertices[0][0],mutable_vertices[0][1],mutable_vertices[3][0],mutable_vertices[3][1]);
        
        
        //Event Listener
        SDL_PollEvent(&event);
            switch(event.type){
                case SDL_QUIT:
                    quit = 1;
                    SDL_Quit();
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_LEFT:
                            longChange = .01;
                            break;
                        case SDLK_RIGHT:
                            longChange = -.01;
                            break;
                        case SDLK_UP:
                            latChange = -.01;
                            break;
                        case SDLK_DOWN:
                            latChange = .01;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        SDL_Delay(10);
        c+=.01;
        //printf("FPS: %f\n",(c*100)/((SDL_GetTicks()-start)/1000));
    }
return 0;
}
