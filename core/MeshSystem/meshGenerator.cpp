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
    void generateTerrain(std::string filePath,int subDivisions, MeshData *mesh)
    {
        int width, height, nChannels;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nChannels, 0);
        std::cout << std::endl << "start" << std::endl;

        //clear meshdata
        mesh->vertices.clear();
        mesh->indices.clear();

        //allocate needed space
        mesh->vertices.reserve((subDivisions + 1) * (subDivisions + 1));
        mesh->indices.reserve(subDivisions * subDivisions * 6);

        float subDivisionScalingX = (float)(width / subDivisions);
        float subDivisionScalingY = (float)(height / subDivisions);
        //set vertices and uv
        for (unsigned int row = 0; row <= subDivisions; row++)
        {
            for (unsigned int col = 0; col <= subDivisions; col++)
            {
                // retrieve texel for (i,j) tex coord
                
                float xDist = col * subDivisionScalingX;
                float yDist = row * subDivisionScalingY;
                unsigned char* texel = data + ((int)xDist + width * (int)yDist) * nChannels;
                // raw height at coordinate
                unsigned char y = texel[0];
                //std::cout << col << std::endl;

                glm::vec2 uv;
                uv.x = ((float)col / subDivisions / 4);
                uv.y = ((float)row / subDivisions / 4);
                glm::vec3 pos;
                pos.x = ((float)col / subDivisions) * width;
                pos.y = (int)y / 2;
                pos.z = ((float)row / subDivisions) * height;
                glm::vec3 normal = glm::vec3(0, 0, 1);
                mesh->vertices.emplace_back(pos, normal, uv);
            }
        }

        //set indices
        for (size_t row = 0; row < subDivisions; row++)
        {
            for (size_t col = 0; col < subDivisions; col++)
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
    void createSphere(float radius, int subDivisions, MeshData* mesh)
    {
        mesh->vertices.clear();
        mesh->indices.clear();
        mesh->vertices.reserve((subDivisions + 1) * (subDivisions + 1));
        mesh->indices.reserve(subDivisions * subDivisions * 6);

        //SPHERE VERTICES
        // used from example w/ changed variables to match mine
        // https://docs.google.com/document/d/1_IIt49iji72oS7ZHgCI8g23-LoVysRddWR9CosIpSTI/edit?tab=t.0#heading=h.ybqgw7sx35t6
        float thetaStep = (2 * PI) / subDivisions;
        float phiStep = PI / subDivisions;
        for (size_t row = 0; row <= subDivisions; row++)
        {
            float phi = row * phiStep;
            for (size_t col = 0; col <= subDivisions; col++)
            {
                float theta = col * thetaStep;
                glm::vec2 uv;
                uv.x = 1.0 - ((float)col / subDivisions);
                uv.y = 1.0 - ((float)row / subDivisions);
                // the setting of the xyz is done using the vertex struct
                glm::vec3 pos;
                float sineTheta = sinf(theta);
                float cosineTheta = cosf(theta);
                float sinePhi = sinf(phi);
                float cosinePhi = cosf(phi);

                pos.x = cosineTheta * sinePhi;
                pos.y = cosinePhi;
                pos.z = sineTheta * sinePhi;

                // bc pos isn't multiplied bu radius it is already the normal, minor optimization change
                mesh->vertices.emplace_back((pos * radius), pos, uv);


            }
        }

        //SPHERE INDICIES
        // should be identical to the subdivided plane ones
        for (size_t row = 0; row < subDivisions; row++)
        {
            for (size_t col = 0; col < subDivisions; col++)
            {
                unsigned int bl = row * (subDivisions + 1) + col;
                unsigned int br = bl + 1;
                unsigned int tl = bl + subDivisions + 1;
                unsigned int tr = tl + 1;

                // Triangle 1
                mesh->indices.emplace_back(bl);
                mesh->indices.emplace_back(br);
                mesh->indices.emplace_back(tr);
                // Triangle 2
                mesh->indices.emplace_back(tr);
                mesh->indices.emplace_back(tl);
                mesh->indices.emplace_back(bl);
            }
        }
        return;
    }

}