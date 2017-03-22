// postprocessing.cxx
// 
// Copyright 2012 Mael N. <contact@maeln.com>
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.


#include "postprocessing.hxx"

namespace PeGL
{

PostProcessing::PostProcessing(GLsizei width, GLsizei height, std::string vertexs_path, std::string frags_path)
{
	// FBO Texture.
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Depth Buffer.
	/*
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	*/
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &depth_buffer);
	glBindTexture(GL_TEXTURE_2D, depth_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// FBO.
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_buffer, 0);
		GLenum status(glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "[ERR] FB : " << status << std::endl;
			throw std::runtime_error("framebuffer error");
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	GLfloat vertices[] = {
		-1,	 -1,
		 1,  -1,
		-1,   1,
		 1,   1
	};
	
	glGenBuffers(1, &fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, fbo_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Post Processing Shaders :
	ShaderLoader shdl;
	PeShader vertexs = shdl.loadShader(vertexs_path, GL_VERTEX_SHADER);
	PeShader frags = shdl.loadShader(frags_path, GL_FRAGMENT_SHADER);
	std::vector<PeShader> shd;
	shd.push_back(vertexs);
	shd.push_back(frags);
	
	postproc = shdl.createProgram(shd);
}


PostProcessing::~PostProcessing()
{
	glDeleteRenderbuffers(1, &depth_buffer);
	glDeleteTextures(1, &fbo_texture);
	glDeleteFramebuffers(1, &fbo);
	glDeleteBuffers(1, &fbo_vertices);
	glDeleteProgram(postproc.addr);
}

void PostProcessing::resize_fbo(GLsizei nwidth, GLsizei nheight)
{
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nwidth, nheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	/*
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, nwidth, nheight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	*/
}

void PostProcessing::bindfb()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void PostProcessing::unbindfb()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::drawfb()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(postproc.addr);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(postproc.uniform["fbo_tex"], 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depth_buffer);
	glUniform1i(postproc.uniform["depth"], 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, fbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
	
}
