#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec4 aColor;
layout (location = 1) in vec2 aTexCoord;

out vec4 Color; //Varying
out vec2 TexCoord;

uniform float uTime = 1.0;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 pos = aPos;
	//pos.y += sin(uTime * 1.5 + pos.x) / 2.0;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
//	Color = aColor; //Pass-through, just transferring information 
	TexCoord = aTexCoord;
}