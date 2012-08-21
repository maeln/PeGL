// postprocessing.hxx
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


#ifndef POSTPROCESSING_HXX
#define POSTPROCESSING_HXX

#include "deps.hxx"
#include "shaderloader.hxx"

namespace PeGL
{
	class PostProcessing
	{
		public:
			PostProcessing(GLsizei width, GLsizei height, std::string vertexs_path, std::string frags_path);
			virtual ~PostProcessing();
			
			void resize_fbo(GLsizei nwidth, GLsizei nheight);
			void bindfb();
			void unbindfb();
			void drawfb();
		
		private:
			GLuint fbo;
			GLuint fbo_texture;
			GLuint depth_buffer;
			GLuint fbo_vertices;
			PeProgram postproc;
	};
}

#endif /* POSTPROCESSING_HXX */ 
