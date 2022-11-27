#pragma once
#ifndef ADVANCEDSHADERDISTRIBUTOR_H
#define ADVANCEDSHADERDISTRIBUTOR_H

#include <string>
#include <map>
#include <vector>
#include <Engine/ResourceManagement/ShaderFillType.h>

namespace Engine
{
	typedef unsigned int GLuint;
	class ShaderGenerator;

	// Based on the advanced shader source code, this generates shaders for each
	// shader fill type (check ShaderFillType.h). This means that each advanced
	// shader can be applied to different sets of vertex data such as gradients,
	// flat colours, and sprites. Above all, it makes it easier for the user
	// developing the advanced shader since the advanced shader code and the
	// default shader code are separate.
	class AdvancedShaderDistributor
	{
	public:
		AdvancedShaderDistributor(std::string advancedSource,
			std::map<ShaderFillType::FillType, ShaderGenerator>& shaders);

		//returns programId to run the shader. Note that advanced shaders are combined with each
		//default shader, so the corresponding default shader enum must be assigned.
		GLuint getAdvancedProgramId(ShaderFillType::FillType fillType = ShaderFillType::FillType::SN_TEXTURE_FILL);

	private:
		const static inline int SHADER_VERSION = 330;
		GLuint advProgramIds[ShaderFillType::types];
		std::string advancedSource;
		std::string vertexSourceSet[ShaderFillType::types], fragmentSourceSet[ShaderFillType::types];

		const std::vector<std::string> VARIABLE_TYPES
		{ "sampler2D", "bool", "int", "uint", "float", "double", "bvec2", "bvec3", "bvec4", "ivec2", "ivec3",
			"ivec4", "uvec2", "uvec3", "uvec4", "vec2", "vec3", "vec4", "dvec2", "dvec3", "dvec4",
			"mat2", "mat3", "mat4"};

		struct SourceExtractor
		{
			std::vector<std::string> layout;
			std::vector<std::string> inVariables;
			std::vector<std::string> outVariables;
			std::vector<std::string> uniforms;
			std::vector<std::string> mainCommands;
		};
		enum SEMessenger
		{
			SEM_PENDING_NONE,
			SEM_PENDING_LAYOUT,
			SEM_PENDING_IN_VARIABLES,
			SEM_PENDING_OUT_VARIABLES,
			SEM_PENDING_UNIFORMS,
			SEM_PENDING_MAIN_COMMANDS
		};
		SEMessenger pendingMessenger;

		void extractSource(std::string source, SourceExtractor& se);
		void extractAdvancedSource(std::string source, SourceExtractor& vertexSourceExtractor,
			SourceExtractor& fragmentSourceExtractor);

		//pushes back the attributes from the advanced shader to the default shader
		void compileSource(SourceExtractor& to, SourceExtractor& from);

		//pushes back the attributes from the compiled shader to a string
		void compileSource(std::string& to, SourceExtractor from);

		//splits string into list separated by \n character.
		std::vector<std::string> split(std::string source);

		//substrings the parent string to represent its data declaration given the current entry.
		//For example, if current entry == "layout (location = 0) in vec3 pos;", returns
		//"vec3 pos;" This will be the string collected by the SourceExtractor struct
		std::string substringAtDataDeclaration(std::string parent);
	};
}

#endif