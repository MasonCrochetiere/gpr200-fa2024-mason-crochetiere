#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 Color; //Varying
out vec2 TexCoord;

mat4 scale(float x, float y, float z){
	return mat4(
		x, 0.0, 0.0, 0.0,
		0.0, y, 0.0, 0.0,
		0.0, 0.0, z, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 rotateZ(float a){
	mat4 m;
	// left is column, right is row
	m[0][0] = cos(a);
	m[1][0] = -sin(a);
	m[0][1] = sin(a);
	m[1][1] = cos(a);
	m[2][2] = 1;
	m[3][3] = 1;

	return m;
}

mat4 translate(float x, float y, float z){
	mat4 m = mat4(1); // Identity matrix. Not filling everything with 1
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;

	return m;
}

uniform float uTime = 1.0;
uniform mat4 Model;

void main()
{
	vec4 pos = vec4(aPos, 1.0);
	//pos.y += sin(uTime * 1.5 + pos.x) / 2.0;

	//mat4 M = translate(0.3, 0.3, 0.0) * rotateZ(radians(90.0 * uTime)) *  scale(2.0, 1.0, 1.0);

	//pos = M * pos;

	gl_Position = Model * pos; 

	Color = aColor; //Pass-through, just transferring information 
	TexCoord = aTexCoord;
}