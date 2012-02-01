#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 world;
uniform mat4 perspective;
uniform mat3 normalMatrix;
uniform vec4 light_dir;

out vec2 pos_calc;
out float light_calc;

const float specularContrib = 0.3;
const float diffuseContrib = 1.0 - specularContrib;

void main()
{
	vec4 result = world * position;
	
	vec3 tnorm = normalize(normalMatrix * normal);
	vec3 lightVec = normalize(light_dir.xyz - result.xyz);
	vec3 reflectVec = reflect(-lightVec, tnorm);
	vec3 viewVec = normalize(-result.xyz);
	float diffuse = max(dot(lightVec, tnorm), 0.0);
	float spec = 0.0;
	if(diffuse > 0.0)
	{
		spec = max(dot(reflectVec, viewVec), 0.0);
		spec = pow(spec, 16.0);
	}
	
	light_calc = diffuseContrib * diffuse + specularContrib * spec;
	pos_calc = position.xy;
	
	gl_Position = perspective * result;
}
