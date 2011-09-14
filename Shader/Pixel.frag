#version 330

uniform float time;
uniform float fragLoop;
out vec4 outputColor;

const vec4 color1 = vec4(0.30, 0.52, 0.54, 1.0);
const vec4 color2 = vec4(0.44, 0.04, 0.06, 1.0);

void main()
{
	float currentTime = mod(time, fragLoop);
	float mixVar = cos(currentTime / fragLoop*22);
    outputColor = mix(color1, color2, mixVar);
}
