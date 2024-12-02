#include <assimp/Importer.hpp>    
#include <assimp/scene.h>  
#include <assimp/postprocess.h>
#include "../macroLib/shader.h"
#include "../macroLib/texture2D.cpp"
#include <vector>
#include "mesh.h"


using namespace macroLib;
using namespace meshSystem;
using namespace ew;
using namespace std;

using namespace macroLib;

namespace meshSystem
{
	class Model
	{
	public:
		Model(char* path)
		{
			loadModel(path);
		}
		void Draw(Shader &shader);
	private:
		vector<Mesh> meshes;
		string directory;

		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	};
}