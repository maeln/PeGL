// imageloader.hxx
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


#ifndef IMAGELOADER_HXX
#define IMAGELOADER_HXX

#include "deps.hxx"

/*!
 * \file imageloader.hxx
 * \brief S'occupe du chargement des textures.
 * \author Maël N.
 * \version 0.1a
 * \date 09.07.2012
 */

namespace PeGL
{
	/*!
	 * \struct PeTexture
	 * \brief Variable contenant ID et l'adresse d'une texture.
	 */
	struct PeTexture
	{
		GLuint unit; /*!< La texture Unit a lier à la texture. */
		GLuint addr; /*!< Adresse de la texture. */
	};
	
	/*!
	 * \class ImageLoader
	 * \brief Utilise SOIL pour charger des images et les uploader en mémoire vidéo.
	 */
	class ImageLoader
	{
		public:
			/*!
			 * \brief Constructeur de la classe ImageLoader.
			 */
			ImageLoader();
			/*!
			 * \brief Destructeur de la classe ImageLoader.
			 */
			virtual ~ImageLoader();
			/*!
			 * \brief Charge une image dans la mémoire vidéo.
			 * \param filename : Chemin de l'image à charger.
			 * \return Retourne un objet PeTexture utilisable.
			 */
			PeTexture loadImage(std::string filename, unsigned int flags, GLuint af_unit);
			
			/*!
			 * \brief libère un ID anciennement utilisé.
			 * \param id : ID à libérer.
			 */
			void free_id(int fid);
		
		private:
			
	};
}

#endif /* IMAGELOADER_HXX */ 
