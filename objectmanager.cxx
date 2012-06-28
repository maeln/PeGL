// objectmanager.cxx
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


#include "objectmanager.hxx"

namespace PeGL
{

ObjectManager::ObjectManager()
{
	
}


ObjectManager::~ObjectManager()
{
	
}

int ObjectManager::draw_PeDW(PeDW obj, glm::vec4 light_position, glm::mat4 world, glm::mat4 perspective, glm::mat4 normal) 
// devrait prendre en argument le nom des uniforms a utiliser et leurs valeurs ( via tableaux ? ).
// Ou alors : Filer la map des uniform AVANT et renvoyer un tableaux associatif "uniform"->"valeur" à la fonction ? ( + différent type a gérer = fait chier).
// 1 map par type de variable a envoyer ? ( pb: y'en a des tonnes : vec2,vec3,vec4,mat3, ... )
// WAIT : on utilise les même uniforms pour les objets "normaux", donc y'a juste à les mettres en arguments
{
	glUseProgram(obj.shaders.addr);
	
	glBindVertexArray(obj.mesh.vao);
	
	glActiveTexture(GL_TEXTURE0 + obj.texture.id);
	glBindTexture(GL_TEXTURE_2D, obj.texture.addr);
	
	// S'occuper des uniforms ici.
	if 	( 	obj.shaders.uniform.find("world") == obj.shaders.uniform.end()
			or obj.shaders.uniform.find("perspective") == obj.shaders.uniform.end()
			or obj.shaders.uniform.find("normalMatrix") == obj.shaders.uniform.end()
			or obj.shaders.uniform.find("light_dir") == obj.shaders.uniform.end()
		)
	{
		std::cerr << "[ER] Uniform manquant." << std::endl;
		return 1; // Erreur
	}
	
	glUniformMatrix4fv(obj.shaders.uniform["world"], 1, GL_FALSE, glm::value_ptr(world));
	glUniformMatrix4fv(obj.shaders.uniform["perspective"], 1, GL_FALSE, glm::value_ptr(perspective));
	glUniformMatrix3fv(obj.shaders.uniform["normalMatrix"], 1, GL_FALSE, glm::value_ptr(normal));
	glUniform4fv(obj.shaders.uniform["light_dir"], 1, glm::value_ptr(light_position));
	
	glDrawElements(GL_TRIANGLES, obj.mesh.elements_size, GL_UNSIGNED_SHORT, 0);
	
	glBindVertexArray(0);
	glUseProgram(0);
	
	return 0; // O -> pas d'erreurs.
}

void ObjectManager::clean_shader(PeShader shader)
{
	glDeleteShader(shader.addr);
	shader.uniform.clear();
	
	delete(&shader);
}

void ObjectManager::clean_program(PeProgram program)
{
	glDeleteProgram(program.addr);
	program.uniform.clear();
	
	delete(&program);
}

void ObjectManager::clean_mesh(PeMesh mesh)
{
	glDeleteBuffers(mesh.vbo.size(), mesh.vbo.get_allocator().allocate(mesh.vbo.size()));
	glDeleteVertexArrays(1, &mesh.vao);
	
	delete(&mesh);
}

void ObjectManager::clean_image(PeTexture texture)
{
	glDeleteTextures(1, &texture.addr);
	
	delete(&texture);
}

void ObjectManager::clean_PeDW(PeDW obj)
{
	ObjectManager::clean_image(obj.texture);
	ObjectManager::clean_program(obj.shaders);
	ObjectManager::clean_mesh(obj.mesh);
	
	delete(&obj);
}

}
