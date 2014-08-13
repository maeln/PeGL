#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 world;
uniform mat4 perspective;

uniform mat3 normalMatrix;
uniform vec4 light_dir;

smooth out vec2 pos_calc;
out vec3 tnorm;
out vec4 result;

void main()
{
	result = model * world * position;

	tnorm = normalize(normalMatrix * normal);

	pos_calc = position.xy;

	gl_Position = perspective * result;
}
