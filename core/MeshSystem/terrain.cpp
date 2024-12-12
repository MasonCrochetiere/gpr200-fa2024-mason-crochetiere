//written by Reece

#include "terrain.h"


namespace meshSystem
{
	Terrain::Terrain(macroLib::Shader* newShader,std::string texturePath,std::string heightMapPath,glm::vec3 scale)
	{
		shader = newShader;

		//initialize terrain
		meshSystem::generatePlane(10, 10, 256, &meshData);
		mesh = meshSystem::Mesh(meshData);
		mesh.addTexture(texturePath, "texture_diffuse", *shader);
		renderer = new MeshRenderer(mesh, Transform(), shader);
        renderer->transform.position = glm::vec3(-400.0f , -50.0f, -400.0f);
        renderer->transform.scale = scale;

		 heightMapID = SetupTexture(heightMapPath);
	}
	void Terrain::Draw()
	{
        shader->use();
        glActiveTexture(GL_TEXTURE0 + 1);
        glUniform1i(glGetUniformLocation(shader->ID, "heightmap"), 1);
        glBindTexture(GL_TEXTURE_2D, heightMapID);


        shader->setFloat("height", height);
        shader->setFloat("peaking", peaking);

		renderer->modelAndDraw();
	}
	unsigned int Terrain::SetupTexture(std::string path)
	{
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load" << std::endl;
            stbi_image_free(data);
        }

        return textureID;
	}
}