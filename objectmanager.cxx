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

PeDW ObjectManager::load_PeDW(std::string meshfile, std::string imgfile, std::string vshadfile, std::string pshadfile)
{
	PeMesh mesh;
	PeTexture texture;
	std::vector<PeShader> shaders;
	PeProgram program;
	
	ShaderLoader shdl;
	shaders.push_back(shdl.loadShader(vshadfile, GL_VERTEX_SHADER));
	shaders.push_back(shdl.loadShader(pshadfile, GL_FRAGMENT_SHADER));
	program = shdl.createProgram(shaders);
	
	MeshLoader mshl;
	mesh = mshl.loadMesh(meshfile);
	
	ImageLoader imgl;
	texture = imgl.loadImage(imgfile, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS, 0);
	
	PeDW dw;
	dw.mesh = mesh;
	dw.texture = texture;
	dw.shaders = program;
	
	return dw;
}

int ObjectManager::draw_PeDW(PeDW obj, glm::vec4 light_position, glm::mat4 model, glm::mat4 world, glm::mat4 perspective, glm::mat4 normal) 
{
	glUseProgram(obj.shaders.addr);
	
	glBindVertexArray(obj.mesh.vao);
	
	glActiveTexture(GL_TEXTURE0 + obj.texture.unit);
	glBindTexture(GL_TEXTURE_2D, obj.texture.addr);
	
	// S'occuper des uniforms ici.
	if(	
			obj.shaders.uniform.find("model") == obj.shaders.uniform.end()
			or obj.shaders.uniform.find("world") == obj.shaders.uniform.end()
			or obj.shaders.uniform.find("perspective") == obj.shaders.uniform.end()
			or obj.shaders.uniform.find("normalMatrix") == obj.shaders.uniform.end()
			or obj.shaders.uniform.find("light_dir") == obj.shaders.uniform.end()
		)
	{
		std::cerr << "[ERR] OBJ : Uniform manquant." << std::endl;
		return 1; // Erreur
	}
	
	glUniformMatrix4fv(obj.shaders.uniform["model"], 1, GL_FALSE, glm::value_ptr(model));
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
}

void ObjectManager::clean_program(PeProgram program)
{
	glDeleteProgram(program.addr);
	program.uniform.clear();
}

void ObjectManager::clean_mesh(PeMesh mesh)
{
	glDeleteBuffers(mesh.vbo.size(), mesh.vbo.data());
	glDeleteVertexArrays(1, &mesh.vao);
}

void ObjectManager::clean_image(PeTexture texture)
{
	
	glDeleteTextures(1, &texture.addr);
}

void ObjectManager::clean_PeDW(PeDW obj)
{
	ObjectManager::clean_image(obj.texture);
	ObjectManager::clean_program(obj.shaders);
	ObjectManager::clean_mesh(obj.mesh);
}

}
