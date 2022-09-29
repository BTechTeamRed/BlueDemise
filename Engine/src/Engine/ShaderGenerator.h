#pragma once
#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H
namespace input
{
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;
	class ShaderGenerator
	{
		int m_programId;
		int genShader(const char* source, GLenum shaderType);
		int requestStatus(GLuint shaderObj, GLenum status);

	public:
		ShaderGenerator(const char* vertexSource, const char* fragmentSource);
		~ShaderGenerator();
		int getProgramId();
	};
}

#endif