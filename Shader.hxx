#ifndef SHADER_HXX_INCLUDED
#define SHADER_HXX_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <vector>
#include <string>
#include <stdlib.h>
#include <map>

class Shader
{
	public:
	Shader();
	const char* LoadShader(std::string filename, std::vector<std::string> &uniform);
	GLuint CompileShader(GLenum shaderType, std::string shaderPath, std::vector<std::string> &unifrom);
	GLuint MakeProgram(GLuint vertexshader, GLuint pixelshader);
	void Remove_s(GLuint shader);
	void Remove_p(GLuint program);
	void InitUniformLocation(GLuint program, std::vector<std::string> &uniform_name, std::map<std::string,GLuint> &uniform);
	
	private:
};

#endif
