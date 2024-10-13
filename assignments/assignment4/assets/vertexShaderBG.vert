#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 Color; //Varying
out vec2 TexCoord;

uniform float uTime = 1.0;
void main()
{
	vec3 pos = aPos * 3;
//	pos.y += sin(uTime * 1.5 + pos.x) / 2.0;

	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0); 
	Color = aColor; //Pass-through, just transferring information 
	TexCoord = aTexCoord;
	//* (sin(uTime * 1.5 + pos.x) * 20);
}