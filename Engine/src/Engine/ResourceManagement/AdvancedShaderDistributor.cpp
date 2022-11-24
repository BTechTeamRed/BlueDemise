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
		extractAdvancedSource(advancedSource, advancedVertexDistributor, advancedFragmentDistributor);

		//load advanced shader source into default variables
		for (int i = 0; i < ShaderFillType::types; i++)
		{
			compileSource(vertexDistributor[i], advancedVertexDistributor);
			compileSource(fragmentDistributor[i], advancedFragmentDistributor);

			//load source set into string and compile
			string vertexSrc = "#version " + to_string(SHADER_VERSION) + "\n\n",
				fragmentSrc = vertexSrc;
			compileSource(vertexSrc, vertexDistributor[i]);
			compileSource(fragmentSrc, fragmentDistributor[i]);

			advProgramIds[i] = ShaderGenerator(vertexSrc, fragmentSrc).getProgramId();
		}
	}

	GLuint AdvancedShaderDistributor::getAdvancedProgramId(ShaderFillType::FillType fillType)
	{
		switch (fillType)
		{
		case ShaderFillType::SN_TEXTURE_FILL:
			return advProgramIds[0];
			break;
		case ShaderFillType::SN_COLOR_FILL:
			return advProgramIds[1];
			break;
		default:
			return advProgramIds[2];
			break;
		}
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
				else if(strContains(src[i], "in")) {
					se.inVariables.push_back(substringAtDataDeclaration(src[i]));
				}
			}
		}
	}

	void AdvancedShaderDistributor::extractAdvancedSource(string source,
		SourceExtractor& vertexExtractor, SourceExtractor& fragmentExtractor)
	{
		vector<string> src = split(source);
		bool loadingVertex, messengerAssigned = false;
		for (int i = 0; i < src.size(); i++)
		{
			//check if assigning extractor variable for vertex of fragment
			if (strContains(src[i], "[vertex]"))
			{
				loadingVertex = true;
				pendingMessenger = SEMessenger::PENDING_NONE;
				continue;
			}
			else if (strContains(src[i], "[fragment]"))
			{
				loadingVertex = false;
				pendingMessenger = SEMessenger::PENDING_NONE;
				continue;
			}

			//check which source extractor variable to be assigned
			if (strContains(src[i], "[layout]"))
			{
				pendingMessenger = SEMessenger::PENDING_LAYOUT;
				messengerAssigned = true;
			}
			else if (strContains(src[i], "[in]"))
			{
				pendingMessenger = SEMessenger::PENDING_IN_VARIABLES;
				messengerAssigned = true;
			}
			else if (strContains(src[i], "[out]"))
			{
				pendingMessenger = SEMessenger::PENDING_OUT_VARIABLES;
				messengerAssigned = true;
			}
			else if (strContains(src[i], "[uniform]"))
			{
				pendingMessenger = SEMessenger::PENDING_UNIFORMS;
				messengerAssigned = true;
			}
			else if (strContains(src[i], "[main]"))
			{
				pendingMessenger = SEMessenger::PENDING_MAIN_COMMANDS;
				messengerAssigned = true;
			}

			//if messenger pending new tag, continue to next line
			if (messengerAssigned)
			{
				messengerAssigned = false;
				continue;
			}

			//Given the above data, assign the corresponding source extractor variable
			//with the corresponding data type
			SourceExtractor* se = ((loadingVertex) ? &vertexExtractor : &fragmentExtractor);
			switch (pendingMessenger)
			{
			case SEMessenger::PENDING_LAYOUT:
				se->layout.push_back(src[i]);
				break;
			case SEMessenger::PENDING_IN_VARIABLES:
				se->inVariables.push_back(src[i]);
				break;
			case SEMessenger::PENDING_OUT_VARIABLES:
				se->outVariables.push_back(src[i]);
				break;
			case SEMessenger::PENDING_UNIFORMS:
				se->uniforms.push_back(src[i]);
				break;
			case SEMessenger::PENDING_MAIN_COMMANDS:
				se->mainCommands.push_back(src[i]);
				break;
			default:
				//if nothing is pended, don't do anything
				break;
			}
		}
	}

	void AdvancedShaderDistributor::compileSource(SourceExtractor& to, SourceExtractor& from)
	{
		for (int i = 0; i < from.layout.size(); i++)
		{
			to.layout.push_back(from.layout[i]);
		}
		for (int i = 0; i < from.inVariables.size(); i++)
		{
			to.inVariables.push_back(from.inVariables[i]);
		}
		for (int i = 0; i < from.outVariables.size(); i++)
		{
			to.outVariables.push_back(from.outVariables[i]);
		}
		for (int i = 0; i < from.uniforms.size(); i++)
		{
			to.uniforms.push_back(from.uniforms[i]);
		}
		//pop ending } of main
		to.mainCommands.pop_back();
		for (int i = 0; i < from.mainCommands.size(); i++)
		{
			to.mainCommands.push_back("\t" + from.mainCommands[i]);
		}
		//reinsert }
		to.mainCommands.push_back("}");
	}

	void AdvancedShaderDistributor::compileSource(std::string& to, SourceExtractor from)
	{
		for (int i = 0; i < from.layout.size(); i++)
		{
			to += "layout (location = " + to_string(i) + ") in " + from.layout[i] + "\n";
		}
		for (int i = 0; i < from.inVariables.size(); i++)
		{
			to += "in " + from.inVariables[i] + "\n";
		}
		for (int i = 0; i < from.outVariables.size(); i++)
		{
			to += "out " + from.outVariables[i] + "\n";
		}
		for (int i = 0; i < from.uniforms.size(); i++)
		{
			to += "uniform " + from.uniforms[i] + "\n";
		}
		for (int i = 0; i < from.mainCommands.size(); i++)
		{
			to += from.mainCommands[i] + ((i < from.mainCommands.size() - 1) ? "\n" : "");
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