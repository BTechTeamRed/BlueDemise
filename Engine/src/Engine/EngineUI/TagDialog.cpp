#include "TagDialog.h"

namespace Engine
{
	TagDialog::TagDialog()
	{
		m_dimension.x = 275;
		m_dimension.y = 125;

		m_buttonDimension.x = 50;
		m_buttonDimension.y = 20;
	}

	const std::string& TagDialog::getTag() const
	{
		return m_tag;
	}

	const TagDialog::ButtonState& TagDialog::getButtonState() const
	{
		return m_buttonState;
	}

	void TagDialog::update()
	{
		m_buttonState = { false, false };

		if (!m_isVisible)
		{
			m_tag.clear();
		}
	}

	void TagDialog::show(unsigned int fbo)
	{
		if (m_isVisible)
		{
			ImGui::Begin("TagDialog", nullptr,
				ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

			ImGui::SetWindowSize("TagDialog", ImVec2(m_dimension.x, m_dimension.y));

			ImGui::Text("Enter a tag:");

			char regText[20];
			strcpy_s(regText, m_tag.c_str());
			ImGui::InputText(" ", regText, 20);
			m_tag = regText;

			setSpacing(3);
			ImGui::Indent(0.5f * m_dimension.x - (0.5f * m_buttonDimension.x));

			if (ImGui::Button("Okay", ImVec2(m_buttonDimension.x, m_buttonDimension.y)))
			{
				m_buttonState.okay = true;
				m_isVisible = false;
			}

			if (ImGui::Button("Cancel", ImVec2(m_buttonDimension.x, m_buttonDimension.y)))
			{
				m_buttonState.cancel = true;
				m_isVisible = false;
			}

			ImGui::End();
		}
	}
}
