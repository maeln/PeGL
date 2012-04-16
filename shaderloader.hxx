// shaderloader.hxx
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


#ifndef SHADERLOADER_HXX
#define SHADERLOADER_HXX

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <string>
#include <map>
#include <stdlib.h>

namespace PeGL
{
	typedef struct
	{
		GLuint addr;
		std::vector<std::string> uniform;
	} shader;
	
	typedef struct
	{
		GLuint addr;
		std::vector<GLuint> shader;
		std::map<std::string, GLuint> uniform;
	} program;
	
	class ShaderLoader
	{
		public:
			ShaderLoader();
			virtual ~ShaderLoader();
			shader 	loadShader(std::string filename, GLenum shader_type); // charge et compile le shader.
			program	createProgram(std::vector<shader> pshader);
		
		private:
			/* add your private declarations */
	};
}

#endif /* SHADERLOADER_HXX */ 
