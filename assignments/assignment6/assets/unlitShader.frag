#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;  

uniform sampler2D ourTexture;
uniform vec3 color;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord);
    FragColor = vec4(color, 1.0f);
}