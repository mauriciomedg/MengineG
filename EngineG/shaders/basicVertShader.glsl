#version 410 core

uniform UniformData
{
	mat4 world;
	mat4 vMat;
	mat4 pMat;
};
	
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

layout (location = 0) out vec3 vertOutColor;

void main(void)
{
	gl_Position =  pMat * vMat * world * vec4(position, 1);
	vertOutColor = vec3(texcoord.x, texcoord.y, 0);
}