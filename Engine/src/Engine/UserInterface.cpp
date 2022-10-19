#include "UserInterface.h"

ImGuiIO* UserInterface::s_IO = nullptr;
ImGuiStyle* UserInterface::s_style = nullptr;

std::map<std::string, ImFont*> UserInterface::s_fonts;

//define the ImGUI vec4s for each colour
ImVec4 UserInterface::red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
ImVec4 UserInterface::green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
ImVec4 UserInterface::blue = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 UserInterface::white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

//initializes a GLFWwindow that can be used with ImGUI
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
	auto font = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontFilename.c_str(), fontSize);

	if (!font)
	{
		return false;
	}

	s_fonts[tag] = font;

	return true;
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
void UserInterface::spacing(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		ImGui::Spacing();
	}
}

//creates the title section above the UI element
void UserInterface::partition(const std::string& fontTag, const std::string& title, const ImVec4& color)
{
	s_style->Colors[ImGuiCol_Text] = color;

	ImGui::PushFont(s_fonts[fontTag]);
	ImGui::Text(title.c_str());
	ImGui::PopFont();

	ImGui::Separator();
	spacing(1);
}
