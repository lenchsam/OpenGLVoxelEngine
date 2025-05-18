#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application
{
public:
	int run();
private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
};

