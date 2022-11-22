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
		struct SourceExtractor
		{
			std::vector<std::string> layout;
			std::vector<std::string> uniforms;
			std::vector<std::string> inVariables;
			std::vector<std::string> outVariables;
			std::vector<std::string> mainCommands;

			bool layoutAvailable, uniformAvailable, inVariablesAvailable,
				outVariablesAvailable, mainCommandsAvailable;
			void print();
		};

		void extractSource(std::string source, SourceExtractor& se);
		void extractAdvancedSource(std::string source, SourceExtractor& vertexSourceExtractor,
			SourceExtractor& fragmentSourceExtractor);
	};
}

#endif