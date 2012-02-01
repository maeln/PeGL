#version 400

layout(row_major) uniform;
layout (location = 0) in vec4 position;
layout (std140) uniform GlobalMatrices
{
	uniform mat4 world;
	uniform mat4 perspective;
};

uniform float time;
uniform float vertLoop;
uniform mat4 modelview;

void main()
{
	//Modifie la position du vertex en fonction du temps. Désactivé pour le moment.
	float timeScale = 3.14159f * 2.f / vertLoop;
	float currentTime = mod(time, vertLoop);
	vec4 totOffset = vec4(cos(currentTime * timeScale)*0.5, sin(currentTime * timeScale)*0.5, 0.f, 0.f);
	///////////////////////////////
	
	vec4 result = modelview * position; //Applique les transformations au vertex.
	result = world * result;
	gl_Position = perspective * result; //Applique la matrice de projection au vertex.
}
