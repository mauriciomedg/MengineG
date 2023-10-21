#version 430

in vec4 varyingColor;
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{ 
	//if (gl_FragCoord.x < 295) 
		color = varyingColor;
	//else 
	//	color = vec4(0.0, 0.0, 1.0, 1.0);
}
