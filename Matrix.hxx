#ifndef MATRIX_HXX_INCLUDED
#define MATRIX_HXX_INCLUDED
#include <vector>

typedef struct 
{
	float val[16];
} mat4;

typedef struct
{
	float x;
	float y;
	float z;
} vec3;

typedef struct
{
	float x;
	float y;
	float z;
	float w;
} vec4;

class Matrix
{
	public:
	Matrix();
	void init(mat4 *matrix);
	void translate(mat4 *matrix, float x, float y, float z);
	void scale(mat4 *matrix, float x, float y, float z);
	void rotate(mat4 *matrix, float angle, float x, float y, float z);
	void Perspective(mat4 *matrix, float angle, float ratio, float near, float far);
	void multiplyMatrix(mat4 *matrix1, mat4 *matrix2);
	void normalizeVector(vec3 *vecteur);
	vec3 cross(vec3 vector1, vec3 vector2);
	void lookAt(mat4 *matrix, vec3 camera, vec3 lookPt, vec3 vertical);
	vec3 calcNormal(vec4 vert1, vec4 vert2, vec4 vert3);
	void makeNormal(float *obj, int size, int dimension, vec3 *normArray);
	vec3 minus(vec3, vec3);
	void arrayVec3toArrayFloat(std::vector<vec3>, float*);
	
	private:
};

#endif
