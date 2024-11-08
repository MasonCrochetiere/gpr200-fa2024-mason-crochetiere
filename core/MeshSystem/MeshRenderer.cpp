//
// Created by Reece on 11/7/2024.
//

#include "MeshRenderer.h"

namespace meshSystem {
    MeshRenderer::MeshRenderer(meshSystem::Mesh newMesh,macroLib::Shader *newShader)
    {
        mesh = newMesh;
        meshShader = newShader;
    }
    MeshRenderer::MeshRenderer(meshSystem::Mesh newMesh,Transform newTransform,macroLib::Shader *newShader)
    {
        mesh = newMesh;
        transform = newTransform;
        meshShader = newShader;
    }

    //sets all the required vertex shader data, then draws the mesh (requires view and projection to be set, as well as the correct shader active)
    void MeshRenderer::modelAndDraw() {
        //setup model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::rotate(model, 0.0f, transform.rotation);
        model = glm::scale(model, transform.scale);

        meshShader->setMat4("model", model);

        mesh.draw();
    }

}