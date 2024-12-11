//written by reece

#include "shader.h"
#include "camera.h"
#include <vector>


using namespace macroLib;
using namespace std;




namespace macroLib {
	class ShaderSystem
	{
	public:
		ShaderSystem() = default;
		ShaderSystem(Camera *newCamera);
		void AddShader(Shader* shader);
		void UpdateShaders(float uTime, const int screenWidth,const int screenHeight);
	private:
		std::vector<Shader*> shaders;
		Camera* camera;
	};
}