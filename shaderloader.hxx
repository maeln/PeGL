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

#include "deps.hxx"

/*!
 * \file shaderloader.hxx
 * \brief Gère le chargement des Shaders.
 * \author Maël N.
 * \version 0.1a
 * \date 09.07.2012
 */

namespace PeGL
{
	struct PeShader
	{
		GLuint addr;
		std::vector<std::string> uniform;
		GLenum type;
	};
	
	// Note : 
	// Les shaders ne sont pas effacé après création du Program afin de pouvoir être utilisé plusieurs fois
	// sans avoir à être recharger manuellement ( évite trop d'I/O ).
	
	struct PeProgram
	{
		GLuint addr;
		std::vector<GLuint> shader;
		std::map<std::string, GLuint> uniform;
	};
	
	class ShaderLoader
	{
		public:
			ShaderLoader();
			virtual ~ShaderLoader();
			PeShader 	loadShader(std::string filename, GLenum shader_type); // charge et compile le shader.
			PeProgram	createProgram(std::vector<PeShader> pshader);
		
		private:
			/* add your private declarations */
	};
}

#endif /* SHADERLOADER_HXX */ 
