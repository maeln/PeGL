#version 330

layout (location = 0) in vec2 position;
uniform sampler2D fbo_tex;
out vec2 tex_coord;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	tex_coord = (position + 1.0) / 2.0;
}
