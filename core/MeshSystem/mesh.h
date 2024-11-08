//written by Reece, nearly copied from mesh class example, so credit to eric winebrenner

#ifndef MESH_H
#define MESH_H


#include "../ew/external/glad.h"
#include "../ew/ewMath/ewMath.h"
#include <glm/gtc/type_ptr.hpp>

using namespace ew;


namespace meshSystem
{
    struct Vertex
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 normal = glm::vec3(0);
        glm::vec2 uv = glm::vec2(0);
        Vertex(){}
        Vertex(const glm::vec3& position,const glm::vec3& normal,const glm::vec2& uv):position(position),normal(normal),uv(uv){}

    };
    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        MeshData() = default;
    };

    class Mesh {
    public:
        Mesh() = default;
        Mesh(const MeshData& meshData);
        void load(const MeshData& meshData);
        void draw() const;
        int getVertexCount() const {return vertexCount;}
        int getIndiciesCount() const {return indicesCount;}

    private:
        bool meshInitialized = false;
        unsigned int vaoRef = 0;
        unsigned int vboRef = 0;
        unsigned int eboRef = 0;
        int vertexCount = 0;
        int indicesCount = 0;
    };
}




#endif //MESH_H
