//written by Reece, nearly copied from mesh class example, so credit to eric winebrenner

#include "mesh.h"

namespace meshSystem {
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

            //Setup Normal Buffer
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,normal));
            glEnableVertexAttribArray(1);

            //Setup UV Buffer
            glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,uv));
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

        //reset bindings I think
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
    //draws the mesh
    void Mesh::draw() const
    {
        //bind mesh vao
        glBindVertexArray(vaoRef);

        //draw mesh
        glDrawElements(GL_TRIANGLES,indicesCount,GL_UNSIGNED_INT,0);
    }
}