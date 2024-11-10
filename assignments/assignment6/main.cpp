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

#include <ParticleSystem/MemoryPool.h>
#include <ParticleSystem/ParticleSystem.h>

#include <DeanLib/include/PerformanceTracker.h>
#include <DeanLib/include/MemoryTracker.h>

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

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};





unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

int main() {
	{
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

		Shader cubeShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
		Shader lightShader("assets/vertexShader.vert", "assets/lightFragmentShader.frag");
		//Shader bgShader("assets/vertexShaderBG.vert", "assets/fragmentShaderBG.frag");

		meshSystem::MeshData cubeMeshData;
		//meshSystem::generatePlane(5.0f, 5.0f, 32, &cubeMeshData);
		meshSystem::generateCube(1.0f, &cubeMeshData);
		meshSystem::Mesh cubeMesh = meshSystem::Mesh(cubeMeshData);
		meshSystem::MeshRenderer bigCube = MeshRenderer(cubeMesh, Transform(), &cubeShader);

		//int poolSize = 32;
		//MemoryPool particlePool(32, sizeof(meshSystem::MeshRenderer));
		//std::vector<MeshRenderer*> particleVec;

		ParticleSystem particleSystem(32);

		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Initialization goes here!
		unsigned int VBO;
		glGenBuffers(1, &VBO); // populates VBO with the next available buffer indicator
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // sets the buffer to mess with to be VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // so this transfers vertex data from CPU to GPU?

		// position (getting XYZ)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		// telling GPU when to jump in data (how many bytes slash floats) to get to each vertex
		glEnableVertexAttribArray(0);

		// color (getting RGBA)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
		// same as above function. the final parameter is the offset (in bytes) to retrieve the data
		glEnableVertexAttribArray(1);

		// setting Normals
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));
		// same as above.
		glEnableVertexAttribArray(2);

		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

		//Render loop
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();

			// ---------------------------------- UPDATE TIME------------------------------------------\\
			
			// per-frame time logic
			float timeValue = glfwGetTime();

			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			camera.updateTime(deltaTime);

			// -------------------------------------GET INPUT------------------------------------------\\

			camera.processInput(window);

			// -------------------------------------OBJECT/GAME LOGIC----------------------------------\\

			particleSystem.updateSystem(timeValue, deltaTime);

			// ---------RENDER LOGIC (So most of it because graphics programming LOL)------------------\\

			//Clear framebuffer
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

			// be sure to activate the shader
			cubeShader.use();

			cubeShader.setVec3("lightPos", lightPos);
			cubeShader.setVec3("lightColor", lightColor);
			cubeShader.setVec3("viewPos", camera.getCameraPos());

			cubeShader.setFloat("ambientStrength", ambientK);
			cubeShader.setFloat("diffuseStrength", diffuseK);
			cubeShader.setFloat("specularStrength", specularK);
			cubeShader.setFloat("shininessStrength", shininess);

			texture.Bind(GL_TEXTURE0);

			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);

			// note that we're translating the scene in the reverse direction of where we want to move
			view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

			model = glm::rotate(model, timeValue * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

			// pass projection matrix to shader (note that in this case it could change every frame)
			projection = glm::perspective(glm::radians(camera.getFOV()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
			cubeShader.setMat4("projection", projection);

			// setting uniform values. probably want to get the vertex locations outside of update for efficiency
			cubeShader.setFloat("uTime", timeValue);

			int viewLoc = glGetUniformLocation(cubeShader.ID, "view");
			cubeShader.setMat4("view", view);

			int projectionLoc = glGetUniformLocation(cubeShader.ID, "projection");
			cubeShader.setMat4("projection", projection);
			// same for View Matrix and Projection Matrix

			glBindVertexArray(VAO);
			bigCube.modelAndDraw();

			particleSystem.renderSystem();

			lightShader.use();


			viewLoc = glGetUniformLocation(lightShader.ID, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
			projectionLoc = glGetUniformLocation(lightShader.ID, "projection");
			lightShader.setMat4("projection", projection);

			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			lightShader.setMat4("model", model);
			lightShader.setVec3("lightColor", lightColor);

			//glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);

			// Start drawing ImGUI
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			//Create a window called Settings
			ImGui::Begin("Settings");
			ImGui::Text("Add Controls Here!");

			ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
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

		//for (unsigned int i = 0; i < particleVec.size(); i++)
		//{
		//	MeshRenderer* target = (MeshRenderer*)particleVec.at(i);

		//	target->~MeshRenderer();
		//	particlePool.freeObject((Byte*)target);

		//}
		//particleVec.clear();

		//std::cout << "Pooled objects " << particlePool.getNumFreeObjects() << std::endl;
	} // so this set of brackets exists to put the project all in one scope
	// that way any instanced objects are deleted at the end
	// which means that checking for memory leaks:tm: works just fine.
	MemoryTracker::getInstance()->reportAllocations(std::cout);

	printf("Shutting down...");
}

void runProgram()
{


}