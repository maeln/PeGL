/*
 * camera.hxx
 * 
 * Copyright 2014 Maël Naccache <maeln@chise>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#ifndef CAMERA_HXX
#define CAMERA_HXX

#include "deps.hxx"
#include <cmath>

namespace PeGL
{
	class Camera
	{
		public:
			Camera(glm::vec3 position, glm::vec3 lookAt, float mouseSpeed, float moveSpeed);
			virtual ~Camera();
			glm::mat4 getView();
			
			void move(glm::vec3 direction);
			void rotate(glm::vec2 mouse);
		
		private:
			glm::vec3 m_position;
			glm::vec3 m_lookAt;
			glm::vec3 m_up;
			glm::vec2 m_angles;
			
			float m_mouseSpeed;
			float m_moveSpeed;
	};
}

#endif /* CAMERA_HXX */ 
