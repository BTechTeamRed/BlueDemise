#include <Engine/ResourceManagement/ShaderNorms.h>
#include <Engine/ResourceManagement/ShaderGenerator.h>
#include "Engine/ResourceManagement/ResourceManager.h"
#include "glad/glad.h"

using namespace std;

namespace Engine
{
	const ShaderNorms::ShaderName ShaderNorms::DEFAULT_SHADER_NAME
		= ShaderNorms::ShaderName::SN_TEXTURE_FILL;

	ShaderNorms* ShaderNorms::m_singleton = nullptr;

	ShaderNorms::ShaderNorms()
	{
		addShader(DEFAULT_SHADER_NAME);
		addShader(SN_COLOR_FILL);
		addShader(SN_TEXT_FILL);
		addShader(SN_GRADIENT_FILL);
	}

	ShaderNorms::~ShaderNorms()
	{
		for (auto [key, value] : m_shaders)
		{
			glDeleteProgram(value);
		}
	}

	ShaderNorms* ShaderNorms::getInstance()
	{
		if (m_singleton == nullptr)
		{
			m_singleton = new ShaderNorms();
		}
		return m_singleton;
	}

	//tests the current renderable or vbo being rendered with the previous. If the current
	//stride doesn't match the previous, a new shader is loaded. Gets called by renderer.
	void ShaderNorms::update(int stride, int& textureCoordinates, int& colorCoordinates,
		int& gradientCoordinates, GLuint& programId)
	{
		if (assignsNewStride(stride))
		{
			if (stride == textureCoordinates)
			{
				programId = getShader();
				glUseProgram(programId);
			}
			else if (stride == colorCoordinates)
			{
				programId = getShader(SN_COLOR_FILL);
				glUseProgram(programId);
			}
			else if (stride == gradientCoordinates)
			{
				programId = getShader(SN_GRADIENT_FILL);
				glUseProgram(programId);
			}
		}
	}

	//true if a new stride amount is introduced so that GL doesn't reload the same shader
	bool ShaderNorms::assignsNewStride(int stride)
	{
		bool changed = false;
		if (stride != m_currentStride)
		{
			m_currentStride = stride;
			changed = true;
		}

		return changed;
	}

	void ShaderNorms::addShader(ShaderName shaderName)
	{
		string shaderStr = getShaderNameString(shaderName);
		m_shaders.insert(pair(shaderName, ShaderGenerator(
			ResourceManager::getInstance()->getShaderData(shaderStr + ".vs").c_str(),
			ResourceManager::getInstance()->getShaderData(shaderStr + ".fs").c_str()
		).getProgramId()));
		GE_CORE_INFO(getShaderNameString(shaderName) + " shader added.");
	}

	GLuint ShaderNorms::getShader(ShaderName shaderName)
	{
		return m_shaders.at(shaderName);
	}

	string ShaderNorms::getShaderNameString(ShaderName shaderName)
	{
		switch (shaderName)
		{
		case SN_TEXTURE_FILL:
			return "TextureFill";
		case SN_COLOR_FILL:
			return "ColorFill";
		case SN_TEXT_FILL:
			return "TextFill";
		default:
			return "GradientFill";
		}
	}
}