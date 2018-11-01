#include "../Headers/Window.h"
#include "../Headers/Renderer.h"
#include <iostream>
#include <GLAD\glad.h>
#include <glfw3.h>

bool Window::Start(unsigned int width, unsigned int height, const char * pName) {
	std::cout << "Window::Start()" << std::endl;
	m_width = width;
	m_height = height;
	m_pName = pName;
	if (!glfwInit()) {
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_pWindow = glfwCreateWindow(m_width, m_height, m_pName, NULL, NULL);
	if (!m_pWindow) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent((GLFWwindow*)m_pWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return true;
}

bool Window::Stop() {
	std::cout << "Window::Stop()" << std::endl;
	if (m_pWindow != nullptr) {
		glfwDestroyWindow((GLFWwindow*)m_pWindow);
	}
	m_pWindow = NULL;

	glfwTerminate();
	return true;
}

bool Window::ShouldClose() {
	if (m_pWindow) {
		return glfwWindowShouldClose((GLFWwindow*)m_pWindow);
	} else {
		return true;
	}
}

void Window::PollEvents() {
	if (glfwGetKey((GLFWwindow*)m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose((GLFWwindow*)m_pWindow, true);
	}
	GLCall(glfwPollEvents());
}


void* Window::GetWindow() {
	return m_pWindow;
}