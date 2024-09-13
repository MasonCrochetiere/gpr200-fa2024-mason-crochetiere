#version 330 core
out vec4 FragColor;
in vec4 Color;

uniform float uTime = 1.0;
void main()
{
	FragColor = Color * (sin(uTime) / 2.0f + 0.5f); 

}