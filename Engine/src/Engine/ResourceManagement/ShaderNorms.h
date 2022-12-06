#pragma once
#ifndef SHADERNORMS_H
#define SHADERNORMS_H

#include <Engine/ResourceManagement/ShaderFillType.h>
#include <string>
#include <vector>
#include <map>

namespace Engine
{
	typedef unsigned int GLuint;
	class ShaderGenerator;
	class AdvancedShaderDistributor;

	class ShaderNorms
	{
	public:
		//any time-related uniform in the advanced shader
		const static inline float TIME_SPEEDUP_MOD = 3.f;

		//requires the stride of the default shader's vertices component
		~ShaderNorms();
		ShaderNorms(ShaderNorms& other) = delete;
		void operator=(const ShaderNorms&) = delete;
		static ShaderNorms* getInstance();

		//sets the uniform value of a float
		void setUniformValuef(int& bind, std::string uniformName, float value);

		//sets the uniform value of an int
		void setUniformValuei(int& bind, std::string uniformName, int value);

		//sets the uniform value of a bool
		void setUniformValueb(int& bind, std::string uniformName, bool value);

		//load shader according to vertices component (given shader is not already loaded)
		void update(double deltaTime, int advancedShaderBind, int stride, int& textureCoordinates,
			int& colorCoordinates, int& gradientCoordinates, GLuint& programId, bool firstPass);

		//loads default shader program based on stride (used only for defaults)
		void assignOnStride(int stride, int& textureCoordinates, int& colorCoordinates,
			int& gradientCoordinates, GLuint& programId);

		//to be tested per render to detect any change in the vertices component's stride
		bool assignsNewStride(int stride);

		//add shader to the shader map
		void addShader(ShaderFillType::FillType shaderName);
		void addAdvancedShader(int bind, std::string& shaderName);

		//check timeCount
		void resetTimeCount();

		//access the shader given key value
		GLuint getShaderReference(ShaderFillType::FillType fillType = ShaderFillType::DEFAULT_FILL_TYPE);
		GLuint getAdvancedShaderReference(int bind);

		//access to the list of shader names
		std::vector<std::string>& getShaderNames();

	private:
		ShaderNorms();
		static ShaderNorms* m_singleton;

		//accessed in update as a counter variable for time
		double m_timeCount = 0;

		//shader generator is stored by default shaders map for further use by adv. shaders
		std::map<ShaderFillType::FillType, ShaderGenerator> m_shaders;
		std::map<int, AdvancedShaderDistributor> m_advancedShaders;

		//store information for reference by shader name
		std::map<std::string, int> m_advancedShaderBinds;
		std::vector<std::string> m_advancedShaderNames
		{
			"SinWave"
		};

		//used to detect any change in stride
		int m_currentStride{ -1 }, m_currentAdvancedShaderBind{ -1 };

		//assigns program to GL and the accessor variable
		void assignProgram(GLuint shaderBind, GLuint& shaderAccessor);

		//converts the shader name enum value into string
		std::string getShaderNameString(ShaderFillType::FillType shaderName);
	};
}

#endif