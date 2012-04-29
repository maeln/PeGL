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

////////////////////////////////////////////////////////////////////////
//////////////// NOTE //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Les fonctions pour charger des textures/meshs/etc ... devraient    //
// renvoyer des pointeurs pour permettre au manager d'objet de les    //
// détruires facilement, ainsi qu'avoir une meilleur gestion          //
// d'erreur ( pointeur = NULL -> erreur durent le chargement ).       //
////////////////////////////////////////////////////////////////////////

namespace PeGL
{
	class ObjectManager
	{
		public:
			ObjectManager();
			virtual ~ObjectManager();
		
		private:
			/* add your private declarations */
	};
}

#endif /* OBJECTMANAGER_HXX */ 
