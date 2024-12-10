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

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int CUBE_COUNT = 20;

const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(0.5f, 0.5f, 0.5f);

float ambientK = 0.1f;
float diffuseK = 1.0f;
float specularK = 0.08f;
float shininess = 2.0f;


bool seeParticleSettings;
bool seeParticleTransform;
bool seeParticleVelocity;
bool seeParticleSpawn;

ParticleSystemValues particleValues;

float radius = 5.0f;
int numSegments = 8;
bool pointRender = false;



float vertices[] = {
	//  X      Y      Z      U     V     NX     NY     NZ
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

float skyboxVertices[] = {
	// positions          
	-100.0f,  100.0f, -100.0f,
	-100.0f, -100.0f, -100.0f,
	 100.0f, -100.0f, -100.0f,
	 100.0f, -100.0f, -100.0f,
	 100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,

	-100.0f, -100.0f,  100.0f,
	-100.0f, -100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f,  100.0f,
	-100.0f, -100.0f,  100.0f,

	 100.0f, -100.0f, -100.0f,
	 100.0f, -100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f, -100.0f,
	 100.0f, -100.0f, -100.0f,

	-100.0f, -100.0f,  100.0f,
	-100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f, -100.0f,  100.0f,
	-100.0f, -100.0f,  100.0f,

	-100.0f,  100.0f, -100.0f,
	 100.0f,  100.0f, -100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	-100.0f,  100.0f,  100.0f,
	-100.0f,  100.0f, -100.0f,

	-100.0f, -100.0f, -100.0f,
	-100.0f, -100.0f,  100.0f,
	 100.0f, -100.0f, -100.0f,
	 100.0f, -100.0f, -100.0f,
	-100.0f, -100.0f,  100.0f,
	 100.0f, -100.0f,  100.0f
};


unsigned int loadCubemap(std::vector<std::string> faces)
{
	stbi_set_flip_vertically_on_load(false);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

int main() {
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

	Camera camera(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	Texture2D texture("assets/blankCube.png", 0, 0);

	//Texture2D texture1("assets/brickTexture.png", 0, 0);
	//Texture2D texture2("assets/awesomeface.png", 0, 0);

	Shader unlitShader("assets/defaultVertex.vert", "assets/unlitShader.frag");
	Shader litShader("assets/defaultVertex.vert", "assets/litShader.frag");
	Shader skyboxShader("assets/skyboxVert.vert", "assets/skyboxFrag.frag");
	//Shader bgShader("assets/vertexShaderBG.vert", "assets/fragmentShaderBG.frag");



	meshSystem::MeshData planeMeshData;
	meshSystem::generatePlane(10,10,256, &planeMeshData);
	meshSystem::Mesh planeMesh = meshSystem::Mesh(planeMeshData);
	planeMesh.addTexture("assets/grass.png", "texture_diffuse",litShader);
	meshSystem::MeshRenderer terrain = MeshRenderer(planeMesh, Transform(), &litShader);
	//terrain.transform.position = glm::vec3(-1000, -50, -1000);

	meshSystem::Model backpack = meshSystem::Model("assets/3DModels/backpack/backpack.obj");

	std::vector<std::string> faces
	{
		"assets/skybox/newRight.png",
		"assets/skybox/newLeft.png",
		"assets/skybox/newTop.png",
		"assets/skybox/newBottom.png",
		"assets/skybox/newFront.png",
		"assets/skybox/newBack.png"
	};
	unsigned int cubemapTexture = loadCubemap(faces);


	// initializing the sphere
	meshSystem::MeshData sphereMeshData;
	meshSystem::createSphere(800.0f, 256, &sphereMeshData);
	meshSystem::Mesh sphereMesh = meshSystem::Mesh(sphereMeshData);
	meshSystem::MeshRenderer sphereRenderer = meshSystem::MeshRenderer(sphereMesh, &skyboxShader);

	meshSystem::MeshData cubeMeshData;
	//meshSystem::generatePlane(5.0f, 5.0f, 32, &cubeMeshData);

	meshSystem::generateCube(1.0f, &cubeMeshData);
	meshSystem::Mesh cubeMesh = meshSystem::Mesh(cubeMeshData);

	ParticleSystem particleSystem(32, &litShader, cubeMesh);

	meshSystem::MeshRenderer bigCube = MeshRenderer(cubeMesh, Transform(), &litShader);

	lightSystem::LightingSystem lightSystem(&litShader);

	lightSystem::PointLight myLight = lightSystem::PointLight();
	lightSystem::DirectionLight dirLight = lightSystem::DirectionLight();
	lightSystem.AddPointLight(&myLight);
	lightSystem.AddDirectionLight(&dirLight);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);

		// ---------------------------------- UPDATE TIME------------------------------------------\\
		
		float timeValue = glfwGetTime();

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.updateTime(deltaTime);

		// -------------------------------------GET INPUT------------------------------------------\\

		// Start drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		camera.processInput(window);

		// -------------------------------------OBJECT/GAME LOGIC----------------------------------\\

		litShader.use();

		particleSystem.setSystemValues(particleValues);

		particleSystem.updateSystem(timeValue, deltaTime, camera.getCameraPos());

		lightSystem.UpdateLighting(camera.getCameraPos());

		//Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// ---------RENDER LOGIC (So most of it because graphics programming LOL)------------------\\


		// -------------------------------------RENDER SKYBOX 1----------------------------\\

		skyboxShader.use();
		{
			glm::mat4 sphereSpin = glm::mat4(1.0f);
			sphereSpin = glm::rotate(sphereSpin, timeValue * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			skyboxShader.setMat4("model", sphereSpin);
			sphereRenderer.modelAndDraw();
		}



		// -------------------------------------RENDER PARTICLE SYSTEM----------------------------\\

		litShader.use();

		litShader.setVec3("lightPos", lightPos);
		litShader.setVec3("lightColor", lightColor);
		litShader.setVec3("viewPos", camera.getCameraPos());

		litShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		litShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		litShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		litShader.setFloat("material.shininess", 32.0f);



		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		model = glm::rotate(model, timeValue * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		// pass projection matrix to shader (note that in this case it could change every frame)
		projection = glm::perspective(glm::radians(camera.getFOV()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
		litShader.setMat4("projection", projection);

		// setting uniform values. probably want to get the vertex locations outside of update for efficiency
		litShader.setFloat("uTime", timeValue);

		// same for View Matrix and Projection Matrix

		particleSystem.renderSystem();



		int viewLoc = glGetUniformLocation(litShader.ID, "view");
		litShader.setMat4("view", view);

		int projectionLoc = glGetUniformLocation(litShader.ID, "projection");
		litShader.setMat4("projection", projection);

		// -------------------------------------RENDER LIGHT CUBE----------------------------\\

		terrain.modelAndDraw();
		backpack.Draw(litShader);
		litShader.use();


		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		litShader.setMat4("model", model);
		litShader.setVec3("lightColor", lightColor);

		bigCube.transform.position = myLight.position;
		bigCube.transform.scale = glm::vec3(0.3f);
		bigCube.modelAndDraw();

		// -------------------------------------RENDER SKYBOX 2----------------------------\\
		
		skyboxShader.use();
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		skyboxShader.setFloat("uTime", timeValue);
		// ... draw the rest of the scene

		// -------------------------------------RENDER IMGUI----------------------------\\

		//Create a window called Settings
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("dirLight Color", &dirLight.color.r);
		ImGui::DragFloat3("Light Position", &myLight.position.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &myLight.color.r);
		ImGui::SliderFloat("Ambient K", &myLight.ambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &myLight.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &myLight.specular, 0.0f, 1.0f);

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