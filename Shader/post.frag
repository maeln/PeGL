#version 330

in vec2 tex_coord;
uniform sampler2D fbo_tex;
uniform sampler2D depth;

out vec4 color;

void main()
{
	color = vec4(texture2D(fbo_tex, tex_coord ).rgb, 1.0);
	/*
	// Based on : http://www.openjejej.com/2012/02/12/detection-de-contours-filtre-de-sobel-en-glsl/
	ivec2 size = textureSize(fbo_tex, 0);
	float dx = 1.0/size.x;
	float dy = 1.0/size.y;
	
	vec2 center = tex_coord;
	
	vec4 a = texture(fbo_tex, center + vec2(-dx, -dy));
	vec4 b = texture(fbo_tex, center + vec2(-dx,   0));
	vec4 c = texture(fbo_tex, center + vec2(-dx,  dy));
	vec4 d = texture(fbo_tex, center + vec2(0,   -dy));
	vec4 e = texture(fbo_tex, center + vec2(0,    dy));
	vec4 f = texture(fbo_tex, center + vec2(+dx, -dy));
	vec4 g = texture(fbo_tex, center + vec2(+dx,   0));
	vec4 h = texture(fbo_tex, center + vec2(+dx,  dy));
	
	// X
	// |1 0 -1|
	// |2 0 -2|
	// |1 0 -1|
	// Y
	// |1   2  1|
	// |0   0  0|
	// |-1 -2 -1|
	
	vec4 colx = 1*a+2*b+1*c+(-1)*f+(-2)*g+(-1)*h;
	vec4 coly = 1*a+(-1)*c+2*d+(-2)*e+1*f+(-1)*h;
	vec4 colf = sqrt(colx * colx + coly * coly);
	float intensity = sqrt(colf.r * colf.g * colf.b);
	
	if(intensity < 0.1)
		color = texture2D(fbo_tex, tex_coord);
	else
		color = vec4(0.f, 0.f, 0.f, 1.f);
		*/
		
		//color = vec4(texture2D( depth, tex_coord ).rgb, 1.0);
}
