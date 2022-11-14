#include <Engine/ResourceManagement/ShaderGenerator.h>
#include "Engine/Utilities/Log.h"
#include "glad/glad.h"

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

	ShaderNorms::ShaderNorms() : m_defaultShader("TextureFill")
	{
		addShader(m_defaultShader);
		addShader("ColorFill");
	}

	ShaderNorms::~ShaderNorms()
	{
		for (auto [key, value] : m_shaders)
		{
			glDeleteProgram(value);
		}
	}

	void ShaderNorms::addShader(string shaderName)
	{
		m_shaders.insert(pair(shaderName, ShaderGenerator(
			ResourceManager::getInstance()->getShaderData(shaderName + ".vs").c_str(),
			ResourceManager::getInstance()->getShaderData(shaderName + ".fs").c_str()
		).getProgramId()));
		GE_CORE_INFO(shaderName +" ADDED.");
	}

	//true if a new stride amount is introduced so that GL doesn't reload the same shader
	bool ShaderNorms::queueStride(int stride)
	{
		bool changed = false;
		if (stride != curStride)
		{
			curStride = stride;
			changed = true;
		}

		return changed;
	}

	GLuint ShaderNorms::getShader(string shaderName)
	{
		return m_shaders.at(shaderName);
	}

	GLuint ShaderNorms::getDefaultShader()
	{
		return getShader(m_defaultShader);
	}
}
