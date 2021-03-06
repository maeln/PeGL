// meshloader.hxx
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


#ifndef MESHLOADER_HXX
#define MESHLOADER_HXX

#include "deps.hxx"

/*!
 * \file meshloader.hxx
 * \brief S'occupe du chargement des Meshs.
 * \author Maël N.
 * \version 0.1a
 * \date 09.07.2012
 */

namespace PeGL
{
	/*!
	 * \struct PeMesh
	 * \brief Variable contenant l'adresse des VBO, du VAO et la taille du tableau d'éléments.
	 */
	struct PeMesh
	{
		std::vector<GLuint> vbo;
		GLuint vao;
		size_t elements_size;
	};
	
	/*!
	 * \class MeshLoader
	 * \brief Gère le chargement de fichier OBJ et la création des VBOs/VAOs d'un mesh.
	 */
	class MeshLoader
	{
		public:
			/*!
			 * \brief Constructeur de la classe MeshLoader.
			 */
			MeshLoader();
			/*!
			 * \brief Destructeur de la classe Meshloader.
			 */
			virtual ~MeshLoader();
			/*!
			 * \brief Charge un mesh et créer les VBO/VAO nécessaire.
			 * \param filename : Chemin vers le fichier obj à charger.
			 * \return Retourne un objet PeMesh utilisable.
			 */
			PeMesh loadMesh(std::string filename);
			/*!
			 * \brief Transforme un tableau de glm::vec3 en tableau C de float.
			 * \param vec3 : Tableau dynamique ( Vector ) de glm::vec3.
			 * \param output : Tableau de Float qui contiendra les données.
			 */
			void vec3float(std::vector<glm::vec3> vec3, float* output);
		
		private:
			/* add your private declarations */
	};
}

#endif /* MESHLOADER_HXX */ 
