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

uniform sampler2D heightmap;

uniform sampler2D highTexture;
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

vec3 highColor = vec3(0.8,0.8,0.8);
vec3 midColor = vec3(1,1,1);
vec3 lowColor = vec3(0.5,1,0.5);
vec3 lowestColor = vec3(0.3,1,0.3);

in float aHeight;
in float aPeaking;

void main()
{
    float height = pow(texture(heightmap,TexCoord).r,aPeaking) * aHeight;
    

    if(height < 0.3f)
    {
        highColor *= 0;
    }
    if(height < 0.05f)
    {
        midColor *= 0;
    }
    if(height < 0.001)
    {
        lowColor *= 0;
    }


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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

        ambient = light.ambient * (highColor + midColor + lowColor + lowestColor);
        diffuse = light.diffuse * diff *  (highColor + midColor + lowColor + lowestColor);
        specular = light.specular * spec *  (highColor + midColor + lowColor + lowestColor);
   

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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
        ambient = light.ambient *  (highColor + midColor + lowColor + lowestColor);
        diffuse = light.diffuse * diff *  (highColor + midColor + lowColor + lowestColor);
        specular = light.specular * spec * (highColor + midColor + lowColor + lowestColor);
    
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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
        ambient = light.ambient * (highColor + midColor + lowColor + lowestColor);
        diffuse = light.diffuse * diff * (highColor + midColor + lowColor + lowestColor);
        specular = light.specular * spec * (highColor + midColor + lowColor + lowestColor);
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular) * light.color;
}