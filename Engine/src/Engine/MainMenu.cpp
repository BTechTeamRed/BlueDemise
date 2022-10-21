#include "MainMenu.h"

namespace Engine
{
	void MainMenu::show()
	{
		if (ImGui::BeginMainMenuBar())
		{
			//sets the font colour to be white
			s_style->Colors[ImGuiCol_Text] = white;

			//Do not use else-if statements here otherwise
			//the menu titles will not render correctly
			//creates a drop down menu where if x is true (open), display y
			if (ImGui::BeginMenu("File"))
			{
				//first parameter is label, second is shortcut, third is a bool for selected
				//They are nullptr because we don't use them yet
				if (ImGui::MenuItem("New", nullptr, nullptr))
				{
					//Handle menu item...
				}

				if (ImGui::MenuItem("Load...", nullptr, nullptr))
				{
					//Handle menu item...
				}

				if (ImGui::MenuItem("Save", nullptr, nullptr))
				{
					//Handle menu item...
				}

				if (ImGui::MenuItem("Save As...", nullptr, nullptr))
				{
					//Handle menu item...
				}

				if (ImGui::MenuItem("Reset", nullptr, nullptr))
				{
					//Handle menu item...
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", nullptr, nullptr))
				{
					//Handle menu item...
				}

				ImGui::EndMenu();
			}

			//separate drop down menu for scene options
			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Play", nullptr, nullptr))
				{
					//Handle menu item...
				}

				if (ImGui::MenuItem("Pause", nullptr, nullptr))
				{
					//Handle menu item...
				}

				if (ImGui::MenuItem("Stop", nullptr, nullptr))
				{
					//Handle menu item...
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}