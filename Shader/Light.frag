#version 330

in vec4 pos_calc;
in vec4 light_calc;

out vec4 outputColor;

uniform vec4 light_dir;

void main()
{
	float comp = dot(light_calc, pos_calc);
	comp = clamp(comp, 0, 1);
	vec4 light_comp = vec4(comp, comp, comp, 1.0);
	
    outputColor = light_comp;
}
