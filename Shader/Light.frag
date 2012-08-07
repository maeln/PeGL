#version 330

smooth in vec2 pos_calc;
in vec3 tnorm;
in vec4 result;

out vec4 outputColor;

uniform vec4 light_dir;
uniform mat4 world;

uniform mat3 normalMatrix;

uniform sampler2D cloud_texture;

vec4 celshad(vec4 color)
{
	vec4 light = light_dir * world;
	
	vec3 lightVec = normalize(light.xyz - result.xyz);
	vec3 reflectVec = reflect(-lightVec, tnorm);
	vec3 viewVec = normalize(-result.xyz);
	float diffuse = max(dot(lightVec, tnorm), 0.0);
	
	float light_calc = diffuse;
	
	if(light_calc < 0.2)
	{
		light_calc = 0.2;
	}
	else if(light_calc < 0.5)
	{
		light_calc = 0.5;
	}
	else if(light_calc < 0.8)
	{
		light_calc = 0.8;
	}
	else
	{
		light_calc = 1.0;
	}
	
	// Il faudrait adoucir légèrement le résultat.
	
	color.rgb *= light_calc;
	return color;
}

void main()
{
	
	vec4 color;
	color = texture2D(cloud_texture, pos_calc).rgba;
	color = celshad(color);
	
	
    outputColor = sqrt(color);
}
