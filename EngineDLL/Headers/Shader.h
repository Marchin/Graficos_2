#ifndef SHADER_H
#define SHADER_H

#include <GLAD\glad.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Export.h"

class ENGINEDLL_API Shader {
public:
	unsigned int ID;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath,
		const GLchar* geometryPath = nullptr,
		const GLchar* tessControlPath = nullptr, 
		const GLchar* tessEvaluationPath = nullptr);
	void Bind() const;
	void SetBool(const std::string &name, bool vale);
	void SetInt(const std::string &name, int value);
	void SetUint(const std::string &name, unsigned int value);
	void SetFloat(const std::string &name, float value);
	void SetVec3(const std::string & name, const glm::vec3 & vector);
	void SetMat4(const std::string &name, const glm::mat4 &mat);
	int GetUniformLocation(const std::string &name);
private:
	unsigned int SetupShader(const GLchar* shaderPath, GLenum type);
	void CheckCompileErrors(unsigned int shader, std::string type);
	std::unordered_map<std::string, int> m_uniformLocationCache;
};

#endif // !SHADER_H
