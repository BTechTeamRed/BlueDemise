#include "ExplorerPanel.h"
#include <filesystem>
#include <iostream>
#include <string>

namespace Engine
{
	bool ExplorerPanel::isAddButtonClicked() const
	{
		return m_isAddButtonClicked;
	}

	const std::string& ExplorerPanel::getSelectedScript() const
	{
		return m_selectedScript;
	}

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
		partition("MyriadPro_Bold_16", "Explorer", grey);

		//sets the text colour to be green
		s_style->Colors[ImGuiCol_Text] = green;

		//creates a drop down where if x is true (open), display y 
		if (ImGui::TreeNode("Scripts"))
		{
			std::string path = "Assets\\Scripts\\";

			for (const auto& entry : std::filesystem::directory_iterator(path))
			{
				auto filename = entry.path().u8string().substr(path.size());

				if (filename.find(".cpp") != std::string::npos)
				{
					//This is true if a script file is clicked on
					if (ImGui::TreeNode(filename.c_str()))
					{
						m_selectedScript = filename;
						ImGui::TreePop();
					}
				}
			}

			ImGui::TreePop();
		}

		setSpacing(3);

		if (ImGui::Button("Add script to selected entity", ImVec2(250, 25)))
		{
			m_isAddButtonClicked = true;
		}

		ImGui::End();
	}
}