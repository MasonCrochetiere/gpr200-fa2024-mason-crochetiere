#version 330 core
out vec4 FragColor;
// in vec4 Color;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;  

uniform sampler2D ourTexture;
uniform float uTime = 1.0;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec4 texColor = texture(ourTexture, TexCoord);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.08;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 

    lightDir   = normalize(lightPos - FragPos);
    viewDir    = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 2); // 32 is the shininess value of our specular highlight
    vec3 specular = lightColor * spec * specularStrength;

    // result
    vec3 result = (ambient + diffuse + specular) * vec3(texColor);
    FragColor = vec4(result, 1.0);

    //FragColor = texColor;

}