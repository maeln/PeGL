//      Matrix.cxx
//      
//      Copyright 2011 Mael N. <contact@maeln.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <iostream>
#include <vector>
#include <math.h>
#include "Matrix.hxx"

using namespace std;

Matrix::Matrix() {}

void Matrix::init(mat4 *matrix)
{
	int i;
	if(matrix != NULL)
	{
		for(i = 0; i<16; i++)
		{
			matrix->val[i] = 0.0;
			if(i == 0 or i == 5 or i == 10 or i == 15)
			{
				matrix->val[i] = 1.0;
			}
		}
	} 
	else
	{
		cerr << "Erreur: Pointeur vers matrice NULL." << endl;
	}
}

void Matrix::translate(mat4 *matrix, float x, float y, float z)
{
	mat4 transMatrix;
	Matrix::init(&transMatrix);
	
	transMatrix.val[3] = x;
	transMatrix.val[7] = y;
	transMatrix.val[11] = z;
	
	Matrix::multiplyMatrix(matrix, &transMatrix);
}

void Matrix::scale(mat4 *matrix, float x, float y, float z)
{
	mat4 scaleMatrix;
	Matrix::init(&scaleMatrix);
	
	scaleMatrix.val[0] = x;
	scaleMatrix.val[5] = y;
	scaleMatrix.val[10] = z;
	
	Matrix::multiplyMatrix(matrix, &scaleMatrix);
}

void Matrix::rotate(mat4 *matrix, float angle, float x, float y, float z)
{
	mat4 rotMatrix;
	Matrix::init(&rotMatrix);
	
	angle = angle * M_PI / 180.0;
	
	vec3 axis = {x, y, z};
	Matrix::normalizeVector(&axis);
	
	rotMatrix.val[0] = axis.x*axis.x * (1 - cos(angle)) + cos(angle);
	rotMatrix.val[1] = axis.x*axis.y * (1 - cos(angle)) - axis.z*sin(angle);
	rotMatrix.val[2] = axis.x*axis.z * (1 - cos(angle)) + axis.y*sin(angle);
	
	rotMatrix.val[4] = axis.x*axis.y * (1 - cos(angle)) + axis.z*sin(angle);
	rotMatrix.val[5] = axis.y*axis.y * (1 - cos(angle)) + cos(angle);
	rotMatrix.val[6] = axis.y*axis.z * (1 - cos(angle)) - axis.x*sin(angle);
	
	rotMatrix.val[8] = axis.x*axis.z * (1 - cos(angle)) - axis.y*sin(angle);
	rotMatrix.val[9] = axis.y*axis.z * (1 - cos(angle)) + axis.x*sin(angle);
	rotMatrix.val[10] = axis.z*axis.z * (1 - cos(angle)) + cos(angle);
	
	rotMatrix.val[15] = 1.0;
	
	Matrix::multiplyMatrix(matrix, &rotMatrix);
}

void Matrix::Perspective(mat4 *matrix, float angle, float ratio, float near, float far)
{
	mat4 project;
	int a;
	float f;
	
	for(a = 0; a < 16; a++)
	{
		project.val[a] = 0;
	}
	
	angle = angle * (M_PI*2.f / 360.f);
	f = 1.f / (tan(angle/2.f));
	
	project.val[0] = f / ratio;
	project.val[5] = f;
	project.val[10] = (near + far) / (near - far);
	project.val[11] = (2*near*far) / (near - far);
	project.val[14] = -1;
	
	Matrix::multiplyMatrix(matrix, &project);
}

void Matrix::multiplyMatrix(mat4 *matrix1, mat4 *matrix2)
{
	mat4 result;
	int a;
	int b;
	int c;
	
	for(a = 0; a < 16; a++)
	{
		result.val[a] = 0.0;
	}
	
	for(c = 0; c < 4; c++)
	{
		for(a = 0; a < 4; a++)
		{
			for(b = 0; b < 4; b++)
			{
				result.val[4*a+c] += matrix1->val[4*a+b] * matrix2->val[4*b+c];
			}
		}
	}
	
	for(a = 0; a < 16; a++)
	{
		matrix1->val[a] = result.val[a];
	}
}

void Matrix::normalizeVector(vec3 *vecteur)
{
	float norme(sqrt(vecteur->x*vecteur->x+vecteur->y*vecteur->y+vecteur->z*vecteur->z));
	
	if(norme != 0.0)
	{
		vecteur->x /= norme;
		vecteur->y /= norme;
		vecteur->z /= norme;
	}
}

