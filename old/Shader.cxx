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


#include "Shader.hxx"

using namespace std;

Shader::Shader() {}

string Shader::LoadShader(string filename, vector<string> &uniform)
{
	string source;
	
	ifstream source_file(filename.c_str());
	if(!source_file) { cerr << "Erreur: Impossible de lire le fichier : " << filename << endl; exit(EXIT_FAILURE); }
	string line;
	while(getline(source_file, line)) // On lit le shader ligne par ligne.
	{
		source += (line + '\n');
		if(line.substr(0, 8) == "uniform ") // Si la ligne commence par «uniform», on note le nom de l'uniform dans un tableau
		{									// pour faciliter l'utilisation des shaders plus tard.
			istringstream str(line.substr(7));
			string type; string name;
			str >> type;
			str >> name;
			name.erase(name.find(";"), 1);  // La déclaration d'un uniform finit toujours par «;» comme une déclaration en C. Il n'est pas utile de le garder dans le nom de l'uniform.
			uniform.push_back(name);
		}
	}
	
	return source;
	
}

GLuint Shader::CompileShader(GLenum shaderType, string shaderPath, vector<string> &uniform)
{
	bool error(false);
	GLenum shader(glCreateShader(shaderType));
	if(shader == 0) {
		cerr << "Erreur: Problème lors de la création d'un shader : " << shaderPath << endl;
		error = true;
	}
	
	GLint state_s(GL_TRUE);
	GLint logSize(0);
	char *log(NULL);
	string src(Shader::LoadShader(shaderPath, uniform));
	const char* src_char = src.c_str();
	glShaderSource(shader, 1, &src_char, NULL);
	glCompileShader(shader);
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
		cerr << "Erreur: Impossible de compiler le Shader : " << shaderPath << " : " << endl << log << endl;
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

void Shader::InitUniformLocation(GLuint program, vector<string> &uniform_name, map<string,GLuint> &uniform)
{
	for(unsigned int i(0); i < uniform_name.size(); i++)
	{
		uniform[uniform_name[i]] = glGetUniformLocation(program, uniform_name[i].c_str());
	}
}
