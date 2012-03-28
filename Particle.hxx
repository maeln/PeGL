#ifndef PARTICLE_HXX_INCLUDED
#define PARTICLE_HXX_INCLUDED

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <vector>

typedef struct
{
	bool alive;
	float life;
	glm::vec3 couleur;
	glm::vec3 position;
	glm::vec3 direction;
} particules;

class ParticleGenerator
{
	public:
	ParticleGenerator(glm::vec3 position, unsigned int maxpart, unsigned int emitamount, unsigned int lifepart, glm::vec3 initvect, float disp, glm::vec3 c);
	~ParticleGenerator();
	void update();
	void SendVertexArray(float VertexArray[]);
	void SendColorArray(float ColorArray[]);
	glm::vec3 GenerateDirection();
	int ArrPartSize();
	glm::vec3 S_InitVector();
	void U_InitVector(glm::vec3);
	void updateVBO();
	size_t SendOffset();
	
	private:
	glm::vec3 InitPosition;
	unsigned int MaxParticle;
	unsigned int PartEmit;
	unsigned int Partlife;
	glm::vec3 InitVector;
	float Dispertion;
	glm::vec3 color;
	std::vector<particules> ArrPart;
	GLuint addr_vbo;
};

#endif
