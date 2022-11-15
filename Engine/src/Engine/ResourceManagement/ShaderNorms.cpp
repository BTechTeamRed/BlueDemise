#include <Engine/ResourceManagement/ShaderNorms.h>
#include <Engine/ResourceManagement/ShaderGenerator.h>
#include "glad/glad.h"

using namespace std;

namespace Engine {
	const ShaderNorms::ShaderName ShaderNorms::DEFAULT_SHADER_NAME
		= ShaderNorms::ShaderName::SN_TEXTURE_FILL;

	ShaderNorms::ShaderNorms(int stride)
	{
		addShader(DEFAULT_SHADER_NAME);
		addShader(ShaderName::SN_COLOR_FILL);
		assignsNewStride(stride);
	}

	ShaderNorms::~ShaderNorms()
	{
		for (auto [key, value] : m_shaders)
		{
			glDeleteProgram(value);
		}
	}

	void ShaderNorms::addShader(ShaderName shaderName)
	{
		string shaderStr = getShaderNameString(shaderName);
		m_shaders.insert(pair(shaderName, ShaderGenerator(
			ResourceManager::getInstance()->getShaderData(shaderStr + ".vs").c_str(),
			ResourceManager::getInstance()->getShaderData(shaderStr + ".fs").c_str()
		).getProgramId()));
		GE_CORE_INFO(shaderName + " SHADER ADDED.");
	}

	//true if a new stride amount is introduced so that GL doesn't reload the same shader
	bool ShaderNorms::assignsNewStride(int stride)
	{
		bool changed = false;
		if (stride != m_curStride)
		{
			m_curStride = stride;
			changed = true;
		}

		return changed;
	}

	GLuint ShaderNorms::getShader(ShaderName shaderName)
	{
		return m_shaders.at(shaderName);
	}

	string ShaderNorms::getShaderNameString(ShaderName shaderName) {
		switch (shaderName) {
		case ShaderName::SN_TEXTURE_FILL:
			return "TextureFill";
			break;
		default:
			return "ColorFill";
			break;
		}
	}
}