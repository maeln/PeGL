// PeGL.cxx
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


#include "deps.hxx"
#include "objectmanager.hxx"
#include <SFML/Window.hpp>
#include <locale>

using namespace std;

int main(int argc, char **argv)
{
	// Locale pour les caractéres français.
	setlocale(LC_ALL, "");
	
	// Créer la fenêtre.
	sf::Window window(sf::VideoMode(800, 600), "PeGL. FPS: 0");
	//window.setVerticalSyncEnabled(true); // VSync activé.

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
	
	// Charge la texture.
	PeGL::ObjectManager objmanager;
	PeGL::PeDW suzanne = objmanager.load_PeDW("Ressources/suzanne.obj", "Ressources/udlr.png", "Shader/Light.vert", "Shader/Light.frag");
	
	sf::Clock tclock;
	float mouse_x(0);
	float mouse_y(0);
	sf::Clock Time;
	unsigned int delta_frame(0);
	float delta_time(Time.getElapsedTime().asSeconds());
	float current_time(0);
	float fps(0);
	float rot_time(Time.getElapsedTime().asSeconds());
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				glDeleteShader(suzanne.shaders.shader.back());
				glDeleteShader(suzanne.shaders.shader.front());
				objmanager.clean_PeDW(suzanne);
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
		
		//Compteur de fps.
		++delta_frame;
		current_time = Time.getElapsedTime().asSeconds();
		if(current_time > (delta_time+0.1))
		{
			delta_time = current_time - delta_time;
			fps = delta_frame / delta_time;
			ostringstream os;
			os << fps;
			static string framerate = "PeGL. FPS: " + os.str();
			window.setTitle(framerate);
			delta_time = Time.getElapsedTime().asSeconds();
			delta_frame = 0;
		}

		// On lave la fenêtre :
		glClearColor(13.f/255.f, 37.f/255.f, 78.f/255.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// On initialise les données nécessaires à l'affichage du Suzanne.
		glm::mat4 normm(glm::transpose(projection));
		glm::vec4 light(0, 2, 1, sqrt(5.0));
		
		if(rot_time+0.0001 < Time.getElapsedTime().asSeconds())
		{
			camera = glm::rotate(camera, (float)(M_PI/180.f)*5, glm::vec3(0,1,0));
			rot_time = Time.getElapsedTime().asSeconds();
		}
		
		objmanager.draw_PeDW(suzanne, light, camera, projection, normm);

		// On affiche la fenêtre :
		window.display();
	}
	
	return EXIT_SUCCESS;
}
