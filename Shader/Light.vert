#version 330

layout(row_major) uniform;
layout (location = 0) in vec4 position;
layout (std140) uniform GlobalMatrices
{
	uniform mat4 world;
	uniform mat4 perspective;
};

smooth out vec4 light_comp;

uniform vec4 light_dir;
uniform mat4 modelview;

void main()
{
	vec4 result = modelview * position;
	result = world * result;
	gl_Position = perspective * result;
	
	//vec4 light_dir = vec4(1, 1, 1, 1);
	vec4 pos;
	pos = perspective * light_dir;
	pos = normalize(pos);
	float comp = dot(pos, (perspective*(modelview*position)));
	comp = clamp(comp, 0, 1);
	light_comp = vec4(comp, comp, comp, 1.0);
}
