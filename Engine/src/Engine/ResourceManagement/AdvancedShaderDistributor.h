#pragma once
#ifndef ADVANCEDSHADERDISTRIBUTOR_H
#define ADVANCEDSHADERDISTRIBUTOR_H

#include <string>
#include <map>
#include <vector>
#include <Engine/ResourceManagement/ShaderFillType.h>
#include <Engine/ResourceManagement/ShaderGenerator.h>

namespace Engine
{
	class AdvancedShaderDistributor
	{
	public:
		AdvancedShaderDistributor(std::string advancedSource,
			std::map<ShaderFillType::FillType, ShaderGenerator>& shaders);

	private:
		std::string advancedSource;
		std::string vertexSourceSet[ShaderFillType::types], fragmentSourceSet[ShaderFillType::types];
		const std::vector<std::string> VARIABLE_TYPES
		{ "bool", "int", "uint", "float", "double", "bvec2", "bvec3", "bvec4", "ivec2", "ivec3",
			"ivec4", "uvec2", "uvec3", "uvec4", "vec2", "vec3", "vec4", "dvec2", "dvec3", "dvec4",
			"mat2", "mat3", "mat4" };
		struct SourceExtractor
		{
			std::vector<std::string> layout;
			std::vector<std::string> inVariables;
			std::vector<std::string> outVariables;
			std::vector<std::string> uniforms;
			std::vector<std::string> mainCommands;

			//Prints the variables. Only used for testing purposes
			void print();
		};
		enum SEMessenger
		{
			PENDING_NONE,
			PENDING_LAYOUT,
			PENDING_IN_VARIABLES,
			PENDING_OUT_VARIABLES,
			PENDING_UNIFORMS,
			PENDING_MAIN_COMMANDS
		};
		SEMessenger pendingMessenger = SEMessenger::PENDING_NONE;

		void extractSource(std::string source, SourceExtractor& se);
		void extractAdvancedSource(std::string source, SourceExtractor& vertexSourceExtractor,
			SourceExtractor& fragmentSourceExtractor);
		//splits string into list separated by \n character.
		std::vector<std::string> split(std::string source);
		//returns true if the string contains a substring value
		bool strContains(std::string parent, std::string substr);
		//substrings the parent string to represent its data declaration given the current entry.
		//For example, if current entry == "layout (location = 0) in vec3 pos;", returns
		//"vec3 pos;" This will be the string collected by the SourceExtractor struct
		std::string substringAtDataDeclaration(std::string parent);
	};
}

#endif