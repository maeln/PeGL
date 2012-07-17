// matrixstack.cxx
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


#include "matrixstack.hxx"

namespace PeGL
{

MatrixStack::MatrixStack(size_t taille) : stack(taille), position(0)
{
	push(glm::mat4(1.0f));
}


MatrixStack::~MatrixStack()
{
	position = 0;
	stack.clear();
}

void MatrixStack::pop()
{
	if(position == 0)
		std::cerr << "[ER] MatrixStack Empty." << std::endl;
	else
		--position;
}
void MatrixStack::push()
{
	push(stack[position]);
}
void MatrixStack::push(glm::mat4 const& matrix)
{
	if((position + 1) == stack.size())
		std::cerr << "[ER] Maximum size of the stack reached." << std::endl;
	else
	{	
		++position;
		stack[position] = matrix;
	}
}

void MatrixStack::set_matrix(glm::mat4 const& matrix)
{
	stack[position] = matrix;
}

void MatrixStack::translate(float x, float y, float z)
{
	glm::vec3 axe(x,y,z);
	stack[position] = glm::translate(stack[position], axe);
}
void MatrixStack::rotate(float angle, float x, float y, float z)
{
	glm::vec3 axe(x,y,z);
	stack[position] = glm::rotate(stack[position], angle, axe);
}
void MatrixStack::scale(float x, float y, float z)
{
	glm::vec3 axe(x,y,z);
	stack[position] = glm::scale(stack[position], axe);
}

glm::mat4 MatrixStack::top()
{
	return stack[position];
}
glm::mat4 MatrixStack::bottom()
{
	return stack[0];
}

}
