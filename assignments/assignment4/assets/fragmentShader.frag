#version 330 core
out vec4 FragColor;
// in vec4 Color;

in vec2 TexCoord;

uniform sampler2D ourTexture;

uniform float uTime = 1.0;
void main()
{
	//FragColor =  * Color * (sin(uTime) / 2.0f + 0.5f);
	vec4 texColor = texture(ourTexture, TexCoord);

//	if(texColor.a < 0.1)
//        discard;

    FragColor = texColor;

}