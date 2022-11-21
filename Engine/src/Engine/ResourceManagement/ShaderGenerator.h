#pragma once
#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H

namespace Engine
{
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;

	// Generates shaders that are then used for OpenGL programs 
	class ShaderGenerator
	{
	public:
		ShaderGenerator(const char* vertexSource, const char* fragmentSource);
		int getProgramId() const;
		const char* getVertexSource();
		const char* getFragmentSource();

	private:
		const char* vertexSource, * fragmentSource;
		int m_programId;
		int genShader(const char* source, GLenum shaderType);
		bool requestStatus(GLuint shaderObj, GLenum status);
	};

	/*
	// Based on the advanced shader source code, this generates shaders for each
	// shader fill type (check ShaderFillType.h). This means that each advanced
	// shader can be applied to different sets of vertex data such as gradients,
	// flat colours, and sprites. Above all, it makes it easier for the user
	// developing the advanced shader since the advanced shader code and the
	// default shader code are separate.
	class AdvancedShaderGenerator {
	public:
		AdvancedShaderGenerator(const char* vertexSource, const char* fragmentSource);
		int getProgramId(ShaderFillType::FillType fillType = ShaderFillType::DEFAULT_FILL_TYPE);

	private:
		const char* vertexSource, * fragmentSource;
		int m_programIds[ShaderFillType::types];
	};
	*/
}

#endif