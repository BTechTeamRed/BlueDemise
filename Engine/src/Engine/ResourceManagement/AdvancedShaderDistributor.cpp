#pragma once
#include <Engine/ResourceManagement/AdvancedShaderDistributor.h>
#include <Engine/Utilities/Log.h>
#include <sstream>

using namespace std;

namespace Engine
{
	void AdvancedShaderDistributor::SourceExtractor::print()
	{
		GE_CORE_INFO("[layout]");
		for (int i = 0; i < layout.size(); i++)
			GE_CORE_INFO(layout[i]);
		GE_CORE_INFO("[in]");
		for (int i = 0; i < inVariables.size(); i++)
			GE_CORE_INFO(inVariables[i]);
		GE_CORE_INFO("[out]");
		for (int i = 0; i < outVariables.size(); i++)
			GE_CORE_INFO(outVariables[i]);
		GE_CORE_INFO("[uniforms]");
		for (int i = 0; i < uniforms.size(); i++)
			GE_CORE_INFO(uniforms[i]);
		GE_CORE_INFO("[main]");
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
		//extractAdvancedSource(advancedSource, advancedVertexDistributor, advancedFragmentDistributor);
	}

	void AdvancedShaderDistributor::extractSource(string source, SourceExtractor& se)
	{
		vector<string> src = split(source);
		bool mainExtracting = false;
		for (int i = 0; i < src.size(); i++)
		{
			if (mainExtracting)
			{
				se.mainCommands.push_back(src[i]);
			}
			else {
				if (strContains(src[i], "layout"))
				{
					se.layout.push_back(substringAtDataDeclaration(src[i]));
				}
				else if (strContains(src[i], "out"))
				{
					se.outVariables.push_back(substringAtDataDeclaration(src[i]));
				}
				else if (strContains(src[i], "uniform"))
				{
					se.uniforms.push_back(substringAtDataDeclaration(src[i]));
				}
				//Once the data extractions are complete, we extract main()
				else if (strContains(src[i], "main()"))
				{
					mainExtracting = true;
					se.mainCommands.push_back(src[i]);
				}
			}
		}
	}

	void AdvancedShaderDistributor::extractAdvancedSource(string source,
		SourceExtractor& vertexExtractor, SourceExtractor& fragmentExtractor)
	{
		vector<string> src = split(source);
		bool loadingVertex;
		for (int i = 0; i < src.size(); i++)
		{
			//check if assigning extractor variable for vertex of fragment
			if (strContains(src[i], "[vertex]"))
			{
				loadingVertex = true;
			}
			else if (strContains(src[i], "[fragment]"))
			{
				loadingVertex = false;
			}

			//check which source extractor variable to be assigned
			bool dataTypeAssigned = true;
			if (strContains(src[i], "[layout]"))
			{
				pendingMessenger = SEMessenger::PENDING_LAYOUT;
			}
			else if (strContains(src[i], "[in]"))
			{
				pendingMessenger = SEMessenger::PENDING_IN_VARIABLES;
			}
			else if (strContains(src[i], "[out]"))
			{
				pendingMessenger = SEMessenger::PENDING_OUT_VARIABLES;
			}
			else if (strContains(src[i], "[uniform]"))
			{
				pendingMessenger = SEMessenger::PENDING_UNIFORMS;
			}
			else if (strContains(src[i], "[main]"))
			{
				pendingMessenger = SEMessenger::PENDING_MAIN_COMMANDS;
			}
			else
			{
				pendingMessenger = SEMessenger::PENDING_NONE;
				dataTypeAssigned = false;
			}

			//if tag [] has been assigned, continue to the next line
			if (dataTypeAssigned)
			{
				continue;
			}

			//Given the above data, assign the corresponding source extractor variable
			//with the corresponding data type
			switch (pendingMessenger)
			{
			case SEMessenger::PENDING_LAYOUT:

				break;
			//if nothing is pended, don't do anything
			default:
				break;
			}
		}
	}

	vector<string> AdvancedShaderDistributor::split(string source)
	{
		stringstream ss(source);
		vector<string> list;

		string s;
		while (getline(ss, s, '\n')) {
			list.push_back(s);
		}

		return list;
	}

	bool AdvancedShaderDistributor::strContains(string parent, string substr)
	{
		return parent.find(substr) != string::npos;
	}

	string AdvancedShaderDistributor::substringAtDataDeclaration(string parent)
	{
		string keyword = "";
		for (int i = 0; i < VARIABLE_TYPES.size(); i++)
		{
			if (strContains(parent, VARIABLE_TYPES[i]))
			{
				keyword = VARIABLE_TYPES[i];
				break;
			}
		}
		if (keyword == "")
		{
			return keyword;
		}

		return parent.substr(parent.find(keyword));
	}
}