#include "../Headers/Renderer.h"
#include <iostream>
#include <glfw3.h>

Renderer::Renderer() {
	m_model = glm::mat4(1.f);
	m_view= glm::mat4(1.f);
	m_projection = glm::mat4(1.f);
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
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::FillColor(float red, float green, float blue) {
	GLCall(glClearColor(red, green, blue, 1.f));
}

void Renderer::SwapBuffers(void* pWindow) {
	GLCall(glfwSwapBuffers((GLFWwindow*)pWindow));
}

void Renderer::DrawTriangles(unsigned int offset, unsigned int count) {
	GLCall(glDrawArrays(GL_TRIANGLES, offset, count));
}

void Renderer::ResetModelMatrix() {
	m_model = glm::mat4(1.f);
}

void Renderer::MultiplyModelMatrix(glm::mat4 transformation) {
	m_model *= transformation;
}

glm::mat4 Renderer::GetModelViewProj() const {
	return (m_projection * m_view * m_model);
}

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function
			<< " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}
