#include <Engine/ResourceManagement/ShaderNorms.h>
#include "Engine/ResourceManagement/ResourceManager.h"
#include "glad/glad.h"

using namespace std;

namespace Engine
{
	ShaderNorms* ShaderNorms::m_singleton = nullptr;

	ShaderNorms::ShaderNorms()
	{
		addShader(ShaderFillType::DEFAULT_FILL_TYPE);
		addShader(ShaderFillType::FillType::SN_COLOR_FILL);
		addShader(ShaderFillType::FillType::SN_GRADIENT_FILL);
		addAdvancedShader(0, "SinWave");
	}

	ShaderNorms::~ShaderNorms()
	{
		for (auto [key, value] : m_shaders)
		{
			glDeleteProgram(value.getProgramId());
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
				programId = getShaderReference();
				glUseProgram(programId);
			}
			else if (stride == colorCoordinates)
			{
				programId = getShaderReference(ShaderFillType::FillType::SN_COLOR_FILL);
				glUseProgram(programId);
			}
			else if (stride == gradientCoordinates)
			{
				programId = getShaderReference(ShaderFillType::FillType::SN_GRADIENT_FILL);
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

	void ShaderNorms::addShader(ShaderFillType::FillType shaderName)
	{
		string shaderStr = getShaderNameString(shaderName);
		m_shaders.insert(pair(shaderName, ShaderGenerator(
			ResourceManager::getInstance()->getShaderData(shaderStr + ".vs").c_str(),
			ResourceManager::getInstance()->getShaderData(shaderStr + ".fs").c_str()
		)));
		GE_CORE_INFO(getShaderNameString(shaderName) + " shader added.");
	}

	void ShaderNorms::addAdvancedShader(int bind, std::string shaderName)
	{
		string asSource = ResourceManager::getInstance()->getShaderData(shaderName + ".as");
		GE_CORE_INFO(asSource.c_str());
		m_advancedShaders.insert(pair(bind, AdvancedShaderDistributor(asSource.c_str())));
	}

	GLuint ShaderNorms::getShaderReference(ShaderFillType::FillType shaderName)
	{
		return m_shaders.at(shaderName).getProgramId();
	}

	string ShaderNorms::getShaderNameString(ShaderFillType::FillType shaderName)
	{
		switch (shaderName)
		{
		case ShaderFillType::FillType::SN_TEXTURE_FILL:
			return "TextureFill";
			break;
		case ShaderFillType::FillType::SN_COLOR_FILL:
			return "ColorFill";
			break;
		default:
			return "GradientFill";
			break;
		}
	}
}