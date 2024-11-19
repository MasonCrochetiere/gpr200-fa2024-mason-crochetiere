//written by Reece

#include "Model.h"

namespace meshSystem {
	void Model::Draw(Shader& shader)
	{
		for(unsigned int i = 0;i < meshes.size();i++) {
			meshes[i].draw();
		}
	}
	void Model::loadModel(string path)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "Assimp Error: " << endl;
			return;
		}
		directory = path.substr(0,path.find_last_of('/'));
		processNode(scene->mRootNode,scene);
	}
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for(unsigned int i = 0;i < node->mNumMeshes;i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh,scene));
		}
		for(unsigned int i = 0;i < node->mNumChildren;i++)
		{
			processNode(node->mChildren[i],scene);
		}
	}
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		vector <Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture2D> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//process vertices
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.uv = vec;
			}
			else
			{
				vertex.uv = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}
		//process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		}
		return Mesh(MeshData(vertices,indices,textures));
	}
	vector<Texture2D> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		return vector<Texture2D>();
	}
}