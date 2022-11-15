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
		void renderScene(const DeltaTime& dt, Scene& scene);
		void initializeScene(Scene& scene);
		void initializeUI(Scene& scene);
		void drawEntities(Scene& scene);
		glm::mat4 Renderer::updateMVP(TransformComponent transform, glm::mat4 view, glm::mat4 projection);
	private:
		void setColor(glm::mat4 mvp, glm::vec4 color);
		Renderer();
		Window m_window;
		
		int m_maxBindableTextures = 32;

		//GL IDs for various objects. 
		GLuint m_programId;
	};
}