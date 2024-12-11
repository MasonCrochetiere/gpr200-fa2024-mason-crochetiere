#include "../macroLib/shader.h"
#include <vector>
#include "../MeshSystem/mesh.h"
#include "../MeshSystem/meshGenerator.h"
#include "../MeshSystem/meshRenderer.h"

using namespace macroLib;
using namespace meshSystem;

namespace skybox
{
	class Skybox
	{
	public:
		Skybox() = default;
		Skybox(Shader* newShader);
		unsigned int loadCubemap(std::vector<std::string> faces);
		void Render(float timeValue);
	private:
		unsigned int cubemapTexture;
		Shader* shader;
		meshSystem::MeshData sphereMeshData;
		meshSystem::Mesh sphereMesh;
		meshSystem::MeshRenderer* sphereRenderer;
	};
}