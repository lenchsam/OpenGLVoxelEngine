#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Application
{
public:
	int run();
	void init();
private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

	GLFWwindow* window;
};

