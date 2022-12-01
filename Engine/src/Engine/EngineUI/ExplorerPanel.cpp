#include "ExplorerPanel.h"
#include <filesystem>
#include <iostream>
#include <string>

namespace Engine
{
	bool ExplorerPanel::isAddButtonClicked() 
	{
		bool isClicked = m_isAddButtonClicked;
		m_isAddButtonClicked = false;
		return isClicked;
	}

	const std::string& ExplorerPanel::getSelectedScript() const
	{
		return m_selectedScript;
	}

	//struct for radio buttons for scripts
	struct ScriptButton
	{
		bool state; // true if selected
		std::string tag; // script name
	};

	void ExplorerPanel::show()
	{
		ImGui::Begin("ExplorerPanel", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		//sets the position and size of the UI element
		static bool isPositionSet = false;
		if (!isPositionSet)
		{
			ImGui::SetWindowPos("ExplorerPanel", ImVec2(m_position.x, m_position.y));
			isPositionSet = true;
		}

		ImGui::SetWindowSize("ExplorerPanel", ImVec2(m_dimension.x, m_dimension.y));

		//Need some .otf/.ttf font files
		//defines the title section above the UI element
		partition("MyriadPro_bold_18", "Explorer", darkCyan);

		ImGui::PushFont(s_fonts["MyriadPro_14"]);
		s_style->Colors[ImGuiCol_Text] = offWhite;

		//creates a drop down where if x is true (open), display y 
		if (ImGui::TreeNode("Scripts"))
		{
			std::string path = "Assets\\Scripts\\";

			for (const auto& entry : std::filesystem::directory_iterator(path))
			{
				auto filename = entry.path().u8string().substr(path.size());

				if (filename.find(".cpp") != std::string::npos)
				{
					// Initializes ScriptButton struct for each scritp
					ScriptButton Script;
					Script.tag = filename;
					Script.state = false;

					// if this script is selected, fill in the radio button
					if (Script.tag == m_selectedScript)
					{
						Script.state = true;
					}

					if (ImGui::RadioButton(Script.tag.c_str(), Script.state))
					{
						m_selectedScript = Script.tag;
					}
				}
			}

			ImGui::TreePop();
		}

		ImGui::PopFont();

		setSpacing(3);

		ImGui::PushFont(s_fonts["MyriadPro_bold_14"]);
		s_style->Colors[ImGuiCol_Text] = white;

		if (ImGui::Button("Add script to selected entity", ImVec2(250, 25)))
		{
			m_isAddButtonClicked = true;
		}


		ImGui::PopFont();
		ImGui::End();
	}
}