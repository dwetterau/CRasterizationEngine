#ifndef H_VECTOR
#define H_VECTOR
typedef struct {
    float x;
    float y;
    float z;
} vector3D;
float magnitude(vector3D* v);
void normalize(vector3D* v);
vector3D cross(vector3D* v1, vector3D* v2);
float dot(vector3D* v1, vector3D* v2);
#endif
