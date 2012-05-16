// imageloader.cxx
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


#include "imageloader.hxx"

namespace PeGL
{

ImageLoader::ImageLoader()
{
	tex_nb = 0;
}


ImageLoader::~ImageLoader()
{
	
}

PeTexture ImageLoader::loadImage(std::string filename)
{
	GLuint oglimg = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	PeTexture tex2d;
	
	tex2d.id = tex_nb;
	tex2d.addr = oglimg;
	
	++tex_nb;
	
	return tex2d;
}

}
