#include "Renderer.h"
#include <iostream>
#include <glfw3.h>

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

bool Renderer::Start() {
	std::cout << "Renderer::Start()" << std::endl;
	return true;
}

bool Renderer::Stop() {
	std::cout << "Renderer::Stop()" << std::endl;
	return true;
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::FillColor(float red, float green, float blue) {
	glClearColor(red, green, blue, 1.f);
}

void Renderer::SwapBuffers(void* pWindow) {
	glfwSwapBuffers((GLFWwindow*)pWindow);
}
