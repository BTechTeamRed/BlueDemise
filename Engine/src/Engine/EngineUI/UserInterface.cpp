#include "UserInterface.h"
#include "Engine/ResourceManagement/ResourceManager.h"

namespace Engine
{
	//IO is used for loading custom fonts
	ImGuiIO* UserInterface::s_IO = nullptr;
	ImGuiStyle* UserInterface::s_style = nullptr;

	std::unordered_map<std::string, ImFont*> UserInterface::s_fonts;

	//define the ImGUI vec4s for each colour
	const ImVec4 UserInterface::LIGHT_GREY = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
	const ImVec4 UserInterface::DARK_GREY = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	const ImVec4 UserInterface::WHITE = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	const ImVec4 UserInterface::OFF_WHITE = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	const ImVec4 UserInterface::DARK_BLUE = UserInterface::ConvertColors(20, 13, 145);
	const ImVec4 UserInterface::LIGHT_BLUE = UserInterface::ConvertColors(14, 127, 231);
	const ImVec4 UserInterface::DARK_CYAN = UserInterface::ConvertColors(12, 186, 223);
	const ImVec4 UserInterface::LIGHT_CYAN = UserInterface::ConvertColors(10, 223, 217);

	//initializes UI that can be used with provided GLFWwindow
	bool UserInterface::initialize(GLFWwindow* window)
	{
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		//TODO - Reference for style
		//https://www.unknowncheats.me/forum/c-and-c-/189635-imgui-style-settings.html
		s_style = &ImGui::GetStyle();
		s_IO = &ImGui::GetIO();

		return true;
	}

	void UserInterface::shutdown()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	ImVec4 UserInterface::ConvertColors(int r, int g, int b)
	{
		return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	}

	//creates new ImGUI frame (window)
	void UserInterface::startUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void UserInterface::endUI()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	//loads a font, returns true if the font is loaded correctly
	bool UserInterface::loadFont(const std::string& fontFilename, float fontSize, const std::string& tag)
	{
		auto font = ImGui::GetIO().Fonts->AddFontFromFileTTF(ResourceManager::getInstance()->getFont(fontFilename).c_str(), fontSize);

		if (!font)
		{
			return false;
		}

		s_fonts[tag] = font;

		return true;
	}

	//sets the visibility flag so when we call show() it will appear or hide based on flag
	void UserInterface::setIsVisible(bool flag)
	{
		m_isVisible = flag;
	}

	//sets the position of a UI element
	void UserInterface::setPosition(const glm::uvec2& position)
	{
		m_position = position;
	}

	//sets the dimension of a UI element
	void UserInterface::setDimension(const glm::uvec2& dimension)
	{
		m_dimension = dimension;
	}

	void UserInterface::setButtonDimension(const glm::uvec2& dimension)
	{
		m_buttonDimension = dimension;
	}

	//defines ImGUI spacing 
	void UserInterface::setSpacing(int amount)
	{
		for (int i = 0; i < amount; i++)
		{
			ImGui::Spacing();
		}
	}

	//creates a heading section above the UI element
	void UserInterface::partition(const std::string& fontTag, const std::string& title, const ImVec4& color)
	{
		s_style->Colors[ImGuiCol_Text] = color;

		ImGui::PushFont(s_fonts[fontTag]);
		ImGui::Text(title.c_str());
		ImGui::PopFont();

		ImGui::Separator();
		setSpacing(1);
	}
}