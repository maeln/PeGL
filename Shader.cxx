//      Shader.cxx
//      
//      Copyright 2011 Mael N. <maeln@maeln.com>
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
#include <math.h>
#include "Shader.hxx"

using namespace std;

Shader::Shader() {}

char* Shader::LoadShader(string filename)
{
	bool error(false);
	char *source(NULL);
	long size;
	long i(0);
	ifstream mfile(filename.c_str());
	if(mfile == NULL)
	{
		cerr << "Erreur: Impossible de lire le fichier ( " << filename << " )." << endl;
		error = true;
	}
	
	if(error == false)
	{
		cout << "Info: Shader en cours de lecture." << endl;
	}
	
	mfile.seekg(0, ios::end);
	size = mfile.tellg();
	mfile.seekg(0, ios::beg);
	
	source = new char[size+1];
	
	if(source == NULL) {
		cerr << "Erreur: Probléme lors de l'allocation mémoire ou de la lecture du fichier." << endl;
		error = true;
	}
	
	while(i < size)
	{
		mfile.get(source[i]);
		i++;
	}
	source[size] = '\0';
	
	if(error == false)
	{
		cout << "Info: Fin de la lecture." << endl;
	}
	
	mfile.close();
	
	return source;
}

GLuint Shader::CompileShader(GLenum shaderType, string shaderPath)
{
	bool error(false);
	GLenum shader(glCreateShader(shaderType));
	if(shader == 0) {
		cerr << "Erreur: Problème lors de la création d'un vertex shader." << endl;
		error = true;
	}
	
	GLint state_s(GL_TRUE);
	GLint logSize(0);
	char *log(NULL);
	const char *src(Shader::LoadShader(shaderPath));
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	delete(src);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &state_s);
	if(state_s != GL_TRUE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
		log = new char[logSize+1];
		if(log == NULL)
		{
			cerr << "Erreur: Impossible d'allouer la mémoire lors de la récupération du Log." << endl;
			error = true;
		}
		glGetShaderInfoLog(shader, logSize, &logSize, log);
		log[logSize] = '\0';
		cerr << "Erreur: Impossible de compiler le Shader : " << log << endl;
		delete(log);
		glDeleteShader(shader);
		error = true;
	}
	
	if(error == false)
	{
		cout << "Info: Shader créer avec succès." << endl;
	}
	
	return shader;
}

GLuint Shader::MakeProgram(GLuint vertexshader, GLuint pixelshader)
{
	bool error(false);
	GLuint program(glCreateProgram());
	GLint logSize(0);
	GLint state_p(GL_TRUE);
	char *log(NULL);
	
	if(vertexshader != 0)
	{
		glAttachShader(program, vertexshader);
	}
	if(pixelshader != 0)
	{
		glAttachShader(program, pixelshader);
	}
	if(pixelshader == 0 && vertexshader == 0)
	{
		cerr << "Erreur: Aucun Shader à attacher au Program." << endl;
		error = true;
	}
	
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &state_p);
	if(state_p != GL_TRUE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		log = new char[logSize+1];
		if(log == NULL)
		{
			cerr << "Erreur: Impossible d'allouer la mémoire lors de la récupération du Log." << endl;
			error = true;
		}
		glGetProgramInfoLog(program, logSize, &logSize, log);
		log[logSize] = '\0';
		cerr << "Erreur: Impossible de lié le Program : " << log << endl;
		delete(log);
		glDeleteProgram(program);
		error = true;
	}
	
	if(error == false)
	{
		cout << "Info: Program lié avec succès." << endl;
	}
	return program;
}

void Shader::Remove_s(GLuint shader)
{
	glDeleteShader(shader);
}

void Shader::Remove_p(GLuint program)
{
	glDeleteProgram(program);
}
