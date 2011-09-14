#ifndef SHADER_HXX_INCLUDED
#define SHADER_HXX_INCLUDED
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

class Shader
{
	public:
	Shader();
	char* LoadShader(std::string filename);
	GLuint CompileShader(GLenum shaderType, std::string shaderPath);
	GLuint MakeProgram(GLuint vertexshader, GLuint pixelshader);
	void Remove_s(GLuint shader);
	void Remove_p(GLuint program);
	
	private:
};

#endif
