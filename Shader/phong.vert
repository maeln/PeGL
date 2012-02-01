#version 400

layout (location = 0) in vec4 MCVertex;
layout (location = 1) in vec3 MCNormal;

out vec4 WCVertex;
out vec3 WCNormal;
out vec4 WCLight;

uniform mat4 world;
uniform mat4 perspective;
uniform mat3 normalMatrix;
uniform vec4 light_dir;

void main()
{
	WCVertex = world * MCVertex;
	WCNormal = normalize(normalMatrix * MCNormal);
	WCLight = vec4(1.0, 1.0, 1.0, 1.0) + world * light_dir;
	
	gl_Position = perspective * WCVertex;
}
