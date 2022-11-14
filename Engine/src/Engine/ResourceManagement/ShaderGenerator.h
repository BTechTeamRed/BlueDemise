#pragma once
#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H

#include "Engine/ResourceManagement/ResourceManager.h"
#include <string>
#include <map>

using namespace std;

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

	private:
		int m_programId;
		int genShader(const char* source, GLenum shaderType);
		bool requestStatus(GLuint shaderObj, GLenum status);
	};

	class ShaderNorms {
	public:
		ShaderNorms(string);
		~ShaderNorms();
		void addShader(string);
		GLuint getShader(string);
		GLuint getDefaultShader();

	private:
		map<string, GLuint> m_shaders;
		string m_defaultShader;
	};
}

#endif