//      OBJLoader.cxx
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


#include "OBJLoader.hxx"

using namespace std;

Obj::Obj() {}

void Obj::load(string filename, vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements)
{
	ifstream file(filename.c_str(), ios::in);
	if (!file) { cerr << "Erreur: Impossible d'ouvrir : " << filename << endl; exit(EXIT_FAILURE); }
	string line;
	
	while(getline(file, line))
	{
		if(line.substr(0,2) == "v ")
		{
			istringstream str(line.substr(2));
			glm::vec3 vertex;
			str >> vertex.x;
			str >> vertex.y;
			str >> vertex.z;
			vertices.push_back(vertex);
		}
		else if(line.substr(0,2) == "f ")
		{
			istringstream str(line.substr(2));
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
}

void Obj::arrayVec3toArrayFloat(vector<glm::vec3> arrayVec, float* arrayFloat)
{
	for(unsigned int i = 0; i < arrayVec.size(); ++i)
	{
		arrayFloat[i*3] = arrayVec[i].x;
		arrayFloat[i*3+1] = arrayVec[i].y;
		arrayFloat[i*3+2] = arrayVec[i].z;
	}
}

