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
		//shader tokens to access map of shaders
		static enum ShaderName {
			SN_TEXTURE_FILL,
			SN_COLOR_FILL
		};
		static const ShaderName DEFAULT_SHADER_NAME;

		//requires the stride of the default shader's vertices component
		ShaderNorms(int defaultStride);
		~ShaderNorms();
		void addShader(ShaderName shaderName);
		//to be tested per render to detect any change in the vertices component's stride
		bool assignsNewStride(int stride);
		//access the shader given key value
		GLuint getShader(ShaderName = DEFAULT_SHADER_NAME);

	private:
		std::map<ShaderName, GLuint> m_shaders;
		//used to detect any change in stride
		int m_currentStride{ -1 };
		//converts the shader name enum value into string
		std::string getShaderNameString(ShaderName shaderName);
	};
}

#endif