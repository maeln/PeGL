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

using namespace std;

int main(int argc, char **argv)
{
	// Créer la fenêtre.
	sf::Window window(sf::VideoMode(800, 600), "Générateur de particule.");
	window.EnableVerticalSync(true); // VSync activé.

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
	GLuint VShader(shader.CompileShader(GL_VERTEX_SHADER, "Shader/Particles.vert"));
	GLuint FShader(shader.CompileShader(GL_FRAGMENT_SHADER, "Shader/Particles.frag"));
	GLuint Program(shader.MakeProgram(VShader, FShader));
	shader.Remove_s(VShader);
	shader.Remove_s(FShader);
	
	// Création du shader pour Suzanne.
	GLuint Suzanne_vertex_shader(shader.CompileShader(GL_VERTEX_SHADER, "Shader/Light.vert"));
	GLuint Suzanne_frag_shader(shader.CompileShader(GL_FRAGMENT_SHADER, "Shader/Light.frag"));
	GLuint Suzanne_prog(shader.MakeProgram(Suzanne_vertex_shader, Suzanne_frag_shader));
	shader.Remove_s(Suzanne_vertex_shader);
	shader.Remove_s(Suzanne_vertex_shader);
	
	// Gestion de Uniform pour les shader Suzannes.
	GLuint SuWorld(glGetUniformLocation(Suzanne_prog, "world"));
	GLuint SuPersp(glGetUniformLocation(Suzanne_prog, "perspective"));
	GLuint SuNormM(glGetUniformLocation(Suzanne_prog, "normalMatrix"));
	GLuint SuLight(glGetUniformLocation(Suzanne_prog, "light_dir"));

	// On initialise les particules :
	ParticleGenerator GenPart(glm::vec3(0, 0, 0), 1000, 100, 100, glm::vec3(0, 1, 0), (M_PI/4.0), glm::vec3(255,255,255));
	
	// On charge un model OBJ.
	Obj objloader;
	
	// Stockage des vertices et normales.
	vector<glm::vec3> vertices;	
	vector<glm::vec3> normals;	
	vector<GLushort> elements;
	objloader.load("suzanne.obj", vertices, normals, elements);
	
	// Transformation en tableaux de Float.
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
	
	// Création du VAO pour «Suzanne».
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
	
	
	// Temps & évenements :
	sf::Clock tclock;
	float mouse_x(0);
	float mouse_y(0);

	while(window.IsOpen())
	{
		sf::Event event;
		while(window.PollEvent(event))
		{
			if(event.Type == sf::Event::Closed)
				window.Close();

			if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Keyboard::Escape))
				window.Close();

			if(event.Type == sf::Event::Resized)
				glViewport(0, 0, (float)event.Size.Width, (float)event.Size.Height);

			if(event.Type == sf::Event::MouseMoved)
			{
				mouse_x = event.MouseMove.X;
				mouse_y = event.MouseMove.Y;
			}
		}

		// On lave la fenêtre :
		glClearColor(13.f/255.f, 37.f/255.f, 78.f/255.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//On met à jour les particules.
		GenPart.update();
		glm::vec3 rot_vec(cos(tclock.GetElapsedTime().AsMilliseconds()), sin(tclock.GetElapsedTime().AsMilliseconds()), 0);
		GenPart.U_InitVector(rot_vec);
		GenPart.updateVBO();
		//~ cout << GenPart.SendOffset() << endl;
		//~ cout << GenPart.ArrPartSize() << endl;
		
		//On créer les tableaux;
		int size = GenPart.ArrPartSize();
		float part_position[size*3];
		float part_color[size*3];
		
		//Et on les remplies :
		GenPart.SendVertexArray(part_position);
		GenPart.SendColorArray(part_color);

		// On affiche tout le bordel.
		glUseProgram(Program);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, part_position);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, part_color);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_POINTS, 0, size);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUseProgram(0);
		
		// On initialise les données nécessaires à l'affichage du Suzanne.
		glm::mat4 normm(glm::transpose(projection));
		glm::vec4 light(0, 2, 1, 1);
		
		glUseProgram(Suzanne_prog);
		
		// On affiche suzanne.
		glBindVertexArray(i_vao);
		glUniformMatrix4fv(SuWorld, 1, GL_FALSE, glm::value_ptr(camera));
		glUniformMatrix4fv(SuPersp, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix3fv(SuNormM, 1, GL_FALSE, glm::value_ptr(normm));
		glUniform4fv(SuLight, 1, glm::value_ptr(light));
		glDrawElements(GL_TRIANGLES, (sizeof(elements[0])*elements.size()), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		

		// On affiche la fenêtre :
		window.Display();
	}

	return EXIT_SUCCESS;
}

