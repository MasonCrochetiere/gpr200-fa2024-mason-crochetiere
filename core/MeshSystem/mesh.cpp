//written by Reece, nearly copied from mesh class example, so credit to eric winebrenner

#include "mesh.h"

namespace meshSystem {
    unsigned int TextureFromFile(std::string filename,bool gamma);

    Mesh::Mesh(const MeshData& meshData)
    {
        load(meshData);
    }

    //loads a set of vertices into the gpu as a mesh
    void Mesh::load(const MeshData& meshData)
    {
        if(!meshInitialized)
        {
            //allocate space for data
            glGenVertexArrays(1,&vaoRef);
            glBindVertexArray(vaoRef);

            glGenBuffers(1,&vboRef);
            glBindBuffer(GL_ARRAY_BUFFER,vboRef);

            glGenBuffers(1,&eboRef);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboRef);

            //Setup Position Buffer
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,position));
            glEnableVertexAttribArray(0);

            //Setup UV Buffer
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,uv));
            glEnableVertexAttribArray(1);

            //Setup Normal Buffer
            glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,normal));
            glEnableVertexAttribArray(2);


            meshInitialized = true;
        }

        //bind vao, vbo, and ebo
        glBindVertexArray(vaoRef);
        glBindBuffer(GL_ARRAY_BUFFER,vboRef);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboRef);

        //inputs vertices data if it exists
        if(meshData.vertices.size() > 0) {
            glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * meshData.vertices.size(),meshData.vertices.data(),GL_STATIC_DRAW);
        }
        //inputs indices data if it exists
        if (meshData.indices.size() > 0) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.indices.size(), meshData.indices.data(), GL_STATIC_DRAW);
        }
        //set num vertices and indices
        vertexCount = meshData.vertices.size();
        indicesCount = meshData.indices.size();
        textures = meshData.textures;

        //reset bindings I think
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
    //draws the mesh
    void Mesh::draw(macroLib::Shader &shader) const
    {
        shader.use();
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        //bind mesh vao
        glBindVertexArray(vaoRef);

        //draw mesh
        glDrawElements(GL_TRIANGLES,indicesCount,GL_UNSIGNED_INT,NULL);

        glActiveTexture(GL_TEXTURE0);
    }


    //adds a new texture to mes
    //types are:
    //texture_diffuse
    //texture_specular
    //texture_normal
    //texture_height
    void Mesh::addTexture(std::string filePath,std::string type,macroLib::Shader &shader)
    {
        shader.use();
        Texture texture;
        texture.id = TextureFromFile(filePath,false);
        texture.type = type;
        texture.path = filePath;
        textures.push_back(texture);
    }
    unsigned int TextureFromFile(std::string filename,bool gamma)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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