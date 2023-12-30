#version 430
uniform float offset;
uniform float halfSize;

layout (location = 0) in vec3 position;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 varyingColor;

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad)
{ 
	mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
	0.0, 1.0, 0.0, 0.0,
	-sin(rad), 0.0, cos(rad), 0.0,
	0.0, 0.0, 0.0, 1.0 );
	return yrot;
}
// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad)
{ 
	mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
	sin(rad), cos(rad), 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0 );
	return zrot;
}
// builds and returns a scale matrix
mat4 buildScale(float x, float y, float z)
{ 
	mat4 scale = mat4(x, 0.0, 0.0, 0.0,
	0.0, y, 0.0, 0.0,
	0.0, 0.0, z, 0.0,
	0.0, 0.0, 0.0, 1.0 );
	return scale;
}
	
mat4 rotate = buildRotateZ(0.75);

void main(void)
{ 
	//gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
	//if (gl_VertexID == 0) 
	
	//gl_Position = rotate * vec4(position, 1.0) + vec4( offset, 0.0, 0.0, 1.0);
	
	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
	varyingColor = (vec4(position, 1.0) / halfSize) * 0.5 + (vec4(0.5, 0.5, 0.5, 0.5));
}