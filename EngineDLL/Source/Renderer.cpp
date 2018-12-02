#include "../Headers/Renderer.h"
#include <iostream>
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() : m_halfCamHeight(10.f), m_halfCamWidth(10.f), m_camPosX(0), m_camPosY(0) {
	m_model = glm::mat4(1.f);
	m_view= glm::lookAt(glm::vec3(m_camPosX, m_camPosY,3.f), glm::vec3(m_camPosX, m_camPosY, 0.f), glm::vec3(0.f, 1.f, 0.f));
	m_projection = glm::ortho(-m_halfCamWidth, m_halfCamWidth, -m_halfCamHeight, m_halfCamHeight, 0.f, 100.f);
}

Renderer::~Renderer() {
}

bool Renderer::Start() {
	std::cout << "Renderer::Start()" << std::endl;
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
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

void Renderer::DrawBuffer(unsigned int offset, unsigned int count) {
	GLCall(glDrawArrays(GL_TRIANGLES, offset, count));
}

void Renderer::DrawBufferStrip(unsigned int offset, unsigned int count) {
	GLCall(glDrawArrays(GL_TRIANGLE_STRIP, offset, count));
}

void Renderer::DrawBufferFan(unsigned int offset, unsigned int count) {
	GLCall(glDrawArrays(GL_TRIANGLE_FAN, offset, count));
}

void Renderer::DrawElements(unsigned int count) {
	GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0));
}

void Renderer::ResetModelMatrix() {
	m_model = glm::mat4(1.f);
}

void Renderer::MultiplyModelMatrix(glm::mat4 transformation) {
	m_model *= transformation;
}

void Renderer::SetModelMatrix(glm::mat4 model) {
	m_model = model;
}

glm::mat4 Renderer::GetModelViewProj() const {
	return (m_projection * m_view * m_model);
}

void Renderer::GetCameraPosition(float* x, float* y) {
	*x = m_camPosX;
	*y = m_camPosY;
}

void Renderer::SetCameraPosition(float x, float y) {
	m_camPosX = x;
	m_camPosY = y;
	m_view = glm::lookAt(glm::vec3(x, y,3.f), glm::vec3(x, y, 0.f), glm::vec3(0.f, 1.f, 0.f));
}

float Renderer::GetCameraWidth() {
	return m_halfCamWidth * 2.f;
}

float Renderer::GetCameraHeight() {
	return m_halfCamHeight * 2.f;
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
