#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 colOutput;

void main()
{
	colOutput = vec4(1.f);
	colOutput.rgb = color.rgb;
	
	vec4 WCVertex = vec4(1.f);
	WCVertex.xyz = position.xyz;
	gl_Position = WCVertex;
}
