#pragma once
#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace input {
	class ShaderGenerator {
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