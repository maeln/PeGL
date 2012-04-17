// meshloader.cxx
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


#include "meshloader.hxx"

namespace PeGL
{

MeshLoader::MeshLoader()
{
	
}


MeshLoader::~MeshLoader()
{
	
}

mesh MeshLoader::loadMesh(std::string filename)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLushort> elements;
	
	std::ifstream file(filename.c_str(), std::ios::in);
	if (!file) { std::cerr << "Erreur: Impossible d'ouvrir : " << filename << std::endl; exit(EXIT_FAILURE); }
	std::string line;
	
	while(getline(file, line))
	{
		if(line.substr(0,2) == "v ")
		{
			std::istringstream str(line.substr(2));
			glm::vec3 vertex;
			str >> vertex.x;
			str >> vertex.y;
			str >> vertex.z;
			vertices.push_back(vertex);
		}
		else if(line.substr(0,2) == "f ")
		{
			std::istringstream str(line.substr(2));
			GLushort a,b,c;
			str >> a;
			str >> b;
			str >> c;
			--a; --b; --c;
			elements.push_back(a);
			elements.push_back(b);
			elements.push_back(c);
		}
	}
		
	glm::vec3 tmp; tmp.x = 0.f; tmp.y = 0.f; tmp.z = 0.f;
	normals.resize(vertices.size(), tmp);
	for(unsigned int i = 0; i < elements.size(); i += 3)
	{
		GLushort ia = elements[i];
		GLushort ib = elements[i+1];
		GLushort ic = elements[i+2];
		
		glm::vec3 tmp1 = vertices[ib] - vertices[ia];
		glm::vec3 tmp2 = vertices[ic] - vertices[ia];
		glm::vec3 normal = glm::cross(tmp1, tmp2);
		tmp = glm::normalize(tmp);
		normals[ia] = normals[ib] = normals[ic] = normal;
	}
	
	float vertices_f[vertices.size()*3];
	float normals_f[normals.size()*3];
	vec3float(vertices, vertices_f);
	vec3float(normals, normals_f);
	
	mesh output_mesh;
	
	// Création des VBO.
	GLuint s_vertices_vbo;
	glGenBuffers(1, &s_vertices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertices_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_f), vertices_f, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	output_mesh.vbo.push_back(s_vertices_vbo);
	
	GLuint s_normals_vbo;
	glGenBuffers(1, &s_normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals_f), normals_f, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	output_mesh.vbo.push_back(s_normals_vbo);
	
	GLuint s_indices_vbo;
	glGenBuffers(1, &s_indices_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indices_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(elements[0])*elements.size()), elements.data(), GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	output_mesh.vbo.push_back(s_indices_vbo);
	
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
	
	output_mesh.vao = i_vao;
	output_mesh.elements_size = sizeof(elements[0])*elements.size();
	
	return output_mesh;
}

void MeshLoader::vec3float(std::vector<glm::vec3> vec, float* output)
{
	for(unsigned int i=0; i < vec.size(); ++i)
	{
		output[i*3] 	= vec[i].x;
		output[i*3+1] 	= vec[i].y;
		output[i*3+2]	= vec[i].z;
	}
}

}
