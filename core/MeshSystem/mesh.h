//written by Reece, nearly copied from mesh class example, so credit to eric winebrenner

#ifndef MESH_H
#define MESH_H


#include "../ew/external/glad.h"
#include "../ew/ewMath/ewMath.h"
#include <glm/gtc/type_ptr.hpp>
#include "../macroLib/texture2D.h"
#include <string>
#include <iostream>
#include "../macroLib/shader.h"
#include <assimp/Importer.hpp>   


using namespace macroLib;

using namespace ew;


namespace meshSystem
{
    struct Vertex
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec2 uv = glm::vec2(0);
        glm::vec3 normal = glm::vec3(0);
        Vertex(){}
        Vertex(const glm::vec3& position,const glm::vec3& normal,const glm::vec2& uv):position(position),normal(normal),uv(uv){}

    };
    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };
    struct MeshData
    {
        MeshData(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
        }
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        MeshData() = default;
    };

    class Mesh {
    public:
        Mesh() = default;
        Mesh(const MeshData& meshData);
        void load(const MeshData& meshData);
        void draw(macroLib::Shader &shader) const;
        void addTexture(std::string filePath,std::string type,macroLib::Shader &shader);
        int getVertexCount() const {return vertexCount;}
        int getIndiciesCount() const {return indicesCount;}

    private:
        std::vector<Texture> textures;
        bool meshInitialized = false;
        unsigned int vaoRef = 0;
        unsigned int vboRef = 0;
        unsigned int eboRef = 0;
        int vertexCount = 0;
        int indicesCount = 0;
    };
}




#endif //MESH_H
