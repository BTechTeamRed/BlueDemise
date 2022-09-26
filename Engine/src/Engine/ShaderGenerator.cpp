#include "ShaderGenerator.h"

namespace input {
	ShaderGenerator::ShaderGenerator(const char* vertexSource, const char* fragmentSource) {
		m_programId = glCreateProgram();
		int vertexShader = genShader(vertexSource, GL_VERTEX_SHADER),
			fragmentShader = genShader(fragmentSource, GL_FRAGMENT_SHADER);

		if (vertexShader == -1 || fragmentShader == -1)
			m_programId = -1;

		glAttachShader(m_programId, vertexShader);
		glAttachShader(m_programId, fragmentShader);
		glLinkProgram(m_programId);

		//check for errors
		if (requestStatus(m_programId, GL_LINK_STATUS) == -1)
			m_programId = -1;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	ShaderGenerator::~ShaderGenerator() {
		glDeleteProgram(m_programId);
	}

	int ShaderGenerator::getProgramId() {
		return m_programId;
	}

	int ShaderGenerator::genShader(const char* source, GLenum shaderType) {
		int shaderObj = glCreateShader(shaderType);
		glShaderSource(shaderObj, 1, &source, NULL);
		glCompileShader(shaderObj);

		//check for errors
		if (requestStatus(shaderObj, GL_COMPILE_STATUS) == -1)
			return -1;

		return shaderObj;
	}

	int ShaderGenerator::requestStatus(GLuint shaderObj, GLenum status) {
		int success;
		char infoLog[512];
		glGetShaderiv(shaderObj, status, &success);
		if (!success) {
			glGetShaderInfoLog(shaderObj, 512, NULL, infoLog);
			std::cout << "Error in shader compilation: " << infoLog << std::endl;
			return -1;
		}
		return 0;
	}
}