vec3 Matrix::cross(vec3 vector1, vec3 vector2)
{
	vec3 result = { 0, 0, 0 };
	result.x = vector1.y * vector2.z - vector1.z * vector2.y;
	result.y = vector1.z * vector2.x - vector1.x * vector2.z;
	result.z = vector1.x * vector2.y - vector1.y * vector2.x;
	
	return result;
}

void Matrix::lookAt(mat4 *matrix, vec3 camera, vec3 lookPt, vec3 vertical)
{
	vec3 vision = { lookPt.x - camera.x, lookPt.y - camera.y, lookPt.z - camera.z };
	
	Matrix::normalizeVector(&vision);
	Matrix::normalizeVector(&vertical);
	
	vec3 normal = Matrix::cross(vision, vertical);
	vec3 axe = Matrix::cross(normal, vision);
	
	Matrix::normalizeVector(&normal);
	Matrix::normalizeVector(&axe);
	
	mat4 worldMatrix;
	worldMatrix.val[0] = normal.x;
	worldMatrix.val[1] = normal.y;
	worldMatrix.val[2] = normal.z;
	
	worldMatrix.val[4] = axe.x;
	worldMatrix.val[5] = axe.y;
	worldMatrix.val[6] = axe.z;
	
	worldMatrix.val[8] = -vision.x;
	worldMatrix.val[9] = -vision.y;
	worldMatrix.val[10] = -vision.z;
	
	worldMatrix.val[15] = 1.0;
	
	Matrix::multiplyMatrix(matrix, &worldMatrix);
	Matrix::translate(matrix, -camera.x, -camera.y, -camera.z);
	
}

vec3 Matrix::calcNormal(vec4 vert1, vec4 vert2, vec4 vert3)
{
	vec3 normale;
	normale.x = (vert2.y-vert1.y)*(vert3.z-vert1.z) - (vert2.z-vert1.z)*(vert3.y-vert1.y);
	normale.y = (vert2.z-vert1.z)*(vert3.x-vert1.x) - (vert2.x-vert1.x)*(vert3.z-vert1.z);
	normale.z = (vert2.x-vert1.x)*(vert3.y-vert1.y) - (vert2.y-vert1.y)*(vert3.x-vert1.x);
	Matrix::normalizeVector(&normale);
	
	return normale;
}

void Matrix::makeNormal(float *obj, int size, int dimension, vec3 *normArray)
{
	int i(0);
	int n(0);
	int pos(0);
	float arr[12];
	vec4 vert1;
	vec4 vert2;
	vec4 vert3;
	while(n<size)
	{
		for(i=0; i<11; i++)
		{
			arr[i] = obj[n+i];
		}
		vert1.x = arr[0];
		vert1.y = arr[1];
		vert1.z = arr[2];
		vert1.w = arr[3];
		
		vert2.x = arr[4];
		vert2.y = arr[5];
		vert2.z = arr[6];
		vert2.w = arr[7];
		
		vert3.x = arr[8];
		vert3.y = arr[9];
		vert3.z = arr[10];
		vert3.w = arr[11];
		
		normArray[pos] = Matrix::calcNormal(vert1, vert2, vert3);
		normArray[pos+1] = Matrix::calcNormal(vert1, vert2, vert3);
		normArray[pos+2] = Matrix::calcNormal(vert1, vert2, vert3);
		
		pos += 3;
		n += 12;
	}
}

vec3 Matrix::minus(vec3 in1, vec3 in2)
{
	vec3 res;
	res.x = in1.x - in2.x;
	res.y = in1.y - in2.y;
	res.z = in1.z - in2.z;
	
	return res;
}

void Matrix::arrayVec3toArrayFloat(vector<vec3> arrayVec, float* arrayFloat)
{
	for(unsigned int i = 0; i < arrayVec.size(); i++)
	{
		arrayFloat[i*3] = arrayVec[i].x;
		arrayFloat[i*3+1] = arrayVec[i].y;
		arrayFloat[i*3+2] = arrayVec[i].z;
	}
}

mat3 Matrix::normalTransposition(mat4 modelview)
{
	mat3 result;
	result.val[0] = modelview.val[0];
	result.val[3] = modelview.val[1];
	result.val[6] = modelview.val[2];
	result.val[1] = modelview.val[4];
	result.val[4] = modelview.val[5];
	result.val[7] = modelview.val[6];
	result.val[2] = modelview.val[8];
	result.val[5] = modelview.val[9];
	result.val[8] = modelview.val[10];
	
	return result;
}
		