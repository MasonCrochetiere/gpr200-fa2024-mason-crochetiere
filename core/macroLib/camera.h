#ifndef CAMERA_H
#define CAMERA_H

// 

#include "../ew/external/glad.h" // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace macroLib
{
    class Camera
    {
    public:
        Camera(GLFWwindow* window);

        void updateTime(float newDeltaTime);

        void processInput(GLFWwindow* window); // this function could potentially just use the window if it were saved from the constructor but i liked this better
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

        glm::vec3 getCameraPos() { return cameraPos; }
        glm::vec3 getCameraFront() { return cameraFront; }
        glm::vec3 getCameraUp() { return cameraUp; }

        float getFOV() { return fov; }
    private:
        bool firstMouse = true;

        float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
        float pitch = 0.0f;

        float lastX = 800.0f / 2.0;
        float lastY = 600.0 / 2.0;
        float fov = 60.0f;

        float defaultSpeed = 2.5f;
        float sprintSpeed = 2.0f;

        // timing
        float deltaTime = 0.0f;	// time between current frame and last frame

        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    };
}

#endif