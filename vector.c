#include <math.h>
#include "vector.h"

float magnitude(vector3D* v){
    return sqrt((v->x*v->x)+(v->y*v->y)+(v->z*v->z));
}
void normalize(vector3D* v){
    float mag = magnitude(v);
    v->x = v->x/mag;
    v->y = v->y/mag;
    v->z = v->z/mag;
}
vector3D cross(vector3D* v1, vector3D* v2){
    vector3D vect;
    vect.x = v1->y*v2->z-v1->z*v2->y;
    vect.y = v1->z*v2->x-v1->x*v2->z;
    vect.z = v1->x*v2->y-v1->y*v2->x;
    return vect;
}
float dot(vector3D* v1, vector3D* v2){
    return v1->x*v2->x+v1->y*v2->y+v1->z*v2->z;
}
