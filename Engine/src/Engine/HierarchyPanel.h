#pragma once
#include "Engine/EngineUI/UserInterface.h"

namespace Engine
{
	class HierarchyPanel : public UserInterface
	{

	public:

		virtual ~HierarchyPanel() {}

		bool isAddButtonClicked() const;

		const std::string& getSelectedEntity() const;

		void addEntity(const std::string entity);

		virtual void show(unsigned int fbo = 0) override;

	private:

		bool m_isAddButtonClicked{ false };

		std::string m_selectedEntity;

		std::vector<std::string> m_entities;

	};
}