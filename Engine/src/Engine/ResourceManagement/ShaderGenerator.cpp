#include "glad/glad.h"

#include <Engine/ResourceManagement/ShaderGenerator.h>
#include "Engine/Utilities/Log.h"

namespace Engine
{
	// Generates a program for OpenGL to use based on a vertex shader  and a fragment shader.
	// Both shaders have to be passed in as source file code.
	ShaderGenerator::ShaderGenerator(const char* vertexSource, const char* fragmentSource)
	{
		m_programId = glCreateProgram();
		const int vertexShader = genShader(vertexSource, GL_VERTEX_SHADER),
			fragmentShader = genShader(fragmentSource, GL_FRAGMENT_SHADER);

		if (vertexShader == -1 || fragmentShader == -1)
			m_programId = -1;

		glAttachShader(m_programId, vertexShader);
		glAttachShader(m_programId, fragmentShader);
		glLinkProgram(m_programId);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	ShaderGenerator::~ShaderGenerator()
	{
		glDeleteProgram(m_programId);
	}

	int ShaderGenerator::getProgramId() const
	{
		return m_programId;
	}

	// Compiles a shader from a given source code, and returns the shader object if compilation succeeds.
	int ShaderGenerator::genShader(const char* source, GLenum shaderType)
	{
		int shaderObj = glCreateShader(shaderType);
		glShaderSource(shaderObj, 1, &source, nullptr);
		glCompileShader(shaderObj);

		//check for errors
		if (!requestStatus(shaderObj, GL_COMPILE_STATUS))
			return -1;

		return shaderObj;
	}

	bool ShaderGenerator::requestStatus(GLuint shaderObj, GLenum status)
	{
		int success;
		char infoLog[512];
		glGetShaderiv(shaderObj, status, &success);
		if (!success) 
		{
			glGetShaderInfoLog(shaderObj, 512, nullptr, infoLog);
			GE_CORE_ERROR("Error in shader compilation {0}", infoLog);
			return false;
		}
		return true;
	}
}
