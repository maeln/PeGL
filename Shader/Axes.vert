#version 400

layout(row_major) uniform;
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (std140) uniform GlobalMatrices
{
	uniform mat4 world;
	uniform mat4 perspective;
};

smooth out vec4 theColor;

void main()
{
	vec4 res = world * position;
	gl_Position = perspective * res;
	theColor = color;
}
