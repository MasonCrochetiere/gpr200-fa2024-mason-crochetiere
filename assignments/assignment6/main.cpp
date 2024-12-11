#include <stdio.h>
#include <math.h>
#include <iostream>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <macroLib/shader.h>
#include <macroLib/texture2D.h>
#include <macroLib/camera.h>
#include <macroLib/shaderSystem.h>

#include "Skybox/skybox.h"

#include <MeshSystem/mesh.h>
#include <MeshSystem/meshGenerator.h>
#include <MeshSystem/MeshRenderer.h>
#include <MeshSystem/Model.h>

#include <ParticleSystem/MemoryPool.h>
#include <ParticleSystem/ParticleSystem.h>

#include <DeanLib/include/PerformanceTracker.h>
#include <DeanLib/include/MemoryTracker.h>

#include <Lighting/LightingSystem.h>

using namespace std;


using namespace ew;
using namespace macroLib;
using namespace meshSystem;
using namespace skybox;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


bool seeParticleSettings;
bool seeParticleTransform;
bool seeParticleVelocity;
bool seeParticleSpawn;

ParticleSystemValues particleValues;

int main() {
	//initialize window
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}


	//initialize camera
	Camera camera(window);


	//setup ImgGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	//intialize shaders
	Shader unlitShader("assets/defaultVertex.vert", "assets/unlitShader.frag");
	Shader litShader("assets/defaultVertex.vert", "assets/litShader.frag");
	Shader skyboxShader("assets/skyboxVert.vert", "assets/skyboxFrag.frag");
	ShaderSystem shaderSystem = ShaderSystem(&camera);
	shaderSystem.AddShader(&unlitShader);
	shaderSystem.AddShader(&litShader);
	shaderSystem.AddShader(&skyboxShader);


	//initialize skybox
	skybox::Skybox skybox(&skyboxShader);
	skybox.rotationSpeed = 0.02f;


	//initialize terrain
	meshSystem::MeshData planeMeshData;
	meshSystem::generatePlane(10,10,256, &planeMeshData);
	meshSystem::Mesh planeMesh = meshSystem::Mesh(planeMeshData);
	planeMesh.addTexture("assets/grass.png", "texture_diffuse",litShader);
	meshSystem::MeshRenderer terrain = MeshRenderer(planeMesh, Transform(), &litShader);


	//initialize models
	meshSystem::Model backpack = meshSystem::Model("assets/3DModels/backpack/backpack.obj");


	//initialize light system
	lightSystem::LightingSystem lightSystem(&litShader);


	//initialize direction light
	lightSystem::DirectionLight dirLight = lightSystem::DirectionLight();
	lightSystem.AddDirectionLight(&dirLight);


	//initialize cube mesh
	meshSystem::MeshData cubeMeshData;
	meshSystem::generateCube(1.0f, &cubeMeshData);
	meshSystem::Mesh cubeMesh = meshSystem::Mesh(cubeMeshData);


	//initialize particle system
	ParticleSystem particleSystem(32, &unlitShader, cubeMesh);
	particleSystem.setLightingSystemRef(&lightSystem);


	//set light shader material values
	litShader.use();
	litShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	litShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	litShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	litShader.setFloat("material.shininess", 32.0f);


	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		// ---------------------------------- UPDATE TIME------------------------------------------\\
		
		float timeValue = glfwGetTime();
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.updateTime(deltaTime);


		//Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// -------------------------------------GET INPUT------------------------------------------\\

		// Start drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		camera.processInput(window);

		// -------------------------------------UPDATE SYSTEMS----------------------------------\\


		lightSystem.UpdateLighting(camera.getCameraPos());

		shaderSystem.UpdateShaders(timeValue, SCREEN_WIDTH, SCREEN_HEIGHT);

		skybox.Render(timeValue);

		particleSystem.setSystemValues(particleValues);

		particleSystem.updateSystem(timeValue, deltaTime, camera.getCameraPos());

		particleSystem.renderSystem();

		// -------------------------------------RENDER MESHES----------------------------\\

		terrain.modelAndDraw();

		// -------------------------------------RENDER IMGUI----------------------------\\

		//Create a window called Settings
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("dirLight Color", &dirLight.color.r);

		//Separate particle settings
		ImGui::Checkbox("View Particle Settings", &seeParticleSettings);

		if (seeParticleSettings)
		{
			ImGui::Checkbox("Transform!", &seeParticleTransform);
			if (seeParticleTransform)
			{
				ImGui::Checkbox("Use Random Position!", &particleValues.useRandomPosition);
				if (particleValues.useRandomPosition)
				{
					ImGui::DragFloat3("Position Min", &particleValues.randomPosMin.x, 1.0f);
					ImGui::DragFloat3("Position Max", &particleValues.randomPosMax.x, 1.0f);
				}
				else
				{
					ImGui::DragFloat3("System Position", &particleValues.position.x, 1.0f);
				}

				ImGui::DragFloat3("System Rotation", &particleValues.rotation.x, 1.0f);
				ImGui::DragFloat3("System Scale", &particleValues.scale.x, 1.0f);
			}

			ImGui::Checkbox("Velocity!", &seeParticleVelocity);
			if (seeParticleVelocity)
			{
				ImGui::Checkbox("Use Random Velocity", &particleValues.useRandomVelocity);
				ImGui::DragFloat3("Particle Velocity", &particleValues.particleVelocity.x, 0.1f);

				if (particleValues.useRandomVelocity)
				{
					ImGui::DragFloat3("Random * Min", &particleValues.randomVelocityMin.x, 0.1f);
					ImGui::DragFloat3("Random * Max", &particleValues.randomVelocityMax.x, 0.1f);
				}

				ImGui::Checkbox("Cos Velocity", &particleValues.cosVelocity);
				ImGui::Checkbox("Sin Velocity", &particleValues.sinVelocity);
			}

			ImGui::Checkbox("Particle Spawn!", &seeParticleSpawn);
			if (seeParticleSpawn)
			{
				ImGui::DragFloat("Time Between Spawns", &particleValues.timeBetweenSpawns);
				ImGui::DragFloat("Particle Lifetime", &particleValues.particleLifetime);
			}
		}

		glBindVertexArray(0);

		ImGui::End();

		// Do the actual rendering with OpenGL
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// -------------------------------------COMPLETE DRAW----------------------------\\

		//Drawing happens here!
		glfwSwapBuffers(window);

	}

	MemoryTracker::getInstance()->reportAllocations(std::cout);

	printf("Shutting down...");
}