#pragma once

#include "Window.h"

namespace Engine {
	class Renderer
	{
		glm::mat4 updateMVP(TransformComponent transform, glm::mat4 view, glm::mat4 projection);
	private:
		Window m_window;
	};
}
