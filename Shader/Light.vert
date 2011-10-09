#version 330

layout(row_major) uniform;
layout (location = 0) in vec4 position;
layout (std140) uniform GlobalMatrices
{
	uniform mat4 world;
	uniform mat4 perspective;
};

//smooth out vec4 light_comp;

uniform vec4 light_dir;

out vec4 pos_calc;
out vec4 light_calc;

void main()
{
	vec4 result = world * position;
	gl_Position = perspective * result;
	
	pos_calc = perspective*position;
	light_calc = normalize(perspective * light_dir);
}
