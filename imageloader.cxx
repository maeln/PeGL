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

#define PNGSIGSIZE 8

namespace PeGL
{

ImageLoader::ImageLoader()
{
	
}


ImageLoader::~ImageLoader()
{
	
}

texture ImageLoader::loadImage(std::string filename)
{
	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
	if(!file) { std::cerr << "Erreur: Impossible d'ouvrir le fichier : " << filename << std::endl; exit(EXIT_FAILURE); }
	png_byte pngsig[PNGSIGSIZE];
	file.read((char*)pngsig, PNGSIGSIZE);
	if(!file.good()) { std::cerr << "Erreur lors de la lecture du fichier : " << filename << std::endl; exit(EXIT_FAILURE); }
	int is_png(png_sig_cmp(pngsig, 0, PNGSIGSIZE));
	if(is_png != 0) { std::cerr << "Erreur le fichier : " << filename << "n'est pas un fichier PNG." << std::endl; exit(EXIT_FAILURE); };
	
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngPtr) { std::cerr << "Erreur : Impossible de créer la stucture de lecture PNG" << std::endl; exit(EXIT_FAILURE); }
	
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr) { std::cerr << "Erreur : impossible de créer la structure d'information PNG." << std::endl; png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0); exit(EXIT_FAILURE); }
	
	png_bytep *rowPtrs = NULL;
	char *pixelData = NULL;
	
	if(setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);
		if(rowPtrs != NULL) delete [] rowPtrs;
		if(pixelData != NULL) delete [] pixelData;
		std::cerr << "Erreur lors de la lecture du fichier png." << std::endl;
		exit(EXIT_FAILURE);
	}
	
	
}

}
