#include "../Headers/Shader.h"
#include "../Headers/Renderer.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath,
	const GLchar* geometryPath, const GLchar* tessControlPath,
	const GLchar* tessEvaluationPath){

	unsigned int vertex;
	unsigned int fragment;
	unsigned int geometry;
	unsigned int tessellationControl;
	unsigned int tessellationEvaluation;

	vertex = SetupShader(vertexPath, GL_VERTEX_SHADER);
	fragment = SetupShader(fragmentPath, GL_FRAGMENT_SHADER);
	if (geometryPath != nullptr) {
		geometry = SetupShader(geometryPath, GL_GEOMETRY_SHADER);
	}
	if (tessControlPath != nullptr) {
		tessellationControl = SetupShader(tessControlPath, GL_TESS_CONTROL_SHADER);
	}if (tessEvaluationPath != nullptr) {
		tessellationEvaluation = SetupShader(tessEvaluationPath, GL_TESS_EVALUATION_SHADER);
	}

	GLCall(ID = glCreateProgram());
	GLCall(glAttachShader(ID, vertex));
	GLCall(glAttachShader(ID, fragment));
	if (geometryPath != nullptr) {
		GLCall(glAttachShader(ID, geometry));
	}
	if (tessControlPath != nullptr) {
		GLCall(glAttachShader(ID, tessellationControl));
	}if (tessEvaluationPath != nullptr) {
		GLCall(glAttachShader(ID, tessellationEvaluation));
	}
	GLCall(glLinkProgram(ID));
	CheckCompileErrors(ID, "PROGRAM");

	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));
	if (geometryPath != nullptr) {
		GLCall(glDeleteShader(geometry));
	}
}

void Shader::Bind() const{
	GLCall(glUseProgram(ID));
}

void Shader::SetBool(const std::string & name, bool value) {
	GLCall(glUniform1i(GetUniformLocation(name.c_str()), (int)value));
}

void Shader::SetInt(const std::string & name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name.c_str()), value));
}

void Shader::SetFloat(const std::string & name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name.c_str()), value));
}

void Shader::SetVec3(const std::string & name, const glm::vec3 & vector) {
	GLCall(glUniform3fv(GetUniformLocation(name.c_str()), 1, &vector[0]));
}

void Shader::SetMat4(const std::string & name, const glm::mat4 & mat) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

int Shader::GetUniformLocation(const std::string & name) {
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[name];
	} else {
		GLCall(int location = glGetUniformLocation(ID, name.c_str()));
		if (location == -1) {
			std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
		}
		m_uniformLocationCache[name] = location;
		return location;
	}
}

unsigned int Shader::SetupShader(const GLchar* shaderPath, GLenum type) {
	std::string code;
	std::ifstream file;
	const char* shaderCode;
	unsigned int id;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << file.rdbuf();
		file.close();
		code = shaderStream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	shaderCode = code.c_str();
	GLCall(id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &shaderCode, NULL));
	GLCall(glCompileShader(id));

	std::string msg;
	if (type == GL_VERTEX_SHADER) msg = "VERTEX";
	else if (type == GL_FRAGMENT_SHADER) msg = "FRAGMENT";
	else if (type == GL_GEOMETRY_SHADER) msg = "GEOMETRY";
	CheckCompileErrors(id, msg);

	return id;
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type){
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
				<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
					<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
}
