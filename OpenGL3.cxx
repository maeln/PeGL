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

using namespace std;

int main(int argc, char **argv)
{
	// Créer la fenêtre.
	sf::Window window(sf::VideoMode(800, 600), "Générateur de particule.");
	window.UseVerticalSync(true); // VSync activé.

	// On initialise Glew :
	GLenum err = glewInit();
	if (GLEW_OK != err) // Vérifie que Glew est bien initialisé, sinon, renvoi l'erreur.
	{
		cerr << "Erreur: " << glewGetErrorString(err) << endl;
	}

	cout << "Info: Glew " << glewGetString(GLEW_VERSION) << " en cours d'utilisation." << endl;

	if (!glewIsSupported("GL_VERSION_4_0")) // On vérifie que OpenGL 3.3 est bien disponible sinon on ferme le programme.
	{
		cerr << "Erreur: OpenGL 4.0 n'est pas disponible sur votre système.";
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
	glm::mat4 projection = glm::perspective(80.f, 4.f/3.f, 1.f, 10.f);
	glm::mat4 camera = glm::lookAt(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 transformation;

	// On créer un objet Shader et on créer les programs nécessaires.
	Shader shader;
	GLuint VShader(shader.CompileShader(GL_VERTEX_SHADER, "Shader/Particles.vert"));
	GLuint FShader(shader.CompileShader(GL_FRAGMENT_SHADER, "Shader/Particles.frag"));
	GLuint Program(shader.MakeProgram(VShader, FShader));
	shader.Remove_s(VShader);
	shader.Remove_s(FShader);

	// On initialise les particules :
	ParticleGenerator GenPart(glm::vec3(0, 0, 0), 1000, 100, 100, glm::vec3(0, 1, 0), (M_PI/4.0), glm::vec3(255,255,255));

	// Temps & évenements :
	sf::Clock tclock;
	float mouse_x(0);
	float mouse_y(0);

	while(window.IsOpened())
	{
		sf::Event event;
		while(window.GetEvent(event))
		{
			if(event.Type == sf::Event::Closed)
				window.Close();

			if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape))
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
		glm::vec3 rot_vec(cos(tclock.GetElapsedTime()*10.f), sin(tclock.GetElapsedTime()*10.f), 0);
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

		// On affiche la fenêtre :
		window.Display();
	}

	return EXIT_SUCCESS;
}

