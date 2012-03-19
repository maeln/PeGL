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


#include "meshmanager.hxx"

using namespace std;

MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
	while(meshdb.size()>0)
	{
		meshdb.erase(meshdb.begin());
	}
}

int MeshManager::add_mesh(vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements)
{
	Obj objloader;
	
	MeshVAO meshVAO;
	
	float vertices_f[vertices.size()*3];
	float normals_f[normals.size()*3];
	objloader.arrayVec3toArrayFloat(vertices, vertices_f);
	objloader.arrayVec3toArrayFloat(normals, normals_f);
	
	// Création des VBO.
	GLuint s_vertices_vbo;
	glGenBuffers(1, &s_vertices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertices_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_f), vertices_f, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint s_normals_vbo;
	glGenBuffers(1, &s_normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals_f), normals_f, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint s_indices_vbo;
	glGenBuffers(1, &s_indices_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indices_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(elements[0])*elements.size()), elements.data(), GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	GLuint i_vao;
	glGenVertexArrays(1, &i_vao);
	glBindVertexArray(i_vao);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertices_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, s_normals_vbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	meshVAO.vao_loc = i_vao;
	
	int id(meshdb.size());
	meshdb[id] = meshVAO;
	
	return meshdb.size();
}
