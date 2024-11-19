#include "Model.h";

namespace meshSystem {
	void Model::Draw(Shader& shader)
	{

	}
	void Model::loadModel(string path)
	{

	}
	void Model::processNode(aiNode* node, const aiScene* scene)
	{

	}
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		return Mesh();
	}
	vector<Texture2D> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		return vector<Texture2D>();
	}
}