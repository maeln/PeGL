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
#include "matrixstack.hxx"

#include <SFML/Window.hpp>
#include <locale>

using namespace std;

int main(int argc, char **argv)
try {
	// Locale pour les caractéres français.
	setlocale(LC_ALL, "");
	
	//Contexte de la fenêtre.
	sf::ContextSettings context(0, 0, 0, 3, 3);
	
	// Créer la fenêtre.
	sf::Window window(sf::VideoMode(800, 600), "PeGL. FPS: 0", sf::Style::Default, sf::ContextSettings(0, 0, 0, 3, 3));
	//window.setVerticalSyncEnabled(true); // VSync activé.
	//window.setMouseCursorVisible(false);
	
	// On initialise Glew :
	GLenum err = glewInit();
	if (GLEW_OK != err) // Vérifie que Glew est bien initialisé, sinon, renvoi l'erreur.
	{
		cerr << "Erreur: " << glewGetErrorString(err) << endl;
	}

	cout << "Info: Glew " << glewGetString(GLEW_VERSION) << " en cours d'utilisation." << endl;
	
	/*
	if (!glewIsSupported("GL_VERSION_3_3")) // On vérifie que OpenGL 3.3 est bien disponible sinon on ferme le programme.
	{
		cerr << "Erreur: OpenGL 3.3 n'est pas disponible sur votre système.";
		exit(EXIT_FAILURE);
	}
	*/
	
	glViewport(0, 0, 800, 600);

	// Tout ce qui concerne le Depth Test ( gestion de la profondeur ).
	glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.f, 1.f);
	glEnable(GL_DEPTH_CLAMP);
	
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Création d'un objet Matrix et initialisation des différentes matrices nécessaire au programme.
	PeGL::MatrixStack m_stack(sizeof(glm::mat4)*4);
	PeGL::MatrixStack terrain_stack(sizeof(glm::mat4)*4);
	glm::mat4 projection = glm::perspective(90.f, 800.f/600.f, 0.1f, 100.f);
	glm::mat4 camera = glm::lookAt(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	m_stack.push(camera);
	terrain_stack.push(camera);
	terrain_stack.translate(0, -2, 0);
	
	// Charge la texture.
	PeGL::ObjectManager objmanager;
	PeGL::PeDW suzanne = objmanager.load_PeDW("Ressources/suzanne.obj", "Ressources/udlr.png", "Shader/Light.vert", "Shader/Light.frag");
	PeGL::PeDW terrain = objmanager.load_PeDW("Ressources/terrain.obj", "Ressources/grass.jpg", "Shader/Light.vert", "Shader/Light.frag");
	
	sf::Clock tclock;
	float mouse_x(0);
	float mouse_y(0);
	sf::Clock Time;
	float delta_frame(0);
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
				
				glDeleteShader(terrain.shaders.shader.back());
				glDeleteShader(terrain.shaders.shader.front());
				objmanager.clean_PeDW(terrain);
				
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
		if(current_time > (delta_time+1.0))
		{
			delta_time = current_time;
			
			fps = delta_frame;
			
			cout << fps << endl;
			
			current_time = Time.getElapsedTime().asSeconds();
			delta_frame = 0;
		}

		// On lave la fenêtre :
		glClearColor(13.f/255.f, 37.f/255.f, 78.f/255.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		// On initialise les données nécessaires à l'affichage du Suzanne.
		if(rot_time+0.0001 < Time.getElapsedTime().asSeconds())
		{
			m_stack.rotate((float)(M_PI/180.f)*5, 0, 1, 0);
			terrain_stack.rotate((float)(M_PI/180.f)*5, 0, 1, 0);
			rot_time = Time.getElapsedTime().asSeconds();
		}
		glm::mat4 normm(glm::transpose(projection));
		glm::vec4 light(0, 2, 1, sqrt(5.0));
		
		objmanager.draw_PeDW(suzanne, light, m_stack.top(), projection, normm);
		objmanager.draw_PeDW(terrain, light, terrain_stack.top(), projection, normm);

		// On affiche la fenêtre :
		window.display();
	}
	
	return EXIT_SUCCESS;
}

catch(std::out_of_range& err)
{
	std::cerr << "[ER] Out Of Range : " << err.what() << std::endl;
}
