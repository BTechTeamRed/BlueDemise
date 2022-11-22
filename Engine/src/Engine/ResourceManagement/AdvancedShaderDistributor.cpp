#pragma once
#include <Engine/ResourceManagement/AdvancedShaderDistributor.h>

using namespace std;

namespace Engine
{
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

		/*
		//load advanced source into shader variables
		SourceExtractor vertexDistributor[ShaderFillType::types], fragmentDistributor[ShaderFillType::types];
		SourceExtractor advancedVertexDistributor, advancedFragmentDistributor;
		*/
	}

	/*
	void AdvancedShaderDistributor::extractSource(SourceExtractor& se, string source)
	{

	}
	*/
}