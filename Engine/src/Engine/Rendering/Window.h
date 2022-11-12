#pragma once

#include "Engine/SceneBuilder/Entity.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Engine {

	class Renderer; //forward declaration

	class Window
	{
	private:
		std::unique_ptr<Entity> m_camera;
		int m_windowWidth{ 1920 };
		int m_windowHeight{ 1080 };
		struct GLFWwindow* m_window;
		struct GLFWwindow* m_UIwindow;

	public:
		Window();

		void resize(int width, int height);

		glm::vec3 screenSpaceToWorldSpace(const glm::vec2& screenSpaceVector);
		glm::vec2 worldSpaceToScreenSpace(const glm::vec3& worldSpaceVector);

		//Get's the projection matrix of the camera
		glm::mat4 getProjectionMatrix() const;


		friend Renderer;
	};

	//Callback to update window size when it changes
//TODO: Handle screen resizing
	void windowResizeCallback(GLFWwindow* window, int width, int height)
	{
		/*Scene* scene = reinterpret_cast<Scene*>(glfwGetWindowUserPointer(window));
		auto cameraView = scene->getEntities<const CameraComponent>();
		auto &camera = scene->m_registry.get<CameraComponent>(cameraView.back());
		camera.viewport.x = width;
		camera.viewport.y = height;*/
	}
}

