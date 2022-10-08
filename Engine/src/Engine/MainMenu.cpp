#include "MainMenu.h"

void MainMenu::show()
{
	if (ImGui::BeginMainMenuBar())
	{
		s_style->Colors[ImGuiCol_Text] = white;

		//Do not use else-if statements here otherwise
		//the menu titles will not render correctly
		if (ImGui::BeginMenu("File"))
		{
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
