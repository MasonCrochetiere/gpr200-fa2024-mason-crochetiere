#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <macroLib/shader.h>
#include <macroLib/texture2D.h>
using namespace macroLib;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[] = {
	// location         colors                 texture coords  
	// X    Y     Z     R     G     B     A
	  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top right
	  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right
	 -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left
	 -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // fourth vertice
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

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

	Texture2D texture("assets/BigChungus.jpg", 0, 0);

	Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Initialization goes here!
	unsigned int VBO;
	glGenBuffers(1, &VBO); // populates VBO with the next available buffer indicator
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // sets the buffer to mess with to be VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // so this transfers vertex data from CPU to GPU?

	unsigned int EBO;
	glGenBuffers(1, &EBO); // populates EBO with the next available buffer indicator
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // sets the buffer to mess with to be VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // so this transfers vertex data from CPU to GPU with our indices as elements?

	// position (getting XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); // telling GPU when to jump in data (how many bytes slash floats) to get to each vertex
	glEnableVertexAttribArray(0);

	// color (getting RGBA)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3)); 
	// same as above function. the final parameter is the offset (in bytes) to retrieve the data
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float))); // this is retrieving texture data. same as above
	glEnableVertexAttribArray(2);
	
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// be sure to activate the shader
		ourShader.use();

		// setting uniform values. probably want to get the vertex locations outside of update for efficiency
		float timeValue = glfwGetTime();
		ourShader.setFloat("uTime", timeValue);

		texture.Bind();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0); 

		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
