#version 430
out vec4 color;

uniform mat4 MV_matrix;
uniform mat4 Proj_matrix;

void main(void)
{ 
	color = vec4(1.0, 0.0, 0.0, 1.0); 
}