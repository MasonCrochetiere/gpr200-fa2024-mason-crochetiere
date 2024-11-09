//Written by Reece

#include "LightingSystem.h"

namespace lightSysten {
    LightingSystem::LightingSystem(macroLib::Shader *shader) {
        this->shader = shader;
    }
    void LightingSystem::UpdateLighting(glm::vec3 viewPos)
    {
        shader->use();
        shader->setVec3("viewPos", viewPos);


        //Direction Lights
        string directionStringStart = "dirLights[";
        for(int i = 0;i < directionLights.size();i++)
        {
            string directionString = directionStringStart + std::to_string(i);
            glm::vec3 diffuseColor = directionLights[i]->color * glm::vec3(0.5f); // decrease the influence
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
            shader->setVec3(directionString + "].ambient", ambientColor);
            shader->setVec3(directionString + "].diffuse", diffuseColor);
            shader->setVec3(directionString + "].specular", directionLights[i]->specular);
            shader->setVec3(directionString + "].direction", directionLights[i]->direction);
        }


        //Point Lights
        string pointStringStart = "pointLights[";
        for(int i = 0;i < pointLights.size();i++)
        {
            string pointString = pointStringStart + std::to_string(i);
            glm::vec3 diffuseColor = pointLights[i]->color * glm::vec3(0.5f); // decrease the influence
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
            shader->setVec3(pointString + "].ambient", ambientColor);
            shader->setVec3(pointString + "].diffuse", diffuseColor);
            shader->setVec3(pointString + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setFloat(pointString + "].constant",  pointLights[i]->constant);
            shader->setFloat(pointString + "].linear",    pointLights[i]->linear);
            shader->setFloat(pointString + "].quadratic", pointLights[i]->quadratic);
        }


        //Spot Lights
        string spotStringStart = "spotLights[";
        for(int i = 0;i < spotLights.size();i++)
        {
            string spotString = spotStringStart + std::to_string(i);
            glm::vec3 diffuseColor = spotLights[i]->color * glm::vec3(0.5f); // decrease the influence
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
            shader->setVec3(spotString + "].ambient", ambientColor);
            shader->setVec3(spotString + "].diffuse", diffuseColor);
            shader->setVec3(spotString + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setFloat(spotString + "].constant",  spotLights[i]->constant);
            shader->setFloat(spotString + "].linear",    spotLights[i]->linear);
            shader->setFloat(spotString + "].quadratic", spotLights[i]->quadratic);
            shader->setFloat("].innerCutOff", glm::cos(glm::radians(spotLights[i]->innerCutOff)));
            shader->setFloat("].outerCutOff", glm::cos(glm::radians(spotLights[i]->outerCutOff)));
            shader->setVec3("].direction", spotLights[i]->direction);
        }
    }
    void LightingSystem::AddDirectionLight(DirectionLight *directionLight)
    {
        directionLights.push_back(directionLight);
    }
    void LightingSystem::AddPointLight(PointLight *pointLight)
    {
        pointLights.push_back(pointLight);
    }
    void LightingSystem::AddSpotLight(SpotLight *spotLight)
    {
        spotLights.push_back(spotLight);
    }
}