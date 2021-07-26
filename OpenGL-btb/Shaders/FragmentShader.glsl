#version 430
out vec4 color;

in vec4 varyingColor;

uniform mat4 MV_matrix;
uniform mat4 Proj_matrix;

void main(void)
{ 
	color = varyingColor; 
}