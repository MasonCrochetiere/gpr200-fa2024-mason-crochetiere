#version 330 core
out vec4 FragColor;
in vec4 Color;

in vec2 TexCoord;

uniform sampler2D ourTexture;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float uTime = 1.0;
void main()
{
	//FragColor =  * Color * (sin(uTime) / 2.0f + 0.5f);
	

	vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.6); 
	texColor = texColor * Color
	//* (sin(uTime) / 2.0f + 0.5f);
	// vec4(texColor.x, texColor.y, texColor.z, 1.0f);

//	if(texColor.a < 0.1)
//        discard;

    FragColor = vec4(texColor.x, texColor.y, texColor.z, 1.0f);

}