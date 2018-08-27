#include "Renderer.h"
#include <iostream>

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
