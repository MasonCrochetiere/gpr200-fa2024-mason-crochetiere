//Written by Reece

#include "LightingSystem.h"

namespace lightSystem {
    LightingSystem::LightingSystem(macroLib::Shader* newShader) {
        shaders.push_back(newShader);
        directionLights = std::vector<DirectionLight*>();
        pointLights = std::vector<PointLight*>();
        spotLights = std::vector<SpotLight*>();
        SetupMaterialUniforms(newShader);
    }
    void LightingSystem::UpdateLighting(glm::vec3 viewPos)
    {
        for (int i = 0; i < shaders.size(); i++)
        {
            macroLib::Shader* shader = shaders[i];
            shader->use();
            shader->setVec3("viewPos", viewPos);
            shader->setFloat("numDirLights", directionLights.size());
            shader->setFloat("numPointLights", pointLights.size());
            shader->setFloat("numSpotLights", spotLights.size());

            //Direction Lights
            string directionStringStart = "dirLights[";
            for (int i = 0; i < directionLights.size(); i++)
            {
                string directionString = directionStringStart + std::to_string(i);
                shader->setVec3(directionString + "].color", directionLights[i]->color);
                shader->setFloat(directionString + "].ambient", directionLights[i]->ambient);
                shader->setFloat(directionString + "].diffuse", directionLights[i]->diffuse);
                shader->setFloat(directionString + "].specular", directionLights[i]->specular);
                shader->setVec3(directionString + "].direction", directionLights[i]->direction);
            }


            //Point Lights
            string pointStringStart = "pointLights[";
            for (int i = 0; i < pointLights.size(); i++)
            {
                if (pointLights[i] != nullptr)
                {
                    string pointString = pointStringStart + std::to_string(i);
                    shader->setVec3(pointString + "].color", pointLights[i]->color);
                    shader->setVec3(pointString + "].position", pointLights[i]->position);
                    shader->setFloat(pointString + "].ambient", pointLights[i]->ambient);
                    shader->setFloat(pointString + "].diffuse", pointLights[i]->diffuse);
                    shader->setFloat(pointString + "].specular", pointLights[i]->specular);
                    shader->setFloat(pointString + "].constant", pointLights[i]->constant);
                    shader->setFloat(pointString + "].linear", pointLights[i]->linear);
                    shader->setFloat(pointString + "].quadratic", pointLights[i]->quadratic);
                }
            }


            //Spot Lights
            string spotStringStart = "spotLights[";
            for (int i = 0; i < spotLights.size(); i++)
            {
                string spotString = spotStringStart + std::to_string(i);
                shader->setVec3(spotString + "].position", spotLights[i]->position);
                shader->setVec3(spotString + "].color", spotLights[i]->color);
                shader->setFloat(spotString + "].ambient", spotLights[i]->ambient);
                shader->setFloat(spotString + "].diffuse", spotLights[i]->diffuse);
                shader->setFloat(spotString + "].specular", spotLights[i]->specular);
                shader->setFloat(spotString + "].constant", spotLights[i]->constant);
                shader->setFloat(spotString + "].linear", spotLights[i]->linear);
                shader->setFloat(spotString + "].quadratic", spotLights[i]->quadratic);
                shader->setFloat(spotString + "].innerCutOff", glm::cos(glm::radians(spotLights[i]->innerCutOff)));
                shader->setFloat(spotString + "].outerCutOff", glm::cos(glm::radians(spotLights[i]->outerCutOff)));
                shader->setVec3(spotString + "].direction", spotLights[i]->direction);
            }
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
    void LightingSystem::RemovePointLight(PointLight* pointLight)
    {
        std::vector<PointLight*>::const_iterator position = std::find(pointLights.begin(), pointLights.end(), pointLight);
        if (position != pointLights.end())
        {
            pointLights.erase(position);
        }
    }
    void LightingSystem::AddSpotLight(SpotLight *spotLight)
    {
        spotLights.push_back(spotLight);
    }
    void LightingSystem::AddShader(macroLib::Shader* newShader)
    {
        shaders.push_back(newShader);
        SetupMaterialUniforms(newShader);
    }
    void LightingSystem::SetupMaterialUniforms(macroLib::Shader* shader)
    {
        shader->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        shader->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shader->setFloat("material.shininess", 32.0f);
    }
}