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
		
		#pragma region Singleton Instance Management
		//Singletons should not be cloneable, this is to prevent clones.
		Renderer(Renderer& other) = delete;

		//Singletons should not be assignable, this is to prevent that.
		void operator=(const Renderer&) = delete;

		//This retrieves a pointer to the current instance of ResourceManager. If it doesn't exist, then one will be created and returned.
		static Renderer* getInstance();
		#pragma endregion

	private:
		Renderer();
		~Renderer() {};
		
		//Renderer is a singleton, so this is the only instance of it.
		static Renderer* m_pinstance;

		//Define window that is managed by this singleton
		Window m_window;

		//Define the max number of bindable textures (31 in this case, as one is used for the Frame Buffer Object)
		int m_maxBindableTextures = 31;

		//programID stores the current used shader (Should be changed in the future when multiple shaders are utilized. Perhaps this will be the 'default' shader. *************
		GLuint m_programId;
		//Load the default shader in, and store it to m_programId. Should be changed when multiple shaders are utilized. **********************
		void loadShaders();

		//Draw entities with a VerticesComponent to screen/FBO
		void drawEntities(Scene& scene);

		//Set color of screen using the current shader, and provided mvp and color.
		void setColor(glm::mat4 mvp, glm::vec4 color, GLuint shaderID);

		//Set texture for current verts, and provide the textureID and current number of bound textures
		bool setTexture(GLuint textureID, int currentBoundTextures);

		//Update the ModelViewProjection matrix of the frame
		glm::mat4 Renderer::updateMVP(TransformComponent transform, glm::mat4 projection);
	};
}