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
#include <fstream>
#include <locale>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include "Shader.hxx"
#include "VBHandler.hxx"
#include "Particle.hxx"
#include "OBJLoader.hxx"
#include "meshmanager.hxx"

using namespace std;

int main(int argc, char **argv)
{
	// Locale pour les caractéres français.
	setlocale(LC_ALL, "");
	
	// Créer la fenêtre.
	sf::Window window(sf::VideoMode(800, 600), "Générateur de particule.");
	window.setVerticalSyncEnabled(true); // VSync activé.

	// On initialise Glew :
	GLenum err = glewInit();
	if (GLEW_OK != err) // Vérifie que Glew est bien initialisé, sinon, renvoi l'erreur.
	{
		cerr << "Erreur: " << glewGetErrorString(err) << endl;
	}

	cout << "Info: Glew " << glewGetString(GLEW_VERSION) << " en cours d'utilisation." << endl;

	if (!glewIsSupported("GL_VERSION_3_3")) // On vérifie que OpenGL 3.3 est bien disponible sinon on ferme le programme.
	{
		cerr << "Erreur: OpenGL 3.3 n'est pas disponible sur votre système.";
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, 800, 600);

	// Tout ce qui concerne le Depth Test ( gestion de la profondeur ).
	glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.f, 1.f);
	glEnable(GL_DEPTH_CLAMP);

	// Création d'un objet Matrix et initialisation des différentes matrices nécessaire au programme.
	glm::mat4 projection = glm::perspective(90.f, 800.f/600.f, 1.f, 10.f);
	glm::mat4 camera = glm::lookAt(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 transformation;

	// On créer un objet Shader et on créer les programs nécessaires.
	Shader shader;
	
	// Création du shader pour Suzanne.
	vector<string> SuzaUniName;
	map<string,GLuint> SuzaUni;
	GLuint Suzanne_vertex_shader(shader.CompileShader(GL_VERTEX_SHADER, "Shader/Light.vert", SuzaUniName));
	GLuint Suzanne_frag_shader(shader.CompileShader(GL_FRAGMENT_SHADER, "Shader/Light.frag", SuzaUniName));
	GLuint Suzanne_prog(shader.MakeProgram(Suzanne_vertex_shader, Suzanne_frag_shader));
	shader.Remove_s(Suzanne_vertex_shader);
	shader.Remove_s(Suzanne_vertex_shader);
	
	// Gestion de Uniform pour les shader Suzannes.
	shader.InitUniformLocation(Suzanne_prog, SuzaUniName, SuzaUni);
	
	// On initialise les particules :
	//~ ParticleGenerator GenPart(glm::vec3(0, 0, 0), 1000, 100, 100, glm::vec3(0, 1, 0), (M_PI/4.0), glm::vec3(255,255,255));
	
	// On charge un model OBJ.
	Obj objloader;
	MeshManager meshmanager;
	
	// Stockage des vertices et normales.
	vector<glm::vec3> vertices;	
	vector<glm::vec3> normals;	
	vector<GLushort> elements;
	objloader.load("suzanne.obj", vertices, normals, elements);
	int suzanne_id(meshmanager.add_mesh(vertices, normals, elements));
	//vertices.clear(); normals.clear(); elements.clear();
	
	
	
	// Temps & évenements :
	sf::Clock tclock;
	float mouse_x(0);
	float mouse_y(0);

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				glClearColor(13.f/255.f, 37.f/255.f, 78.f/255.f, 0.f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				vertices.clear(); normals.clear(); elements.clear();
				objloader.~Obj();
				meshmanager.~MeshManager();
				
				window.close();
			}

			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			{
				glClearColor(13.f/255.f, 37.f/255.f, 78.f/255.f, 0.f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				vertices.clear(); normals.clear(); elements.clear();
				objloader.~Obj();
				meshmanager.~MeshManager();
				
				window.close();
			}

			if(event.type == sf::Event::Resized)
			{
				glViewport(0, 0, (float)event.size.width, (float)event.size.height);
				projection = glm::perspective(90.f, (float)event.size.width/(float)event.size.height, 1.f, 10.f);
			}

			if(event.type == sf::Event::MouseMoved)
			{
				mouse_x = event.mouseMove.x;
				mouse_y = event.mouseMove.y;
			}
		}

		// On lave la fenêtre :
		glClearColor(13.f/255.f, 37.f/255.f, 78.f/255.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// On initialise les données nécessaires à l'affichage du Suzanne.
		glm::mat4 normm(glm::transpose(projection));
		glm::vec4 light(0, 2, 1, 1);
		
		glUseProgram(Suzanne_prog);
		
		// On affiche suzanne.
		glBindVertexArray(suzanne_id);
		glUniformMatrix4fv(SuzaUni["world"], 1, GL_FALSE, glm::value_ptr(camera));
		glUniformMatrix4fv(SuzaUni["perspective"], 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix3fv(SuzaUni["normalMatrix"], 1, GL_FALSE, glm::value_ptr(normm));
		glUniform4fv(SuzaUni["light_dir"], 1, glm::value_ptr(light));
		glDrawElements(GL_TRIANGLES, (sizeof(elements[0])*elements.size()), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		

		// On affiche la fenêtre :
		window.display();
	}

	return EXIT_SUCCESS;
}

