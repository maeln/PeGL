#ifndef OBJLOADER_HXX_INCLUDED
#define OBJLOADER_HXX_INCLUDED
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Matrix.hxx"

class Obj
{
	public:
	Obj();
	//void load(std::string filename);
	void load(std::string filename, std::vector<vec3> &vertices, std::vector<vec3> &normals, std::vector<GLushort> &elements);
	std::string dealWithSlash(std::string);
	std::vector<std::string> splitSpace(std::string);
	float* vectorToFloatArray(std::vector<float>& array);
	
	private:
};

#endif
