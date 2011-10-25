//      OpenGL3.cxx
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
#include <fstream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <cmath>
#include "Shader.hxx"
#include "Matrix.hxx"
#include "VBHandler.hxx"
#include "OBJLoader.hxx"

using namespace std;

int main(int argc, char **argv)
{
	//Fenêtre.
	sf::Window window(sf::VideoMode(800, 600), "Sunflower Engine");
	window.UseVerticalSync(true);
	
	glViewport(0, 0, 800, 600);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.f, 1.f);
	glEnable(GL_DEPTH_CLAMP);
	
	//Matrices.
	Matrix matrix;
	mat4 projection, modelview, world;
	matrix.init(&projection);
	matrix.init(&modelview);
	matrix.init(&world);
	vec3 camera = { 1, 1, 1 };
	vec3 lookPt = { 0, 0, 0 };
	vec3 vertical = { 0, 1, 0, };
	matrix.lookAt(&world, camera, lookPt, vertical);
	matrix.Perspective(&projection, 45.f, 4.f/3.f, 0.1, 100.f);
	
	//Glew.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "Erreur: " << glewGetErrorString(err) << endl;
	}
	cout << "Info: Glew " << glewGetString(GLEW_VERSION) << " en cours d'utilisation." << endl;
	if (!glewIsSupported("GL_VERSION_3_3"))
	{
		cerr << "Erreur: OpenGL 3.3 n'est pas disponible sur votre système.";
		exit(EXIT_FAILURE);
	}
	
	//Shader.
	Shader shader;
	
	GLuint VertexShad(shader.CompileShader(GL_VERTEX_SHADER, "Shader/Light.vert"));
	GLuint PixelShad(shader.CompileShader(GL_FRAGMENT_SHADER, "Shader/Light.frag"));
	GLuint program(shader.MakeProgram(VertexShad, PixelShad));
	GLuint axesVert(shader.CompileShader(GL_VERTEX_SHADER, "Shader/Axes.vert"));
	GLuint axesFrag(shader.CompileShader(GL_FRAGMENT_SHADER, "Shader/Axes.frag"));
	GLuint axesProg(shader.MakeProgram(axesVert, axesFrag));
	shader.Remove_s(axesVert);
	shader.Remove_s(axesFrag);
	shader.Remove_s(VertexShad);
	shader.Remove_s(PixelShad);
	
	GLuint light_dirLoc = glGetUniformLocation(program, "light_dir");
	GLuint vertLoopLoc = glGetUniformLocation(program, "vertLoop");
	GLuint fragLoopLoc = glGetUniformLocation(program, "fragLoop");
	GLuint addr(0);
	GLuint MatLoc = glGetUniformBlockIndex(program, "GlobalMatrices");
	glUniformBlockBinding(program, MatLoc, addr);
	GLuint MatLoc2 = glGetUniformBlockIndex(axesProg, "GlobalMatrices");
	glUniformBlockBinding(axesProg, MatLoc2, addr);
	
	//UBO
	GLuint GlobalMatricesUBO;
	glGenBuffers(1, &GlobalMatricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, GlobalMatricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4)*2, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, addr, GlobalMatricesUBO, 0, sizeof(mat4) * 2);
	
	//Vertex.
	float axes[] = {
		0.0,	0.0,	0.0,	1.0,
		1.0,	0.0,	0.0,	1.0,
		
		0.0,	0.0,	0.0,	1.0,
		0.0,	1.0,	0.0,	1.0,
		
		0.0,	0.0,	0.0,	1.0,
		0.0,	0.0,	1.0,	1.0,
		//color
		1.0,	0.0,	0.0,	1.0,
		1.0,	0.0,	0.0,	1.0,
		
		0.0,	1.0,	0.0,	1.0,
		0.0,	1.0,	0.0,	1.0,
		
		0.0,	0.0,	1.0,	1.0,
		0.0,	0.0,	1.0,	1.0
	};
	
	//Load suzanne.
	Obj obj;
	vector<vec3> s_vertices;
	vector<vec3> s_normals;
	vector<GLushort> s_indices;
	obj.load("suzanne.obj", s_vertices, s_normals, s_indices);
	float sf_vertices[s_vertices.size()*3];
	float sf_normals[s_vertices.size()*3];
	matrix.arrayVec3toArrayFloat(s_vertices, sf_vertices);
	matrix.arrayVec3toArrayFloat(s_normals, sf_normals);
	
	
	GLuint s_vertices_vbo;
	glGenBuffers(1, &s_vertices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertices_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sf_vertices), sf_vertices, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint s_normals_vbo;
	glGenBuffers(1, &s_normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sf_normals), sf_normals, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint s_indices_vbo;
	glGenBuffers(1, &s_indices_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indices_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(s_indices[0])*s_indices.size()), s_indices.data(), GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	GLuint i_vao;
	glGenVertexArrays(1, &i_vao);
	glBindVertexArray(i_vao);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertices_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//VB
	VBHandler vbHandler;
	GLuint vboAxes = vbHandler.makeVBO(axes, sizeof(axes), GL_STREAM_DRAW);
	
	int nbDimension[] = { 4, 4, 4 };
	void* Offset2[] = { 0, (void*)(sizeof(float)*4*6) };
	GLuint vaoAxes = vbHandler.makeVAO(vboAxes, 2, nbDimension, Offset2);
	
	//Time.
	sf::Clock tclock;
	float actt(0.0);
	float difft(0.0);
	float pdef(1.f);
	
	//Main Loop.
	bool aKey(false);
	bool zKey(false);
	
	while(window.IsOpened())
	{
		sf::Event event;
		while(window.GetEvent(event))
		{
			if(event.Type == sf::Event::Closed)
			{
				window.Close();
			}
			
			if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape))
			{
				window.Close();
			}
			
			if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::A))
			{
				aKey = true;
			}
			
			if((event.Type == sf::Event::KeyReleased) && (event.Key.Code == sf::Key::A))
			{
				aKey = false;
			}
			
			if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Z))
			{
				zKey = true;
			}
			
			if((event.Type == sf::Event::KeyReleased) && (event.Key.Code == sf::Key::Z))
			{
				zKey = false;
			}
			
			if(event.Type == sf::Event::Resized)
			{
				glViewport(0, 0, event.Size.Width, event.Size.Height);
			}
		}
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		vec4 light = { sin(tclock.GetElapsedTime()), cos(tclock.GetElapsedTime()), 3, 2 };
		
		float scaleIT = (1/M_PI)*atan(tan((tclock.GetElapsedTime()*10-M_PI)/2))*3;
		
		if(scaleIT < 0 && !aKey) {
			pdef = pdef + 0.02;
		}
		if(scaleIT > 0 && !aKey) {
			pdef = pdef - 0.02;
		}
		
		if(zKey)
		{
			difft = tclock.GetElapsedTime() - actt;
		}
		if(!zKey)
		{
			actt = tclock.GetElapsedTime() - difft;
		}
		
		matrix.init(&world);
		matrix.lookAt(&world, camera, lookPt, vertical);
		matrix.translate(&world, -0.5, -0.5, -0.5);
		matrix.rotate(&world, -90.0f, 1.0f, 0.0f, 0.0f);
		//matrix.rotate(&world, actt*100, 0.0f, 0.0f, 1.0f);
		matrix.rotate(&world, event.MouseMove.X, 0.0f, 0.0f, 1.0f);
		matrix.rotate(&world, event.MouseMove.Y, 0.0f, 1.0f, 0.0f);
		
		glBindBuffer(GL_UNIFORM_BUFFER, GlobalMatricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), world.val);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), projection.val);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		glUseProgram(axesProg);
		
		glBindVertexArray(vaoAxes);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);
		
		glUseProgram(program);
		
		glBindVertexArray(i_vao);
		glUniform4f(light_dirLoc, light.x, light.y, light.z, light.w);
		glUniform1f(vertLoopLoc, 5.f);
		glUniform1f(fragLoopLoc, 5.f);
		glDrawElements(GL_TRIANGLES, (sizeof(s_indices[0])*s_indices.size()), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		
		glUseProgram(0);
		
		window.Display();
	}
	
	shader.Remove_p(program);
	
	return EXIT_SUCCESS;
}
