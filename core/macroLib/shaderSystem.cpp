//written by Reece

#include "shaderSystem.h";




namespace macroLib
{
	ShaderSystem::ShaderSystem(Camera* newCamera)
	{
		camera = newCamera;
	}
	void ShaderSystem::AddShader(Shader* shader)
	{
		shaders.push_back(shader);
	}
	void ShaderSystem::UpdateShaders(float uTime, const int screenWidth,const int screenHeight)
	{
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera->getFOV()), (float)screenWidth / (float)screenHeight, camera->NEAR_PLANE, camera->FAR_PLANE);
		view = glm::lookAt(camera->getCameraPos(), camera->getCameraPos() + camera->getCameraFront(), camera->getCameraUp());
		for (Shader* shader : shaders)
		{
			shader->use();
			shader->setMat4("projection", projection);
			shader->setFloat("uTime", uTime);
			shader->setMat4("view", view);
		}
	}
}