#pragma once

#include <map>
#include <string>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "glm/glm.hpp"

class UserInterface
{

public:

	static ImVec4 red;
	static ImVec4 green;
	static ImVec4 blue;
	static ImVec4 white;

	static void startUI();
	static void endUI();

	static bool loadFont(const std::string& fontFilename, float fontSize, const std::string& tag);

	virtual ~UserInterface() = 0 {}

	static bool initialize(GLFWwindow* window);

	void setPosition(const glm::uvec2& position);
	void setDimension(const glm::uvec2& dimension);
	void setButtonDimension(const glm::uvec2& dimension);

	void spacing(int amount);
	void partition(const std::string& fontTag, const std::string& title = "", const ImVec4& color = white);

	virtual void show() = 0;
	
	void shutdown();

protected:

	static ImGuiIO* s_IO;
	static ImGuiStyle* s_style;
	static std::map<std::string, ImFont*> s_fonts;

	glm::vec2 m_position{ 0.0f, 0.0f };
	glm::vec2 m_dimension{ 0.0f, 0.0f };
	glm::vec2 m_buttonDimension{ 0.0f, 0.0f };

};