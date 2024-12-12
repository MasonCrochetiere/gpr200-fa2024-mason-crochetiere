//written by reece

#version 330 core
#define MAX_DIR_LIGHTS 8
#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 8
out vec4 FragColor;
// in vec4 Color;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;



uniform sampler2D ourTexture;
uniform float uTime = 1.0;
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};
struct DirLight {
    vec3 direction;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
};
struct SpotLight {
    vec3 position;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;

    float constant;
    float linear;
    float quadratic;

    float innerCutOff;
    float outerCutOff;
    vec3 direction;
};
struct PointLight
{
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;

    float ambient;
    float diffuse;
    float specular;
};

uniform float numDirLights;
uniform float numPointLights;
uniform float numSpotLights;
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform Material material;
uniform SpotLight light;

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result;

    //directional light
    for(int i = 0;i < numDirLights;i++)
    {
        result += CalcDirLight(dirLights[i],norm,viewDir);
    }
    //point lights
    for(int i = 0;i < numPointLights;i++)
    {
        result += CalcPointLight(pointLights[i],norm,FragPos,viewDir);
    }
    for(int i = 0;i < numSpotLights;i++)
    {
        result += CalcSpotLight(spotLights[i],norm,FragPos,viewDir);
    }
    FragColor = vec4(result, 1.0f);
}
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //diffuse shading
    float diff = max(dot(normal, lightDir),0.0f);
    //specular shading
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
    //combine results
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.diffuse, TexCoord).rgb;
    return (ambient + diffuse + specular) * light.color;
}
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //diffuse shading
    float diff = max(dot(normal,lightDir),0.0f);
    //specular shading
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir), 0.0f),material.shininess);
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    //combine
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.diffuse, TexCoord).rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * light.color;
}
vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 fragPos,vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //diffuse shading
    float diff = max(dot(normal,lightDir),0.0f);
    //specular shading
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    //spotlight intensity
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon,0.0f,1.0f);
    //combine
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.diffuse, TexCoord).rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular) * light.color;
}