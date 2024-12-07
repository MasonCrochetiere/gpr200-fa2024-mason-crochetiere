#include "texture2D.h"
namespace macroLib {

    Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode)
    {
        //filterMode: GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, etc
        //wrapMode; GL_REPEAT, GL_CLAMP_TO_BORDER, etc
		// NOT IMPLEMENTED YET

		// TEXTURE GENERATION START

		//unsigned int m_id;
		glGenTextures(1, &m_id); // 1, so this unsigned int stores the location of one texture
		glBindTexture(GL_TEXTURE_2D, m_id); // bind this as the texture 2d to work on

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);

		//printf("Loading texture of path ");
		//printf(filePath);

		//int m_width, m_height, m_nrChannels;
		unsigned char* data = stbi_load(filePath, &m_width, &m_height, &m_nrChannels, 0);// loading these ints with data from that file
		if (data)
		{
			if (m_nrChannels == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else if (m_nrChannels = 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				printf("Loaded image does not have 3 or 4 channels! Trying to load as RGBA anyway...");
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			
			// crazy list of parameters: object type target, mipmap level, value format, width, height, legacy 0, color format, texture datatype, location of data
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data); // don't need the space going forward

		// TEXTURE GENERATION COMPLETE
    }

    Texture2D::~Texture2D()
    {

    }

    void Texture2D::Bind(unsigned int slot) //Bind to a specific texture unit
    {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, m_id);	
    }
}