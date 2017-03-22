// objectmanager.hxx
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


#ifndef OBJECTMANAGER_HXX
#define OBJECTMANAGER_HXX

#include "shaderloader.hxx"
#include "meshloader.hxx"
#include "imageloader.hxx"

/*!
 * \file objectmanager.hxx
 * \brief Gère et créer différent objets et les affiches.
 * \author Maël N.
 * \version 0.1a
 * \date 09.07.2012
 */

/*!
 * \namespace PeGL
 * 
 * Composante graphique du moteur PeGA, gérant l'affichage avec OpenGL 3.3.
 * 
 */
namespace PeGL
{
	/*!
	 * \struct PeDW
	 * \brief Objet affichable.
	 * 
	 * PeDW est une variable contenant un Mesh, une texture et un program qui pourront être affichés.
	 * Cela permet de simplifier la gestion d'objet basique.
	 */
	struct PeDW
	{
		PeMesh mesh;
		PeTexture texture;
		PeProgram shaders; 
	};
	
	/*!
	 * \class ObjectManager
	 * \brief gère les objets et les affiches.
	 */
	class ObjectManager
	{
		public:
			/*!
			 * \brief Constructeur de ObjectManager, n'a pas besoin de paramètres.
			 */
			ObjectManager();
			/*!
			 * \brief Destructeur de ObjectManager.
			 */
			virtual ~ObjectManager();
			
			/*!
			 * \brief Créer un objets PeDW.
			 * 
			 * TO-DO: Accepter plusieurs pixel shaders et rendre la texture optionnel.
			 * 
			 * \param meshfile : Chemin vers le fichier mesh.
			 * \param imgfile : Chemin vers le fichier image qui servira de texture pour le mesh. NB: devrait être optionnel ( objet sans texture ).
			 * \param vshadfile : Chemin vers un fichier vertex shader.
			 * \param pshadfile : Chemin vers un fichier pixel shader.
			 * \return Retourne un objet PeDW prêt à être affiché.
			 */
			PeDW load_PeDW(std::string meshfile, std::string imgfile, std::string vshadfile, std::string pshadfile);
			
			/*!
			 * \brief Affiche un objet PeDW.
			 * 
			 * TO_DO: Meilleur gestion de la lumière ( plusieurs lumières, différentes couleurs / puissances ).
			 * 
			 * \param obj : L'objet PeDW à afficher.
			 * \param light_position : Position de la lumière.
			 * \param model : Matrice de tranformation.
			 * \param world : Matrice «Camera». 
			 * \param perspective : Matrice de perspective ( frustum ).
			 * \param normal : Matrice normal à la matrice de perspective.
			 * \return Retourne 0 si aucune erreur ne c'est produite, 1 sinon.
			 */
			int draw_PeDW(PeDW obj, glm::vec4 light_position, glm::mat4 model, glm::mat4 world, glm::mat4 perspective, glm::mat4 normal);
			
			/*!
			 * \brief Supprime un objet PeDW.
			 * \param obj : L'objet PeDW à supprimer.
			 */
			void clean_PeDW(PeDW obj);
			
			/*!
			 * \brief Supprime un program.
			 * 
			 * Supprime un program de la mémoire vidéo, mais ne supprime pas les shaders qui le compose.
			 * 
			 * \param prog : Program à supprimer.
			 */
			void clean_program(PeProgram prog);
			
			/*!
			 * \brief Supprime un shader de la mémoire vidéo.
			 * \param shad : Shader à supprimer.
			 */
			void clean_shader(PeShader shad);
			
			/*!
			 * \brief Supprime un objet PeMesh ( VBO/VAO ).
			 * \param mesh : Objet PeMesh a supprimer.
			 */
			void clean_mesh(PeMesh mesh);
			
			/*!
			 * \brief Supprime une texture de la mémoire vidéo.
			 * \param texture : Texture à supprimer.
			 */
			void clean_image(PeTexture texture);
		
		private:
			/* add your private declarations */
	};
}

#endif /* OBJECTMANAGER_HXX */ 
