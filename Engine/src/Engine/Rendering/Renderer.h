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
		void stopScene(Scene& scene);
		
		void drawEntities(Scene& scene);
		
		glm::mat4 Renderer::updateMVP(TransformComponent transform, glm::mat4 projection);

		#pragma region Singleton Instance Management
		//Singletons should not be cloneable, this is to prevent clones.
		Renderer(Renderer& other) = delete;

		//Singletons should not be assignable, this is to prevent that.
		void operator=(const Renderer&) = delete;

		//This retrieves a pointer to the current instance of ResourceManager. If it doesn't exist, then one will be created and returned.
		static Renderer* getInstance();
		#pragma endregion

	private:
		//static lock m_mutex is the lock to be used for static functions. Otherwise, m_functionlock should be utilized.
		//static std::mutex m_mutex;

		Renderer();
		~Renderer() {};
		
		void setColor(glm::mat4 mvp, glm::vec4 color);
		bool setTexture(GLuint textureID, int currentBoundTextures);
		
		Window m_window;
		
		int m_maxBindableTextures = 32;

		static Renderer* m_pinstance;
		GLuint loadShaders();

		//programID stores the current used shader.
		GLuint m_programId;
	};
}