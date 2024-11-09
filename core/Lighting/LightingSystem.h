//written by Reece

#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H
#include <vector>
#include <glm/detail/type_vec4.hpp>

#include "../macroLib/shader.h"
using namespace std;


namespace lightSysten {
    struct DirectionLight {
        glm::vec3 color;
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
    struct PointLight{
        glm::vec3 color;
        glm::vec3 position;

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
    struct SpotLight {
        glm::vec3 color;
        glm::vec3 position;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

        float innerCutOff;
        float outerCutOff;
        glm::vec3 direction;
    };

    class LightingSystem {
    public:
        LightingSystem() = default;
        LightingSystem(macroLib::Shader* shader);
        static void UpdateLighting(glm::vec3 viewPos);
        static void AddDirectionLight(DirectionLight *directionLight);
        static void AddPointLight(PointLight *pointLight);
        static void AddSpotLight(SpotLight *spotLight);
    private:
        static macroLib::Shader *shader;
        static std::vector<DirectionLight*> directionLights;
        static std::vector<PointLight*> pointLights;
        static std::vector<SpotLight*> spotLights;
    };
}




#endif //LIGHTINGSYSTEM_H
