#pragma once
#include <Engine/Utilities/DeltaTime.h>
#include <Engine/Core.h>
#include <Engine/Rendering/Window.h>

namespace Engine
{
	class Scene;

	class ENGINE_API Renderer
	{
	public:
		void renderScene(const DeltaTime& dt, Scene scene);
	private:
		void setColor(glm::mat4 mvp, glm::vec4 color);
		Renderer();
		Window m_Window;
		
		int m_maxBindableTextures = 32;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;

		//GL IDs for various objects. 
		GLuint m_programId;
	};
}