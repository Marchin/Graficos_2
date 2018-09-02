#include "Window.h"
#include <iostream>
#include <glfw3.h>


bool Window::Start(unsigned int width, unsigned int height, const char * pName) {
	std::cout << "Window::Start()" << std::endl;
	m_width = width;
	m_height = height;
	m_pName = pName;
	if (!glfwInit()) {
		return -1;
	}
	m_pWindow = glfwCreateWindow(m_width, m_height, m_pName, NULL, NULL);
	if (!m_pWindow) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent((GLFWwindow*)m_pWindow);
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
	glfwPollEvents();
}

void Window::Refresh() {
	glClearColor(0.0f, 0.0f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers((GLFWwindow*)m_pWindow);
}

void* Window::GetWindow() {
	return m_pWindow;
}