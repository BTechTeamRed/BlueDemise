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
		~ShaderGenerator();
		int getProgramId() const;

	private:
		int m_programId;
		int genShader(const char* source, GLenum shaderType);
		bool requestStatus(GLuint shaderObj, GLenum status);
	};
}

#endif