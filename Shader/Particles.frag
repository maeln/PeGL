#version 330

in vec4 colOutput;

out vec4 outputColor;

void main()
{
	vec4 test = colOutput;
	outputColor = test;
}
