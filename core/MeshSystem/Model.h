//written by Reece

#include <assimp/Importer.hpp>    
#include <assimp/scene.h>  
#include <assimp/postprocess.h>
#include "../macroLib/shader.h"
#include "../macroLib/texture2D.h"
#include <vector>
#include "mesh.h"
#include "MeshRenderer.h";


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
			transform = Transform();
			loadModel(path);
		}
		void Draw(Shader &shader);
		meshSystem::Transform transform;
	private:
		vector<Texture> textures_loaded;
		vector<Mesh> meshes;
		string directory;

		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	};
}