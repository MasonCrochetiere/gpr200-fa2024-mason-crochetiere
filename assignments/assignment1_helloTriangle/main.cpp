#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[] = {
	// first triangle
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f,  0.5f, 0.0f,  // top left 
};

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); 
}
)";
// this is the source code we are sending to the GPU to run and make the vertex shader

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";
// same deal as vertex shader source; code we send to the GPU to generate color. 
// the variable's name is arbitrary

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

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Initialization goes here!
	unsigned int VBO;
	glGenBuffers(1, &VBO); // populates VBO with the next available buffer indicator
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // sets the buffer to mess with to be VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // so this transfers vertex data from CPU to GPU?

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // telling GPU when to jump in data (how many bytes?) to get to each vertex
	glEnableVertexAttribArray(0);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // similar to VBO, populating vertexShader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // apply our source code to this shader. the 1 means we're only supplying 1 shader
	glCompileShader(vertexShader); // same as compiling any other code. Could fail...

	int success;
	char infoLog[512]; // arbitrary number to hold error log

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); 
	if (!success)
	{
		
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // similar to VBO, populating vertexShader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // apply our source code to this shader. the 1 means we're only supplying 1 shader
	glCompileShader(fragmentShader); // same as compiling any other code. Could fail...

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)	
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // program combines shaders
	glAttachShader(shaderProgram, vertexShader); 
	glAttachShader(shaderProgram, fragmentShader); // attach our vertex and color shaders
	glLinkProgram(shaderProgram); // links shaders all together

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::PROGRAM::LINKING_FAILED\n%s", infoLog);
	}

	glUseProgram(shaderProgram); // need to make sure we use the program :)
	// consider using this during game loop instead, esp if swapping between multiple program

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// shader cleanup

	//glBindBuffer(GL_ARRAY_BUFFER, 0); // sets the buffer to mess with to be 0, so nothing
	
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
