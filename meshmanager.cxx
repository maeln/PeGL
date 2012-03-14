// meshmanager.cxx
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

#include "meshmanager.hxx"

using namespace std;

Mesh::Mesh(vector<glm::vec3> &vert, vector<glm::vec3> &norm, vector<GLushort> &ele)
{
	vertices = vert;
	normals = norm;
	elements = ele;
}

Mesh::~Mesh()
{
	delete &vertices;
	delete &normals;
	delete &elements;
}

vector<glm::vec3> Mesh::s_vertices()
{
	return vertices;
}

vector<glm::vec3> Mesh::s_normals()
{
	return normals;
}

vector<GLushort> Mesh::s_elements()
{
	return elements;
}

MeshManager::MeshManager()
{
	vector<Mesh> meshdb;
}


MeshManager::~MeshManager()
{
	while(meshdb.size()>0)
	{
		meshdb[0].~Mesh();
		meshdb.erase(meshdb.begin());
	}
}

int MeshManager::add_mesh(vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements)
{
	Mesh mesh(vertices, normals, elements);
	 meshdb.push_back(mesh);
	 return meshdb.size();
}
