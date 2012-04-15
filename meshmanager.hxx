// meshmanager.hxx
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


#ifndef MESHMANAGER_HXX
#define MESHMANAGER_HXX

#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OBJLoader.hxx"

typedef struct
{
	GLuint vao_loc;
	GLuint vbo_loc[3];
	size_t elements_size;
} MeshVAO;

class MeshManager
{
	public:
		MeshManager();
		virtual ~MeshManager();
		// Ajoute le mesh à la table de hashage et créer VBO/VAO.
		int add_mesh(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elments);
		MeshVAO r_mesh(int id);
		
	
	private:
		std::vector<MeshVAO> meshdb;
		/* add your private declarations */
};

#endif /* MESHMANAGER_HXX */ 
