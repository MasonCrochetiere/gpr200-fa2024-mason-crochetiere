#include <stdio.h>
#include <math.h>

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

glm::vec3 lightColor(0.5f, 0.5f, 0.5f);

float ambientK = 0.1f;
float diffuseK = 1.0f;
float specularK = 0.08f;
float shininess = 2.0f;


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
	Camera camera(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	Texture2D texture("assets/cube.png", 0, 0);
	//Texture2D texture1("assets/brickTexture.png", 0, 0);
	//Texture2D texture2("assets/awesomeface.png", 0, 0);

	Shader litShader("assets/defaultVertex.vert", "assets/litShader.frag");
	Shader unlitShader("assets/defaultVertex.vert", "assets/unlitShader.frag");

	meshSystem::MeshData planeMeshData;
	meshSystem::MeshData cubeMeshData;
	meshSystem::generatePlane(5.0f,5.0f,32, &planeMeshData);
	meshSystem::generateCube(1.0f,&cubeMeshData);
	meshSystem::Mesh planeMesh = meshSystem::Mesh(planeMeshData);
	meshSystem::Mesh cubeMesh = meshSystem::Mesh(cubeMeshData);
	meshSystem::MeshRenderer bigPlane = MeshRenderer(planeMesh,Transform(),&litShader);
	meshSystem::MeshRenderer lightCube = MeshRenderer(cubeMesh,Transform(),&unlitShader);



	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.updateTime(deltaTime);

		// input
		camera.processInput(window);

		//Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1.getID());
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2.getID());

		float timeValue = glfwGetTime();

		// be sure to activate the shader
		litShader.use();
		litShader.setVec3("pointLights[0].position", lightCube.transform.position + glm::vec3(0.5f) * lightCube.transform.scale);
		//litShader.setVec3("lightColor", lightColor);
		litShader.setVec3("viewPos", camera.getCameraPos());
		litShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		litShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		litShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		litShader.setFloat("material.shininess", 32.0f);
		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		litShader.setVec3("pointLights[0].ambient", ambientColor);
		litShader.setVec3("pointLights[0].diffuse", diffuseColor);
		litShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		litShader.setFloat("pointLights[0].constant",  1.0f);
		litShader.setFloat("pointLights[0].linear",    0.09f);
		litShader.setFloat("pointLights[0].quadratic", 0.032f);
		//litShader.setFloat("pointLights[0].innerCutOff", glm::cos(glm::radians(10.0f)));
		//litShader.setFloat("pointLights[0].outerCutOff", glm::cos(glm::radians(25.0f)));
		//litShader.setVec3("pointLights[0].direction", camera.getCameraFront());

		texture.Bind(GL_TEXTURE0);


		//calculate and set view and projection
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
		projection = glm::perspective(glm::radians(camera.getFOV()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
		litShader.setMat4("view", view);
		litShader.setMat4("projection", projection);

		unlitShader.use();
		unlitShader.setMat4("view", view);
		unlitShader.setMat4("projection", projection);
		unlitShader.setVec3("color",glm::vec3(1.0f, 1.0f, 1.0f));

		bigPlane.modelAndDraw();
		lightCube.modelAndDraw();


		// Start drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//Create a window called Settings
		ImGui::Begin("Settings");
		ImGui::Text("Add Controls Here!");

		ImGui::DragFloat3("Light Position", &lightCube.transform.position.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &shininess, 2.0f, 1024.0f);

		ImGui::End();

		// Do the actual rendering with OpenGL
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}