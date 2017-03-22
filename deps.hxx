#ifndef DEPS_HXX
#define DEPS_HXX

/*!
 * \file deps.hxx
 * \brief Fichier qui contient les #includes commun à tout les fichiers.
 * \author Maël N.
 * \version 0.1a
 * \date 09.07.2012
 */

#define GLM_FORCE_RADIANS 

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdexcept>

#include <vector>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern "C"
{
	#include <stdlib.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <png.h>
	#include <zlib.h>
	#include <SOIL/SOIL.h>
}

#endif

