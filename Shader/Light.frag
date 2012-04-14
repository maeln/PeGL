#version 330

in vec2 pos_calc;
in float light_calc;

out vec3 outputColor;

uniform vec4 light_dir;
uniform sampler2D cloud_texture;

void main()
{
	vec3 color = vec3(0.2, 0.3, 0.8);
	color *= light_calc;
	color = color + texture2D(cloud_texture, pos_calc).rgb;
    outputColor = sqrt(color);
}
