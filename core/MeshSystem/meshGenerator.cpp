//written by Reece but nearly copied from procGen example file, so credit to eric winebrenner

#include "meshGenerator.h"
#include <iostream>
namespace meshSystem
{

    //creates a cube face given a direction and size
    static void createCubeFace(const glm::vec3& normal,float size, MeshData* mesh)
    {
        //starts vertex at end of vertex list
        unsigned int startVertex = mesh->vertices.size();

        //setup axes
        glm::vec3 a = glm::vec3(normal.z,normal.x,normal.y);
        glm::vec3 b = glm::cross(normal,a);

        //sets vertex positions and uv of face
        for (int i = 0; i < 4; i++)
        {
            int col = i % 2;
            int row = i / 2;
            glm::vec3 pos = normal * size * 0.5f;
            pos -= (a + b) * size * 0.5f;
            pos += (a * (float)col + b * (float)row) * size;
            const glm::vec2 uv = glm::vec2(col, row);
            mesh->vertices.emplace_back(pos,normal,uv);
        }

        //set indices
        mesh->indices.emplace_back(startVertex);
        mesh->indices.emplace_back(startVertex + 1);
        mesh->indices.emplace_back(startVertex + 3);
        mesh->indices.emplace_back(startVertex + 3);
        mesh->indices.emplace_back(startVertex + 2);
        mesh->indices.emplace_back(startVertex);
    }

    //creates a cube of a given size
    void generateCube(float size,MeshData* mesh)
    {
        mesh->vertices.reserve(24); //6 x 4 vertices
        mesh->indices.reserve(36); //6 x 6 indices
        createCubeFace(glm::vec3{ +0.0f,+0.0f,+1.0f }, size, mesh); //Front
        createCubeFace(glm::vec3{ +1.0f,+0.0f,+0.0f }, size, mesh); //Right
        createCubeFace(glm::vec3{ +0.0f,+1.0f,+0.0f }, size, mesh); //Top
        createCubeFace(glm::vec3{ -1.0f,+0.0f,+0.0f }, size, mesh); //Left
        createCubeFace(glm::vec3{ +0.0f,-1.0f,+0.0f }, size, mesh); //Bottom
        createCubeFace(glm::vec3{ +0.0f,+0.0f,-1.0f }, size, mesh); //Back
    }

    //creates a mesh of a given width/height and a number of subDivisions
    void generatePlane(float width, float height, int subDivisions, MeshData *mesh)
    {
        //clear meshdata
        mesh->vertices.clear();
        mesh->indices.clear();

        //allocate needed space
        mesh->vertices.reserve((subDivisions + 1) * (subDivisions + 1));
        mesh->indices.reserve(subDivisions * subDivisions * 6);

        //set vertices and uv
        for(size_t row = 0;row <= subDivisions;row++)
        {
            for(size_t col = 0;col <= subDivisions;col++)
            {
                glm::vec2 uv;
                uv.x = ((float)col / subDivisions);
                uv.y = ((float)row / subDivisions);
                glm::vec3 pos;
                pos.x = uv.x * width;
                pos.y = uv.y * height;
                pos.z = 0;
                glm::vec3 normal = glm::vec3(0,0,1);
                mesh->vertices.emplace_back(pos,normal,uv);
            }
        }

        //set indices
        for(size_t row = 0;row < subDivisions;row++)
        {
            for(size_t col = 0;col < subDivisions;col++)
            {
                unsigned int bottomLeft = row * (subDivisions + 1) + col;
                unsigned int bottomRight = bottomLeft + 1;
                unsigned int topLeft = bottomLeft + subDivisions + 1;
                unsigned int topRight = topLeft + 1;

                //triangle 1
                mesh->indices.emplace_back(bottomLeft);
                mesh->indices.emplace_back(bottomRight);
                mesh->indices.emplace_back(topRight);
                //triangle 2
                mesh->indices.emplace_back(topRight);
                mesh->indices.emplace_back(topLeft);
                mesh->indices.emplace_back(bottomLeft);
            }
        }
    }
    void generateTerrain(std::string filePath,MeshData *mesh)
    {
        int width, height, nChannels;
        unsigned char *data = stbi_load("assets/iceland_heightmap.png",&width,&height,&nChannels,0);

        // vertex generation
        float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data

        mesh->vertices.reserve(width * height * 3);

        for (unsigned int i = 0; i < height; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                // retrieve texel for (i,j) tex coord
                unsigned char* texel = data + (j + width * i) * nChannels;
                // raw height at coordinate
                unsigned char y = texel[0];

                // vertex
               // mesh->vertices.emplace_back(-height / 2.0f + i);        // v.x
                //mesh->vertices.emplace_back((int)y * yScale - yShift); // v.y
               // mesh->vertices.emplace_back(-width / 2.0f + j);        // v.z
            }
        }

    }

}