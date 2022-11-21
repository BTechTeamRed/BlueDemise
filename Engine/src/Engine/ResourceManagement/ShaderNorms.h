#pragma once
#ifndef SHADERNORMS_H
#define SHADERNORMS_H

#include <Engine/ResourceManagement/ShaderFillType.h>
#include <Engine/ResourceManagement/ShaderGenerator.h>
#include <string>
#include <map>

namespace Engine
{
	typedef unsigned int GLuint;

	class ShaderNorms
	{
	public:
		//requires the stride of the default shader's vertices component
		~ShaderNorms();
		ShaderNorms(ShaderNorms& other) = delete;
		void operator=(const ShaderNorms&) = delete;
		static ShaderNorms* getInstance();
		//load shader according to vertices component (given shader is not already loaded)
		void update(int stride, int& textureCoordinates, int& colorCoordinates,
			int& gradientCoordinates, GLuint& programId);
		//to be tested per render to detect any change in the vertices component's stride
		bool assignsNewStride(int stride);
		//add shader to the shader map
		void addShader(ShaderFillType::FillType shaderName);
		//access the shader given key value
		GLuint getShaderReference(ShaderFillType::FillType fillType = ShaderFillType::DEFAULT_FILL_TYPE);

	private:
		ShaderNorms();
		static ShaderNorms* m_singleton;
		//shader generator is stored by default shaders map for further use by adv. shaders
		std::map<ShaderFillType::FillType, ShaderGenerator> m_shaders;
		//std::map<int, AdvancedShaderGenerator> m_advancedShaders;//TODO
		//used to detect any change in stride
		int m_currentStride{ -1 };
		//converts the shader name enum value into string
		std::string getShaderNameString(ShaderFillType::FillType shaderName);
	};
}

#endif