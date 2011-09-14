//      VBHandler.cxx
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

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "VBHandler.hxx"

VBHandler::VBHandler() {}

GLuint VBHandler::makeVBO(float data[], size_t dataSize, GLenum drawMode)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, drawMode);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	return vbo;
}

GLuint VBHandler::makeVAO(GLuint Vbo, int nbArray, int nbDimension[], void* Offset[])
{
	GLuint vao;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	
	int i;
	for(i = 0; i < nbArray; i++)
	{
		glEnableVertexAttribArray(i);
	}
	for(i = 0; i < nbArray; i++)
	{
		glVertexAttribPointer(i, nbDimension[i], GL_FLOAT, GL_FALSE, 0, Offset[i]);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	return vao;
}

Vbh VBHandler::makeVBH(float data[], size_t dataSize, GLenum drawMode, int nbArray, int nbDimension[], void* Offset[])
{
	GLuint mVbo = VBHandler::makeVBO(data, dataSize, drawMode);
	GLuint mVao = VBHandler::makeVAO(mVbo, nbArray, nbDimension, Offset);
	
	Vbh mVbh;
	mVbh.Vbo = mVbo;
	mVbh.Vao = mVao;
	
	return mVbh;
}
