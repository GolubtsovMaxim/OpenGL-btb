#version 430
layout (location = 0) in vec3 position;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;

uniform float tf;

out vec4 varyingColor;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void)
{ 
	float i = gl_InstanceID + tf;
	float a = sin(0.35 * i) * 8.0;
	float b = sin(0.52 * i) * 8.0;
	float c = sin(0.7 * i) * 8.0;

	mat4 localRotX = buildRotateX(1.75*i);
	mat4 localRotY = buildRotateY(1.75*i);
	mat4 localRotZ = buildRotateZ(1.75*i);

	mat4 localTranslate = buildTranslate(a,b,c);

	mat4 new_m_matrix = localTranslate * localRotX * localRotY * localRotZ;
	mat4 mv_matrix = v_matrix * new_m_matrix;

	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
	varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

mat4 buildTranslate(float x, float y, float z)
{ 
	mat4 translate = mat4(1.0, 0.0, 0.0, 0.0,
						  0.0, 1.0, 0.0, 0.0,
						  0.0, 0.0, 1.0, 0.0,
						  x, y, z, 1.0 );
	return translate;
}

mat4 buildRotateX(float rad)
{ 
	mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
					 0.0, cos(rad), -sin(rad), 0.0,
					 0.0, sin(rad), cos(rad), 0.0,
				     0.0, 0.0, 0.0, 1.0 );
	return xrot;
}

mat4 buildRotateY(float rad)
{ 
	mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
					 0.0, 1.0, 0.0, 0.0,
					 -sin(rad), 0.0, cos(rad), 0.0,
					 0.0, 0.0, 0.0, 1.0 );
	return yrot;
}

mat4 buildRotateZ(float rad)
{ 
	mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
					 sin(rad), cos(rad), 0.0, 0.0,
				     0.0, 0.0, 1.0, 0.0,
					 0.0, 0.0, 0.0, 1.0 );
	return zrot;
}