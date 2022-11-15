#pragma once
#ifndef SHADERNORMS_H
#define SHADERNORMS_H

#include "Engine/ResourceManagement/ResourceManager.h"
#include <string>
#include <map>

namespace Engine {
	typedef unsigned int GLuint;

	class ShaderNorms {
	public:
		static enum ShaderName {
			SN_TEXTURE_FILL,
			SN_COLOR_FILL
		};
		static const ShaderName DEFAULT_SHADER_NAME;

		ShaderNorms(int stride);
		~ShaderNorms();
		void addShader(ShaderName shaderName);
		bool assignsNewStride(int stride);
		GLuint getShader(ShaderName = DEFAULT_SHADER_NAME);

	private:
		std::map<ShaderName, GLuint> m_shaders;
		int m_currentStride{ -1 };
		std::string getShaderNameString(ShaderName shaderName);
	};
}

#endif