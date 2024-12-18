//Author: Conor Byrne
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform float uTime;
uniform float rotationSpeed;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat4 rotationY(float angle)
{
    return mat4(
        cos(angle), 0, sin(angle), 0,
        0, 1, 0, 0,
        -sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1
    );
}
mat4 rotationX(float angle)
{
    return mat4(
        1, 0, 0, 0,
        0, cos(angle), -sin(angle), 0,
        0, sin(angle), cos(angle), 0,
        0, 0, 0, 1
    );
}
void main()
{
    mat4 rotatedView = view * rotationX(-0.1 * rotationSpeed * uTime) * rotationY(-0.1 * rotationSpeed * uTime);
    
    TexCoords = aPos;
    gl_Position = projection * rotatedView * vec4(aPos, 1.0);

	//TexCoords = aPos;
	//gl_Position = projection * view * vec4(aPos, 1.0);
}