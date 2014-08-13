/*
 * camera.cxx
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

#include "camera.hxx"

namespace PeGL
{

Camera::Camera(glm::vec3 position, glm::vec3 lookAt, float mouseSpeed, float moveSpeed) : m_up(0.f,1.f,0.f)
{
	m_position = position;
	m_lookAt = lookAt;
	m_angles = glm::vec2(0.f, 0.f);
	m_mouseSpeed = mouseSpeed;
	m_moveSpeed = moveSpeed;
}


Camera::~Camera()
{
	
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(m_position, m_lookAt + m_position, m_up);
}

void Camera::move(glm::vec3 direction)
{
	glm::vec3 forward = glm::vec3(sinf(m_angles.x), 0, cosf(m_angles.x));
	glm::vec3 right = glm::vec3(-forward.z, 0, forward.x);
	
	m_position += right * m_moveSpeed * direction.x;
	m_position += forward * m_moveSpeed * direction.z;
	m_position.y +=  m_moveSpeed * direction.y;
}

void Camera::rotate(glm::vec2 mouse)
{
	m_angles.x += mouse.x * m_mouseSpeed;
	m_angles.y += mouse.y * m_mouseSpeed;
	
	m_lookAt.x = sinf(m_angles.x) * cosf(m_angles.y);
	m_lookAt.y = sinf(m_angles.y);
	m_lookAt.z = cosf(m_angles.x) * cosf(m_angles.y);
}

}
