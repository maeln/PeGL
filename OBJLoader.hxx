#ifndef OBJLOADER_HXX_INCLUDED
#define OBJLOADER_HXX_INCLUDED
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Obj
{
	public:
	Obj();
	void load(std::string filename, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements);
	void arrayVec3toArrayFloat(std::vector<glm::vec3> arrayVec, float* arrayFloat);

	private:
};

#endif
