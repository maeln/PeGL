// shaderloader.cxx
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


#include "shaderloader.hxx"

namespace PeGL
{

ShaderLoader::ShaderLoader()
{
	
}


ShaderLoader::~ShaderLoader()
{
	
}

PeShader ShaderLoader::loadShader(std::string filename, GLenum shader_type)
{
	std::string source;
	PeShader pshader;
	
	std::ifstream source_file(filename.c_str());
	if(!source_file) { std::cerr << "Erreur: Impossible de lire le fichier : " << filename << std::endl; exit(EXIT_FAILURE); }
	std::string line;
	while(getline(source_file, line)) // On lit le shader ligne par ligne.
	{
		source += (line + '\n');
		if(line.substr(0, 8) == "uniform ") // Si la ligne commence par «uniform», on note le nom de l'uniform dans un tableau
		{									// pour faciliter l'utilisation des shaders plus tard.
			std::istringstream str(line.substr(7));
			std::string type; std::string name;
			str >> type;
			str >> name;
			name.erase(name.find(";"), 1);  // La déclaration d'un uniform finit toujours par «;» comme une déclaration en C. Il n'est pas utile de le garder dans le nom de l'uniform.
			pshader.uniform.push_back(name);
		}
	}
	
	bool error(false);
	GLuint tmp(glCreateShader(shader_type));
	if(tmp == 0) {
		std::cerr << "Erreur: Problème lors de la création d'un shader : " << filename << std::endl;
		error = true;
	}
	
	GLint state_s(GL_TRUE);
	GLint logSize(0);
	char *log(NULL);
	const char* src_char = source.c_str();
	glShaderSource(tmp, 1, &src_char, NULL);
	glCompileShader(tmp);
	glGetShaderiv(tmp, GL_COMPILE_STATUS, &state_s);
	if(state_s != GL_TRUE)
	{
		glGetShaderiv(tmp, GL_INFO_LOG_LENGTH, &logSize);
		log = new char[logSize+1];
		if(log == NULL)
		{
			std::cerr << "Erreur: Impossible d'allouer la mémoire lors de la récupération du Log." << std::endl;
			error = true;
		}
		glGetShaderInfoLog(tmp, logSize, &logSize, log);
		log[logSize] = '\0';
		std::cerr << "Erreur: Impossible de compiler le Shader : " << filename << " : " << std::endl << log << std::endl;
		delete(log);
		glDeleteShader(tmp);
		error = true;
	}
	
	if(error == false)
	{
		std::cout << "Info: Shader créer avec succès." << std::endl;
		pshader.addr = tmp;
	}
	
	return pshader;
}

PeProgram ShaderLoader::createProgram(std::vector<PeShader> pshader)
{
	PeProgram oprogram;
	GLuint tmpProg(glCreateProgram());
	GLint logSize(0);
	GLint state_p(GL_TRUE);
	char *log(NULL);
	bool error(false);
	
	if(pshader.size() > 0)
	{
		for(unsigned int i=0; i < pshader.size(); ++i)
		{
			glAttachShader(tmpProg, pshader[i].addr);
			oprogram.shader.push_back(pshader[i].addr);
		}
		glLinkProgram(tmpProg);
		glGetProgramiv(tmpProg, GL_LINK_STATUS, &state_p);
		if(state_p != GL_TRUE)
		{
			glGetProgramiv(tmpProg, GL_INFO_LOG_LENGTH, &logSize);
			log = new char[logSize+1];
			if(log == NULL) { std::cerr << "Erreur: Impossible d'allouer la mémoire lors de la récupération du Log." << std::endl; exit(EXIT_FAILURE); }
			glGetProgramInfoLog(tmpProg, logSize, &logSize, log);
			log[logSize] = '\0';
			std::cerr << "Erreur: Impossible de lier le Program : " << log << std::endl;
			delete(log);
			glDeleteProgram(tmpProg);
			error = true;
		}
	} else {
		std::cerr << "Erreur: Pas de shader à lier au program." << std::endl;
	}
	
	if(error == false)
	{
		std::cout << "Info: Program lié avec succès." << std::endl;
		oprogram.addr = tmpProg;
		for(unsigned int i=0; i < pshader.size(); ++i)
		{
			for(unsigned int n=0; i < pshader[i].uniform.size(); ++n)
			{
				oprogram.uniform[pshader[i].uniform[n]] = glGetUniformLocation(oprogram.addr, pshader[i].uniform[n].c_str());
			}
		}
	}
	
	return oprogram;
}

}
