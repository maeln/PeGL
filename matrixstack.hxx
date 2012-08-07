// matrixstack.hxx
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


#ifndef MOVEMENTMANAGER_HXX
#define MOVEMENTMANAGER_HXX

#include "deps.hxx"

/*!
 * \file matrixstack.hxx
 * \brief Gère les stack de Matrices et la génération de celle ci.
 * \author Maël N.
 * \version 0.1a
 * \date 09.07.2012
 */

namespace PeGL
{
	class MatrixStack
	{
		public:
			MatrixStack(size_t taille);
			virtual ~MatrixStack();
			
			void pop();
			void push();
			void push(glm::mat4 const& matrix);
			
			void set_matrix(glm::mat4 const& matrix);
			
			void translate(float x, float y, float z);
			void rotate(float angle, float x, float y, float z);
			void scale(float x, float y, float z);
			
			glm::mat4 top();
			glm::mat4 bottom();
			
		private:
			std::vector<glm::mat4> stack;
			unsigned int position;
	};
}

#endif /* MOVEMENTMANAGER_HXX */ 
