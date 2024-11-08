//
// Created by Reece on 11/7/2024.
//

#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "../ew/external/glad.h"
#include "../ew/ewMath/ewMath.h"
#include <glm/gtc/type_ptr.hpp>
#include "../macroLib/shader.h"
#include "mesh.h"
using namespace macroLib;
using namespace meshSystem;

namespace meshSystem {
    struct Transform
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 rotation = glm::vec3(0);
        glm::vec3 scale = glm::vec3(1);
    };

    class MeshRenderer {
    public:
        MeshRenderer();
        MeshRenderer(meshSystem::Mesh newMesh, macroLib::Shader *newShader);
        MeshRenderer(meshSystem::Mesh newMesh,Transform newTransform, macroLib::Shader *newShader);
        void modelAndDraw();
        Transform transform;
    private:
        macroLib::Shader *meshShader;
        meshSystem::Mesh mesh;

    };
}




#endif //MESHRENDERER_H
