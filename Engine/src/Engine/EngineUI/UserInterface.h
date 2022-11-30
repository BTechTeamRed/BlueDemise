#pragma once

#include <unordered_map>
#include <string>

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "glm/glm.hpp"

namespace Engine
{
	class UserInterface
	{

	public:

		//initializes ImGUI vec4s to be used for colours
		static ImVec4 darkGrey;
		static ImVec4 lightGrey;
		static ImVec4 white;
		static ImVec4 offWhite;
		static ImVec4 darkBlue;
		static ImVec4 lightBlue;
		static ImVec4 darkCyan;
		static ImVec4 lightCyan;

		static ImVec4 ConvertColors(int r, int g, int b);

		//initialize start and end functions
		static void startUI();
		static void endUI();

		//loadFont takes in a string file name, float font size, and string tag and returns true if the font is valid
		static bool loadFont(const std::string& fontFilename, float fontSize, const std::string& tag);

		virtual ~UserInterface() = 0 {};

		//initialize takes in a GLFWwindow as a parameter and initializes a window to be used with ImGUI
		static bool initialize(GLFWwindow* window);
		static void shutdown();

		//set position takes in a vec2 and is used to set the position of the window
		void setIsVisible(bool flag);
		void setPosition(const glm::uvec2& position);
		void setDimension(const glm::uvec2& dimension);
		void setButtonDimension(const glm::uvec2& dimension);

		void setSpacing(int amount);

		//partition defines the title section of the UI element
		void partition(const std::string& fontTag, const std::string& title = "", const ImVec4& color = white);

		virtual void update() {}
		virtual void show() {}

	protected:

		static ImGuiIO* s_IO;
		static ImGuiStyle* s_style;

		//Changed to unordered_map for efficiency
		static std::unordered_map<std::string, ImFont*> s_fonts;

		bool m_isVisible{ false };

		glm::vec2 m_position{ 0.0f, 0.0f };
		glm::vec2 m_dimension{ 0.0f, 0.0f };
		glm::vec2 m_buttonDimension{ 0.0f, 0.0f };

	};
}