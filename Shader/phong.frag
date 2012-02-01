#version 400

in vec4 WCVertex;
in vec3 WCNormal;
in vec4 WCLight;

out vec3 color_Out;

struct lightSource
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

struct material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

vec4 ambient = vec4(0.4, 0.4, 0.4, 1.f);

lightSource light0 = lightSource(
	WCLight,
	vec4(1.f, 1.f, 1.f, 1.f),
	vec4(0.f, 0.0f, 0.0f, 1.f),
	0.f, 0.5f, 0.f,
	180.f, 0.f,
	vec3(0.f, 0.f, 0.f)
);

material mat0 = material(
	vec4(0.1, 0.1, 0.1, 1.f),
	vec4(1.f, 1.f, 1.f, 1.f),
	vec4(1.f, 1.f, 1.f, 1.f),
	8.0
);

void main()
{
	vec3 gamma = vec3(2.2, 2.2, 2.2);
	vec3 normal = normalize(WCNormal);
	vec3 view = normalize(-WCVertex.xyz);
	vec3 lightDirection;
	float attenuation;
	
	if(light0.position.w == 0.0) //Directional light.
	{
		attenuation = 1.f;
		lightDirection = normalize(light0.position.xyz);
	}
	else
	{
		vec3 lightView = light0.position.xyz - WCVertex.xyz;
		float distance = length(lightView);
		lightDirection = normalize(lightView);
		attenuation = 1.0 / (light0.constantAttenuation + light0.linearAttenuation * distance + light0.quadraticAttenuation * distance * distance);
		if(light0.spotCutoff <= 90.f)
		{
			float clampedCosine = max(0.f, dot(-lightDirection, light0.spotDirection));
			if(clampedCosine < cos(radians(light0.spotCutoff)))
			{
				attenuation = 0.f;
			}
			else
			{
				attenuation = attenuation * pow(clampedCosine, light0.spotExponent);
			}
		}
	}
	vec3 ambientLighting = ambient.rgb * mat0.ambient.rgb;
	vec3 diffuseReflection = attenuation * light0.diffuse.rgb * mat0.diffuse.rgb * max(0.f, dot(normal, lightDirection));
	vec3 specularReflection;
	if(dot(normal, lightDirection) < 0.f)
	{
		specularReflection = vec3(0.f, 0.f, 0.f);
	}
	else
	{
		specularReflection = attenuation * light0.specular.rgb * mat0.specular.rgb * pow(max(0.f, dot(reflect(-lightDirection.xyz, normal), view)), mat0.shininess);
	}
	color_Out = pow(ambientLighting + diffuseReflection + specularReflection, gamma);
}

	
