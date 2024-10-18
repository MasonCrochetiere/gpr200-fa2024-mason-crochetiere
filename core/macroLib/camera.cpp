#include "camera.h"

// solutions in this class for getting the GLFW functions in a class and not main were grabbed from this stackoverflow
// https://stackoverflow.com/questions/78314504/is-there-a-way-to-use-glfwsetscrollcallback-function-inside-a-class

namespace macroLib {

    Camera::Camera(GLFWwindow* window)
    {
		glfwSetWindowUserPointer(window, this);

		glfwSetFramebufferSizeCallback(window, Camera::framebuffer_size_callback);
		glfwSetCursorPosCallback(window, Camera::mouse_callback);
		glfwSetScrollCallback(window, Camera::scroll_callback);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

	void Camera::updateTime(float newDeltaTime)
	{
		deltaTime = newDeltaTime;
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
	void Camera::processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float sprint = 1.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			sprint *= sprintSpeed;

		float cameraSpeed = static_cast<float>(defaultSpeed * sprint * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp;
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void Camera::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void Camera::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		Camera* camera = (Camera*)glfwGetWindowUserPointer(window);

		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (camera->firstMouse)
		{
			camera->lastX = xpos;
			camera->lastY = ypos;
			camera->firstMouse = false;
		}

		float xoffset = xpos - camera->lastX;
		float yoffset = camera->lastY - ypos; // reversed since y-coordinates go from bottom to top
		camera->lastX = xpos;
		camera->lastY = ypos;

		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera->yaw += xoffset;
		camera->pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (camera->pitch > 89.0f)
			camera->pitch = 89.0f;
		if (camera->pitch < -89.0f)
			camera->pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
		front.y = sin(glm::radians(camera->pitch));
		front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
		camera->cameraFront = glm::normalize(front);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Camera* camera = (Camera*)glfwGetWindowUserPointer(window);

		camera->fov -= (float)yoffset;
		if (camera->fov < 1.0f)
			camera->fov = 1.0f;
		if (camera->fov > 120.0f)
			camera->fov = 120.0f;
	}

}