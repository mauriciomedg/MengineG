#version 410 core

uniform UniformData
{
	float scale;
};
	
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;

layout (location = 0) out vec3 outColor;

void main(void)
{ 
	gl_Position.xyz = position * scale;
	//gl_Position.y *= 1.5;
	gl_Position.w = 1.0;
	
	outColor = inColor;
}