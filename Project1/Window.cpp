#include "Window.h"
#include <iostream>
#include <glfw3.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


Window::Window(unsigned int width, unsigned int height, const char * name) {

}

bool Window::Start() {
	std::cout << "Window::Start()" << std::endl;
	if (!glfwInit()) {
		return -1;
	}
	m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpengGL", NULL, NULL);
	if (!m_window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	return true;
}

bool Window::Stop() {
	if (m_window != nullptr) {

	}
	std::cout << "Window::Stop()" << std::endl;
	return true;
}
