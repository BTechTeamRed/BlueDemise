#pragma once
#include <Engine/ResourceManagement/ShaderFillType.h>
#include <Engine/ResourceManagement/AdvancedShaderDistributor.h>
#include <Engine/ResourceManagement/ShaderGenerator.h>
#include <Engine/Utilities/BasicUtilities.h>
#include <sstream>

using namespace std;
using namespace Engine::Utilities;

namespace Engine
{
	AdvancedShaderDistributor::AdvancedShaderDistributor(string advancedSource,
		ShaderGenerator& defaultShader)
	{
		string vertexSourceSet = defaultShader.getVertexSource(),
			fragmentSourceSet = defaultShader.getFragmentSource();

		//load source into shader variables
		SourceExtractor vertexDistributor, fragmentDistributor,
			advancedVertexDistributor, advancedFragmentDistributor;

		//extract default shaders
		extractSource(vertexSourceSet, vertexDistributor);
		extractSource(fragmentSourceSet, fragmentDistributor);

		//extract advanced shader source
		extractAdvancedSource(advancedSource, advancedVertexDistributor, advancedFragmentDistributor);

		//load advanced shader source into default variables
		compileSource(vertexDistributor, advancedVertexDistributor);
		compileSource(fragmentDistributor, advancedFragmentDistributor);

		//load source set into string and compile
		string vertexSrc = "#version " + to_string(SHADER_VERSION) + "\n\n",
			fragmentSrc = vertexSrc;
		compileSource(vertexSrc, vertexDistributor);
		compileSource(fragmentSrc, fragmentDistributor);

		advProgramId = ShaderGenerator(vertexSrc, fragmentSrc).getProgramId();
	}

	GLuint AdvancedShaderDistributor::getAdvancedProgramId()
	{
		return advProgramId;
	}

	void AdvancedShaderDistributor::extractSource(string source, SourceExtractor& se)
	{
		vector<string> src = strSplit(source);
		bool mainExtracting = false;
		for (int i = 0; i < src.size(); i++)
		{
			if (mainExtracting)
			{
				se.mainCommands.push_back(src[i]);
			}
			else
			{
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
				else if (strContains(src[i], "in"))
				{
					se.inVariables.push_back(substringAtDataDeclaration(src[i]));
				}
			}
		}
	}

	void AdvancedShaderDistributor::extractAdvancedSource(string source,
		SourceExtractor& vertexExtractor, SourceExtractor& fragmentExtractor)
	{
		vector<string> src = strSplit(source);
		bool loadingVertex = true, messengerAssigned = false;
		for (int i = 0; i < src.size(); i++)
		{
			//check if assigning extractor variable for vertex of fragment
			if (strContains(src[i], "[vertex]"))
			{
				loadingVertex = true;
				pendingMessenger = SEMessenger::SEM_PENDING_NONE;
				continue;
			}
			else if (strContains(src[i], "[fragment]"))
			{
				loadingVertex = false;
				pendingMessenger = SEMessenger::SEM_PENDING_NONE;
				continue;
			}

			//check which source extractor variable to be assigned
			if (strContains(src[i], "[layout]"))
			{
				pendingMessenger = SEMessenger::SEM_PENDING_LAYOUT;
				messengerAssigned = true;
			}
			else if (strContains(src[i], "[in]"))
			{
				pendingMessenger = SEMessenger::SEM_PENDING_IN_VARIABLES;
				messengerAssigned = true;
			}
			else if (strContains(src[i], "[out]"))
			{
				pendingMessenger = SEMessenger::SEM_PENDING_OUT_VARIABLES;
				messengerAssigned = true;
			}
			else if (strContains(src[i], "[uniform]"))
			{
				pendingMessenger = SEMessenger::SEM_PENDING_UNIFORMS;
				messengerAssigned = true;
			}
			else if (strContains(src[i], "[main]"))
			{
				pendingMessenger = SEMessenger::SEM_PENDING_MAIN_COMMANDS;
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
			SourceExtractor* se = (loadingVertex ? &vertexExtractor : &fragmentExtractor);
			switch (pendingMessenger)
			{
			case SEMessenger::SEM_PENDING_LAYOUT:
				se->layout.push_back(src[i]);
				break;
			case SEMessenger::SEM_PENDING_IN_VARIABLES:
				se->inVariables.push_back(src[i]);
				break;
			case SEMessenger::SEM_PENDING_OUT_VARIABLES:
				se->outVariables.push_back(src[i]);
				break;
			case SEMessenger::SEM_PENDING_UNIFORMS:
				se->uniforms.push_back(src[i]);
				break;
			case SEMessenger::SEM_PENDING_MAIN_COMMANDS:
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

	//Given line of code, gathers the data type and variable name i.e. mat4 mvp;
	//from layout(location=0) in mat4 mvp;
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

		return ((keyword != "") ? parent.substr(parent.find(keyword)) : keyword);
	}
}