//written by Reece

#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H
#include <vector>
#include <glm/detail/type_vec4.hpp>

#include "../macroLib/shader.h"
using namespace std;


namespace lightSystem {
    struct DirectionLight {
        glm::vec3 color = glm::vec3(1.0f);
        glm::vec3 direction = glm::vec3(0.0f,0.0f,1.0f);
        float ambient = 0.1f;
        float diffuse = 0.5f;
        float specular = 0.5f;
    };
    struct PointLight{
        glm::vec3 color = glm::vec3(1.0f);
        glm::vec3 position = glm::vec3(0.0f);

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

        float ambient = 0.1f;
        float diffuse = 0.5f;
        float specular = 0.5f;
    };
    struct SpotLight {
        glm::vec3 color = glm::vec3(1.0f);
        glm::vec3 position =- glm::vec3(0.0f);
        float ambient = 0.1f;
        float diffuse = 0.5f;
        float specular = 0.5f;

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

        float innerCutOff = 10.0f;
        float outerCutOff = 20.0f;
        glm::vec3 direction = glm::vec3(0.0f,0.0f,1.0f);
    };

    class LightingSystem {
    public:
        LightingSystem() = default;
        LightingSystem(macroLib::Shader* shader);
        void UpdateLighting(glm::vec3 viewPos);
        void AddDirectionLight(DirectionLight *directionLight);
        void AddPointLight(PointLight *pointLight);
        void AddSpotLight(SpotLight *spotLight);
    private:
        macroLib::Shader *shader;
        std::vector<DirectionLight*> directionLights;
        std::vector<PointLight*> pointLights;
        std::vector<SpotLight*> spotLights;
    };
}




#endif //LIGHTINGSYSTEM_H
