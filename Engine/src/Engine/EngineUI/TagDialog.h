#pragma once

#include "Engine/EngineUI/UserInterface.h"

namespace Engine
{
	class TagDialog : public UserInterface
	{
	public:

		struct ButtonState
		{
			bool okay = false;
			bool cancel = false;
		};

		TagDialog();

		const std::string& getTag() const;
		const ButtonState& getButtonState() const;

		virtual void update() override;
		virtual void show(unsigned int fbo = 0) override;

	private:

		std::string m_tag;
		ButtonState m_buttonState;
	};
}
