// objectmanager.hxx
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


#ifndef OBJECTMANAGER_HXX
#define OBJECTMANAGER_HXX

/*
 * Cette classe s'occupe de stocker l'adresse des mesh, vbo, textures ...
 * de les redistribuer si besoin et surtout
 * de les décharger proprement.
*/

class ObjectManager
{
	public:
		ObjectManager();
		virtual ~ObjectManager();
		void add_texture(GLuint id);
		void add_vbo(GLuint id);
		void add_vao(GLuint id);
		void add_mesh(
	
	private:
		/* add your private declarations */
};

#endif /* OBJECTMANAGER_HXX */ 
