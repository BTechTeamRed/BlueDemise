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
		/*
		struct SourceExtractor
		{
			vector<std::string> layout;
			vector<std::string> uniform;
			vector<std::string> inVariables;
			vector<std::string> outVariables;
			vector<std::string> mainCommands;
		};
		*/

		//void extractSource(SourceExtractor& se, std::string source);
	};
}

#endif