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


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "OBJLoader.hxx"
#include "Matrix.hxx"

using namespace std;

Obj::Obj() {}

void Obj::load(string filename, vector<vec3> &vertices, vector<vec3> &normals, vector<GLushort> &elements)
{
	ifstream file(filename.c_str(), ios::in);
	if (!file) { cerr << "Erreur: Impossible d'ouvrir : " << filename << endl; exit(1); }
	string line;
	
	while(getline(file, line))
	{
		if(line.substr(0,2) == "v ")
		{
			istringstream str(line.substr(2));
			vec3 vertex;
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
			a--; b--; c--;
			elements.push_back(a);
			elements.push_back(b);
			elements.push_back(c);
		}
		else
		{
			/* NOTHJING, yeah, nothJing. */
		}
	}
		
	vec3 tmp; tmp.x = 0.f; tmp.y = 0.f; tmp.z = 0.f;
	Matrix mat;
	normals.resize(vertices.size(), tmp);
	for(unsigned int i = 0; i < elements.size(); i += 3)
	{
		GLushort ia = elements[i];
		GLushort ib = elements[i+1];
		GLushort ic = elements[i+2];
		
		vec3 tmp1 = mat.minus(vertices[ib], vertices[ia]);
		vec3 tmp2 = mat.minus(vertices[ic], vertices[ia]);
		vec3 normal = mat.cross(tmp1, tmp2);
		mat.normalizeVector(&tmp);
		normals[ia] = normals[ib] = normals[ic] = normal;
	}
}

/*
void Obj::load(std::string filename)
{
	vector<vec3> v, vn;
	vector<unsigned int> iv,it,in;
	ifstream file(filename.c_str(), ios::in);
	string line;
	
	if(line[0] == 'v')
	{
		if(line[1] == ' ') //It's a Vertex.
		{
			char x[255], y[255], z[255];
			sscanf(line.c_str(), "v %s %s %s", x, y, z);
			vec3 vec;
			vec.x = atof(x);
			vec.y = atof(y);
			vec.z = atof(z);
			v.push_back(vec);
		}
		else if(line[1] == 'n') //It's a Normal.
		{
			char x[255], y[255], z[255];
			sscanf(line.c_str(), "vn %s %s %s", x, y, z);
			vec3 vec;
			vec.x = atof(x);
			vec.y = atof(y);
			vec.z = atof(z);
			vn.push_back(vec);
		}
	}
	else if(line[0] == 'f')
	{
		line = Obj::dealWithSlash(line);
		vector<string> face = splitSpace(line.substr(2));
		int data = (int)face.size() / 3;
		
		for(int i = 0; i < (data==3?3:4); i++)
		{
			iv.push_back(strtol(face[i*3].c_str(), NULL, 10) - 1);
			it.push_back(strtol(face[i*3+1].c_str(), NULL, 10) - 1);
			in.push_back(strtol(face[i*3+2].c_str(), NULL, 10) - 1);
		}
		if(data == 3)
		{
			iv.push_back(strtol(face[0].c_str(), NULL, 10) - 1);
			it.push_back(strtol(face[1].c_str(), NULL, 10) - 1);
			in.push_back(strtol(face[2].c_str(), NULL, 10) - 1);
		}
	}
}
*/

/*
string Obj::dealWithSlash(string line)
{
	string s;
	for(int i=0; i<line.size(); i++)
	{
		if(i<line.size()-1 && line[i] == '/' && line[i+1] == '/')
		{
			s += "/1/";
			i++;
		}
		else
		{
			s += line[i];
		}
	}
	
	string d;
	for(int i = 0; i<s.size(); i++)
	{
		if(s[i] == '/')
		{
			d += ' ';
		}
		else
		{
			d += s[i];
		}
	}
	
	return d;
}

vector<string> splitSpace(string line)
{
	vector<string> result;
	string s;
	for(int i; i<line.size(); i++)
	{
		if(line[i] == ' ' || i == line.size()-1)
		{
			if(i == s.size()-1)
			{
				s += line[i];
			}
			result.push_back(s);
			s="";
		}
		else
		{
			s += line[i];
		}
	}
	
	return result;
}

float* vectorToFloatArray(std::vector<float>& array)
{
	float* res = NULL;
	res = (float*)malloc(array.size()*sizeof(float));
	if(res == NULL || array.empty())
	{
		
}
*/
