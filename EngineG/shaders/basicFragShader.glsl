#version 430 core

//layout (location = 0) in vec3 vertOutColor;
in vec2 tc; 
layout (location = 0) out vec4 outColor;
layout (binding=0) uniform sampler2D samp;

void main(void)
{ 
	//outColor = vec4(vertOutColor, 1);
	outColor = texture(samp, tc);
}