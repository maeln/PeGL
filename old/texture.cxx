// texture.cxx
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


#include "texture.hxx"

using namespace std;

Texture::Texture()
{
	texture_position.clear();
}


Texture::~Texture()
{
	
}

sf::Image* Texture::LoadToMemory(string filename)
{
	sf::Image *texture(NULL);
	texture = new sf::Image;
	texture->loadFromFile(filename);
	
	return texture;
}

GLuint Texture::UploadToVidMem(sf::Image *image)
{
	unsigned int width = (unsigned int)image->getSize().x;
	unsigned int height = (unsigned int)image->getSize().y;
	const unsigned int resolution = width * height;
	GLubyte img_array[resolution*4]; // On utilise du RGBA, donc 1 pixel = 4 cases de données.
	unsigned int n(0);
	
	for(unsigned int y(0); y < height; ++y)
	{
		for(unsigned int x(0); x < width; ++x)
		{
			img_array[n*4] 		= (GLubyte)image->getPixel(x,y).r;
			img_array[n*4+1]	= (GLubyte)image->getPixel(x,y).g;
			img_array[n*4+2]	= (GLubyte)image->getPixel(x,y).b;
			img_array[n*4+3]	= (GLubyte)image->getPixel(x,y).a;
			++n;
		}
	}
	
	delete image;
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_array);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	texture_position[texture] = texture_position.size()-1;
	
	return texture;
}

int Texture::texture_nb(GLuint texture_id)
{
	return texture_position[texture_id];
}
