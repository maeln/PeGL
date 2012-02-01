//      Particle.cxx
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


#define GLM_SWIZZLE
#define MAX_PARTICLES 1000
#define GRAVITY 9.81

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include "Shader.hxx"
#include "VBHandler.hxx"
#include "Particle.hxx"

using namespace std;

/* TO DO :
 * - Utiliser les VBOs.
 * - ID : Renvoyer l'indice du VBO au lieu d'un Array.
 * - Renvoyer l'Offset pour vertex/color.
 */ 

ParticleGenerator::ParticleGenerator(glm::vec3 position, unsigned int maxpart, unsigned int emitamount, unsigned int lifepart, glm::vec3 initvect, float disp, glm::vec3 c)
{
	srand(time(NULL));
	InitPosition = position;
	MaxParticle = maxpart;
	InitVector = initvect;
	Dispertion = (float)glm::clamp(disp, 0.f, 3.14159f);
	PartEmit = emitamount;
	Partlife = lifepart;
	color = c;
	glGenBuffers(1, &addr_vbo);
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::update()
{
	// On rajoute des Particules.
	if(ArrPart.size() <= MaxParticle)
	{
		unsigned int NumberOfParticle = rand() % PartEmit;
		for(unsigned int i=0; i < NumberOfParticle; i++)
		{
			particules tmpPart;
			tmpPart.alive = true;
			tmpPart.life = rand() % Partlife;
			tmpPart.couleur = color;
			tmpPart.position = InitPosition;
			tmpPart.direction = GenerateDirection();
			ArrPart.push_back(tmpPart);
		}
		if(ArrPart.size() > MaxParticle)
		{
			int diff = ArrPart.size() - MaxParticle;
			for(int i=0; i<diff; i++)
			{
				ArrPart.pop_back();
			}
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	// On met à jour celle déjà créer.
	if(ArrPart.size() > 0)
	{
		for(unsigned int n=0; n < ArrPart.size(); n++)
		{
			if(ArrPart[n].life <= 0)
				ArrPart[n].alive = false;
				
			if(ArrPart[n].alive)
			{
				ArrPart[n].life--;
				ArrPart[n].position.x += ArrPart[n].direction.x;
				ArrPart[n].position.y += ArrPart[n].direction.y;
				ArrPart[n].position.z += ArrPart[n].direction.z;
				//ArrPart[n].direction.y -= GRAVITY/1000.f;
			}
			if(!ArrPart[n].alive)
			{
				ArrPart[n].alive = true;
				ArrPart[n].life = rand() % Partlife;
				ArrPart[n].position = InitPosition;
				ArrPart[n].couleur = color;
				ArrPart[n].direction = GenerateDirection();
			}
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

void ParticleGenerator::updateVBO()
{
	const int Offset = ArrPart.size();
	//~ float part_position[Offset*3];
	//~ float part_color[Offset*3];
	float part[Offset*3+Offset*3];
	
	if(ArrPart.size() > 0)
	{
		for(int i=0; i<Offset; i++)
		{
			part[i*3]	= ArrPart[i].position.x;
			part[i*3+1]	= ArrPart[i].position.y;
			part[i*3+2]	= ArrPart[i].position.z;
		}
	}
	
	//Créer un tableaux valide ?
	
	if(ArrPart.size() > 0)
	{
		for(int i=Offset; i<(Offset*2); i++)
		{
			part[i*3]	= ArrPart[i].couleur.x;
			part[i*3+1]	= ArrPart[i].couleur.y;
			part[i*3+2]	= ArrPart[i].couleur.z;
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, addr_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(part), part, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleGenerator::SendVertexArray(float part_position[])
{
	if(ArrPart.size() > 0)
	{
		for(unsigned int i=0; i<ArrPart.size(); i++)
		{
			part_position[i*3]		= ArrPart[i].position.x;
			part_position[i*3+1]	= ArrPart[i].position.y;
			part_position[i*3+2]	= ArrPart[i].position.z;
		}
	}
}

void ParticleGenerator::SendColorArray(float part_color[])
{
	if(ArrPart.size() > 0)
	{
		for(unsigned int i=0; i<ArrPart.size(); i++)
		{
			part_color[i*3]		= ArrPart[i].couleur.x;
			part_color[i*3+1]	= ArrPart[i].couleur.y;
			part_color[i*3+2]	= ArrPart[i].couleur.z;
		}
	}
}

glm::vec3 ParticleGenerator::GenerateDirection()
{
	//Version NON Trigonométrique.
	glm::vec3 vecteur_direction;
	glm::vec3 result;
	//float randseed = ((rand() % 100) - (rand() % 100))/100.f;
	vecteur_direction.x = InitVector.x + ((rand() % 100) - (rand() % 100))*Dispertion/100.f;
	vecteur_direction.y = InitVector.y + ((rand() % 100) - (rand() % 100))*Dispertion/100.f;
	vecteur_direction.z = 0;
	return vecteur_direction/100.f;
}

int ParticleGenerator::ArrPartSize()
{
	return ArrPart.size();
}

size_t ParticleGenerator::SendOffset()
{
	return sizeof(float)*(ArrPart.size()*3);
}

glm::vec3 ParticleGenerator::S_InitVector()
{
	return InitVector;
}

void ParticleGenerator::U_InitVector(glm::vec3 up)
{
	InitVector = up;
}
