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
#include "camera.hxx"
#include "postprocessing.hxx"

#include <GLFW/glfw3.h>
#include <locale>

using namespace std;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{		
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// Intialise tout les paramêtres OpenGL
void InitGL(glm::vec2 winSize)
{
	glViewport(0, 0, winSize.x, winSize.y);
	
	// Face Culling.
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

	// Tout ce qui concerne le Depth Test ( gestion de la profondeur ).
	glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.f, 1.f);
	glEnable(GL_DEPTH_CLAMP);
	
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char **argv)
{
	// Locale pour les caractéres français.
	setlocale(LC_ALL, "");
	
	// Taille de la fenêtre
	glm::vec2 winSize(1280,720);
	float FoV = 90.f;
	float near = 0.1f;
	float far = 100.f;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	// Créer la fenêtre.
	GLFWwindow* glWindow;
	if(!glfwInit())
	{
		cerr << "[ERR] MAIN : Unable to initailize GLFW." << endl;
		return -1;
	}
	
	glWindow = glfwCreateWindow(winSize.x, winSize.y, "PeGL, FPS : 0", NULL, NULL);
	if(!glWindow)
	{
		cerr << "[ERR] MAIN : Unable to create window." << endl;
		return -1;
	}
	
	// Contexte OpenGL
	glfwMakeContextCurrent(glWindow);
	
	// On initialise Glew :
	GLenum err = glewInit();
	if (GLEW_OK != err) // Vérifie que Glew est bien initialisé, sinon, renvoi l'erreur.
	{
		cerr << "[ERR] MAIN : Glew Error : " << glewGetErrorString(err) << endl;
		return -1;
	}

	cout << "[INF] MAIN : Glew " << glewGetString(GLEW_VERSION) << " en cours d'utilisation." << endl;
	
	/*
	if (!glewIsSupported("GL_VERSION_3_3")) // On vérifie que OpenGL 3.3 est bien disponible sinon on ferme le programme.
	{
		cerr << "[ERR] MAIN : OpenGL 3.3 n'est pas disponible sur votre système.";
		return -1;
	}
	*/
	
	InitGL(winSize);

	// Création d'un objet Matrix et initialisation des différentes matrices nécessaire au programme.
	PeGL::MatrixStack m_stack;
	PeGL::MatrixStack terrain_stack;
	
	PeGL::Camera camera(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 0.001, 0.8);
	glm::mat4 projection = glm::perspective(FoV, static_cast<float>(winSize.x)/static_cast<float>(winSize.y), near, far);
	
	terrain_stack.translate(0, -2, 0);
	
	// Charge la texture.
	PeGL::ObjectManager objmanager;
	PeGL::PeDW suzanne = objmanager.load_PeDW("Ressources/suzanne.obj", "Ressources/udlr.png", "Shader/Light.vert", "Shader/Light.frag");
	PeGL::PeDW terrain = objmanager.load_PeDW("Ressources/terrain.obj", "Ressources/grass.png", "Shader/Light.vert", "Shader/Light.frag");
	
	// Framebuffer et Post Processing.
	PeGL::PostProcessing post(winSize.x, winSize.y, "Shader/post.vert", "Shader/post.frag");
	
	// Initialisation des variable de la main loop.
	glm::vec2 previousMousePosition(winSize.x/2,winSize.y/2);
	
	float delta_frame(0);
	float delta_time(glfwGetTime());
	float current_time(0);
	float fps(0);
	float frameTime(0.010);
	
	bool vsync(false);
	bool vPressed(false);
	
	cout << "[DBG] MAIN : Max texture units : " << GL_MAX_TEXTURE_UNITS << endl;
	
	// Key callback
	glfwSetKeyCallback(glWindow, key_callback);
	
	glfwSetCursorPos(glWindow, winSize.x/2, winSize.y/2);
	
	while(!glfwWindowShouldClose(glWindow))
	{
		float timeAtStart(glfwGetTime());
		// Viewport Handling
		float ratio;
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(glWindow, &fbWidth, &fbHeight);
		ratio = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
		
		glViewport(0, 0, fbWidth, fbHeight);
		projection = glm::perspective(FoV, ratio, near, far);
		post.resize_fbo((GLsizei)fbWidth, (GLsizei)fbHeight);
		
		//Compteur de fps.
		++delta_frame;
		current_time = glfwGetTime();
		if(current_time > (delta_time+1.0))
		{
			delta_time = current_time;
			
			fps = delta_frame;
			
			std::stringstream ss;
			ss << "PeGL. FPS: " << fps << " | FrameTime : " << frameTime;
			glfwSetWindowTitle(glWindow, ss.str().c_str());
			
			current_time = glfwGetTime();
			delta_frame = 0;
		}
		
		// Mouse handling
		double mouseX, mouseY;
		glfwGetCursorPos(glWindow, &mouseX, &mouseY);
		
		glm::vec2 dmouse;
		dmouse.x = mouseX - previousMousePosition.x;
		dmouse.y = mouseY - previousMousePosition.y;
				
		previousMousePosition.x = fbWidth/2;
		previousMousePosition.y = fbHeight/2;				
			
		camera.rotate(-dmouse);
		
		glfwSetCursorPos(glWindow, fbWidth/2, fbHeight/2);
		
		// Keyboard
		// Qwerty / Azerty hack
		if(glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_PRESS) // Q
		{
			camera.move(glm::vec3(-1.f*frameTime,0.f,0.f));
		}
				
		if(glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_PRESS) // D
		{
			camera.move(glm::vec3(1.f*frameTime,0.f,0.f));
		}
		
		if(glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS) // S
		{
			camera.move(glm::vec3(0.f,0.f,-1.f*frameTime));
		}
		
		if(glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS) // Z
		{
			camera.move(glm::vec3(0.f,0.f,1.f*frameTime));
		}
		
		if(glfwGetKey(glWindow, GLFW_KEY_SPACE) == GLFW_PRESS) // Space
		{
			camera.move(glm::vec3(0.f,1.f*frameTime,0.f));
		}
		
		if(glfwGetKey(glWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // LShift
		{
			camera.move(glm::vec3(0.f,-1.f*frameTime,0.f));
		}
		
		// Vsync key
		if(glfwGetKey(glWindow, GLFW_KEY_V) == GLFW_PRESS) // V
		{
			vPressed = true;
		}
		
		if(vPressed)
		{
			if(glfwGetKey(glWindow, GLFW_KEY_V) == GLFW_RELEASE)
			{
				vsync = !vsync;
				if(vsync)
				{
					cout << "[INF] MAIN : Vsync On." << endl;
				}
				else
				{
					cout << "[INF] MAIN : Vsync Off." << endl;
				}
				
				vPressed = false;
			}
		}
		
		if(vsync)
		{
			glfwSwapInterval(1);
		} 
		else 
		{
			glfwSwapInterval(0);
		}
		
		// On lave la fenêtre :
		post.bindfb();
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// On initialise les données nécessaires à l'affichage.
		glm::mat4 normm(glm::transpose(projection));
		glm::vec4 light(1, 1, 1, 1);
		
		objmanager.draw_PeDW(suzanne, light, m_stack.top(), camera.getView(), projection, normm);
		objmanager.draw_PeDW(terrain, light, terrain_stack.top(), camera.getView(), projection, normm);
		post.unbindfb();
		
		post.drawfb();
		
		// On affiche la fenêtre :
		glfwSwapBuffers(glWindow);
		glfwPollEvents();
		
		float timeAtEnd(glfwGetTime());
		frameTime = timeAtEnd - timeAtStart;
	}
	
	glDeleteShader(suzanne.shaders.shader.back());
	glDeleteShader(suzanne.shaders.shader.front());
	objmanager.clean_PeDW(suzanne);
				
	glDeleteShader(terrain.shaders.shader.back());
	glDeleteShader(terrain.shaders.shader.front());
	objmanager.clean_PeDW(terrain);
	
	glfwDestroyWindow(glWindow);
	
	glfwTerminate();
	
	return 0;
}
