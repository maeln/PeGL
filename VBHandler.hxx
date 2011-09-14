#ifndef VBHANDLER_HXX_INCLUDED
#define VBHANDLER_HXX_INCLUDED
#include <GL/gl.h>
#include <GL/glu.h>

typedef struct
{
	GLuint Vbo;
	GLuint Vao;
} Vbh;

class VBHandler
{
	public:
	VBHandler();
	GLuint makeVBO(float data[], size_t dataSize, GLenum drawMode);
	GLuint makeVAO(GLuint Vbo, int nbArray, int nbDimension[], void* Offset[]);
	Vbh makeVBH(float data[], size_t dataSize, GLenum drawMode, int nbArray, int nbDimension[], void* Offset[]);
	
	private:
};

#endif
