//Author: Conor Byrne
#include "skybox.h"

namespace skybox
{
	Skybox::Skybox(Shader* newShader)
	{
		shader = newShader;
		std::vector<std::string> faces
		{
			"assets/skybox/newRight.png",
			"assets/skybox/newLeft.png",
			"assets/skybox/newTop.png",
			"assets/skybox/newBottom.png",
			"assets/skybox/newFront.png",
			"assets/skybox/newBack.png"
		};
		unsigned int cubemapTexture = loadCubemap(faces);

		meshSystem::createSphere(8000.0f, 256, &sphereMeshData);
		sphereMesh = meshSystem::Mesh(sphereMeshData);
		sphereRenderer = new meshSystem::MeshRenderer(sphereMesh, shader);
	}
	unsigned int Skybox::loadCubemap(std::vector<std::string> faces)
	{
		stbi_set_flip_vertically_on_load(false);
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}
	void Skybox::Render(float timeValue)
	{
		shader->use();

		glm::mat4 sphereSpin = glm::mat4(1.0f);
		sphereSpin = glm::rotate(sphereSpin, timeValue * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		shader->setMat4("model", sphereSpin);
		shader->setFloat("rotationSpeed", rotationSpeed);
		sphereRenderer->modelAndDraw();
	}
}