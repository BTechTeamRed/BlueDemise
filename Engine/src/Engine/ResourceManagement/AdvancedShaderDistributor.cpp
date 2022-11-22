#pragma once
#include <Engine/ResourceManagement/AdvancedShaderDistributor.h>
#include <Engine/Utilities/Log.h>

using namespace std;

namespace Engine
{
	void AdvancedShaderDistributor::SourceExtractor::print()
	{
		GE_CORE_INFO("[layout]");
		for (int i = 0; i < layout.size(); i++)
			GE_CORE_INFO(layout[i]);
		for (int i = 0; i < uniforms.size(); i++)
			GE_CORE_INFO(uniforms[i]);
		for (int i = 0; i < inVariables.size(); i++)
			GE_CORE_INFO(inVariables[i]);
		for (int i = 0; i < outVariables.size(); i++)
			GE_CORE_INFO(outVariables[i]);
		for (int i = 0; i < mainCommands.size(); i++)
			GE_CORE_INFO(mainCommands[i]);
	}

	AdvancedShaderDistributor::AdvancedShaderDistributor(string advancedSource,
		std::map<ShaderFillType::FillType, ShaderGenerator>& shaders) :
		advancedSource(advancedSource)

	{
		//extract source from default shaders
		vertexSourceSet[0] = shaders.at(ShaderFillType::FillType::SN_TEXTURE_FILL).getVertexSource();
		vertexSourceSet[1] = shaders.at(ShaderFillType::FillType::SN_COLOR_FILL).getVertexSource();
		vertexSourceSet[2] = shaders.at(ShaderFillType::FillType::SN_GRADIENT_FILL).getVertexSource();

		fragmentSourceSet[0] = shaders.at(ShaderFillType::FillType::SN_TEXTURE_FILL).getFragmentSource();
		fragmentSourceSet[1] = shaders.at(ShaderFillType::FillType::SN_COLOR_FILL).getFragmentSource();
		fragmentSourceSet[2] = shaders.at(ShaderFillType::FillType::SN_GRADIENT_FILL).getFragmentSource();

		//load source into shader variables
		SourceExtractor vertexDistributor[ShaderFillType::types], fragmentDistributor[ShaderFillType::types];
		SourceExtractor advancedVertexDistributor, advancedFragmentDistributor;

		//extract default shaders
		for (int i = 0; i < ShaderFillType::types; i++)
		{
			extractSource(vertexSourceSet[i], vertexDistributor[i]);
			extractSource(fragmentSourceSet[i], fragmentDistributor[i]);
		}

		//extract advanced shader source
		extractAdvancedSource(advancedSource, advancedVertexDistributor, advancedFragmentDistributor);
	}

	void AdvancedShaderDistributor::extractSource(string source, SourceExtractor& se)
	{
		source.find("[layout]");
		GE_CORE_INFO("layout location: " + source.find("[layout]"));

		GE_CORE_INFO("Source:");
		GE_CORE_INFO(source);
	}

	void AdvancedShaderDistributor::extractAdvancedSource(string source,
		SourceExtractor& vertexExtractor, SourceExtractor& fragmentExtractor)
	{
		GE_CORE_INFO("Advanced Source:");
		GE_CORE_INFO(source);
	}
}