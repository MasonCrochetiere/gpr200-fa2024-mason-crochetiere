#version 330 core
out vec4 FragColor;
// in vec4 Color;

in vec3 TexCoords;
in vec3 Normal;
in vec3 FragPos;  
in vec3 textureDir;

uniform sampler2D ourTexture;
uniform float uTime;
uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, TexCoords);
}