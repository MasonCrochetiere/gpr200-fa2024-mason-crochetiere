//written by Reece

#include "../macroLib/shader.h"
#include "mesh.h"
#include "meshGenerator.h"
#include "MeshRenderer.h"


using namespace macroLib;

namespace meshSystem
{
	class Terrain
	{
	public:
		Terrain() = default;
		Terrain(macroLib::Shader* newShader,std::string texturePath,std::string heightMapPath,glm::vec3 scale);
		void Draw();
		unsigned int SetupTexture(std::string path);

		float height = 1;
		float peaking = 1;
	private:
		macroLib::Shader* shader;
		meshSystem::MeshData meshData;
		meshSystem::Mesh mesh;
		meshSystem::MeshRenderer* renderer;

		unsigned int heightMapID;
		unsigned int highTextureID;

	};
}