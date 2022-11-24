#pragma once
#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H

#include <string>

namespace Engine
{
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;

	// Generates shaders that are then used for OpenGL programs 
	class ShaderGenerator
	{
	public:
		ShaderGenerator(std::string vertexSource, std::string fragmentSource);
		int getProgramId() const;
		std::string getVertexSource();
		std::string getFragmentSource();

	private:
		std::string vertexSource, fragmentSource;
		int m_programId;
		int genShader(const char* source, GLenum shaderType);
		bool requestStatus(GLuint shaderObj, GLenum status);
	};
}

#endif