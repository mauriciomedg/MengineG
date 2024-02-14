#version 410 core

layout (location = 0) in vec3 vertOutColor;
layout (location = 0) out vec4 outColor;

void main(void)
{ 
	outColor = vec4(vertOutColor, 1);
}