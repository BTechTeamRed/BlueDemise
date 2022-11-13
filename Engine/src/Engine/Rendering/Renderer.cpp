#include "Renderer.h"
#include "Engine/SceneBuilder/Scene.h"

namespace Engine
{

	void Renderer::renderScene(const DeltaTime& dt, Scene scene)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		auto cameraView = getEntities<const CameraComponent>();
		const auto camera = m_registry.get<CameraComponent>(cameraView.back());
		glm::mat4 pm = glm::ortho(0.f, camera.viewport.x, camera.viewport.y, 0.f, camera.nearZ, camera.farZ);
		glm::mat4 vm = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -10.f)); //position of camera in world-space


		//Render all entities
		//Get entities that contain transform & vertices & color components,
		const auto solidObj = getEntities<const TransformComponent, const VerticesComponent, const ColorComponent>();

		//For each updatable entity (with transform, vertices, and color components), draw them.
		for (auto [entity, transform, vertices, color] : solidObj.each())
		{
			//Bind Texture
			if (m_registry.all_of<TextureComponent>(entity))
			{
				const auto texture = m_registry.get<const TextureComponent>(entity);
				glBindTexture(GL_TEXTURE_2D, texture.texID);
			}

			if (m_registry.all_of<AnimationComponent>(entity))
			{
				glBindBuffer(GL_ARRAY_BUFFER, vertices.vboID);
				auto& anim = m_registry.get<AnimationComponent>(entity);
				anim.deltaTime += dt;

				if (anim.deltaTime > anim.animationSpeed)
				{
					anim.deltaTime = 0;
					anim.currentIndex++;
					if (anim.currentIndex > anim.spritesOnSheet) anim.currentIndex = 0;
				}

				//Calculation for finding the sprite in a texture.
				const float tx = (anim.currentIndex % anim.numPerRow) * anim.texWidthFraction;
				const float ty = (anim.currentIndex / anim.numPerRow + 1) * anim.texHeightFraction;

				//bind VBO
				float vertices[] =
				{
					// positions  // texture coords (UV coords)
					0.f, 0.f, 0.f,  tx, ty,														// top left
					1.f, 0.f, 0.f,  tx + anim.texWidthFraction, ty,								// top right
					1.f, 1.f, 0.f,  tx + anim.texWidthFraction, ty + anim.texHeightFraction,	// bottom right
					0.f, 1.f, 0.f,  tx, ty + anim.texHeightFraction								// bottom left
				};

				//Buffer new data into VBO
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

				glBindTexture(GL_TEXTURE_2D, anim.texID);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, vertices.vboID);
				float vertices[] =
				{
					// positions  // texture coords (UV coords)

					0.f, 0.f, 0.f,  0.f, 0.f,  // top left
					1.f, 0.f, 0.f,  1.f, 0.f,  // top right
					1.f, 1.f, 0.f,  1.f, 1.f,  // bottom right
					0.f, 1.f, 0.f,  0.f, 1.f,  // bottom left
				};

				//Buffer new data into VBO
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
			}

			//Update the MVP
			const glm::mat4 mvp = updateMVP(transform, vm, pm);

			//Set the color of the object
			setColor(mvp, color.color);

			glBindVertexArray(vertices.vaoID);

			glDrawElements(GL_TRIANGLES, vertices.numIndices, GL_UNSIGNED_INT, nullptr);
		}
	}


	/*

	//Insitialize OpenGL, returning true if successful. Window based on GLFW.
	bool Scene::initializeGL()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "BlueDemise", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_window == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return false;
		}

		m_UIwindow = glfwCreateWindow(m_windowWidth, m_windowHeight, "User Interface", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_UIwindow == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window (UI)");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);



		//Setup a callback to update the viewport size when the window is resized
		//glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));
		//glfwSetWindowSizeCallback(m_window, windowResizeCallback);

		//Setting the icon
		ResourceManager::getInstance()->setAppIcon(*m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GE_CORE_ERROR("Failed to initialize GLAD");
			glfwTerminate();
			return false;
		}

		loadShaders();

		glfwSwapInterval(1);
		glClearColor(0.1f, 0.1f, 0.1f, 1);

		return true;
	}

	//Callback to update window size when it changes
	//TODO: Handle screen resizing
	void windowResizeCallback(GLFWwindow* window, int width, int height)
	{
		/*Scene* scene = reinterpret_cast<Scene*>(glfwGetWindowUserPointer(window));
		auto cameraView = scene->getEntities<const CameraComponent>();
		auto &camera = scene->m_registry.get<CameraComponent>(cameraView.back());
		camera.viewport.x = width;
		camera.viewport.y = height;
	}

	//clears the window and renders all entities that need to be rendered (those with transform, vertices, color).

	//loads and generates shaders to be used in scene. Replace with shader wrappers as per the .h todo.
	void Scene::loadShaders()
	{

		std::string vertexData = ResourceManager::getInstance()->getShaderData("Fill.vs");
		std::string fragmentData = ResourceManager::getInstance()->getShaderData("Fill.fs");

		ShaderGenerator shaderGenerator(vertexData.c_str(), fragmentData.c_str());

		m_programId = shaderGenerator.getProgramId();
		glUseProgram(m_programId);
	}


	#pragma region Renderable Entities
	//Set the color of the current drawable object. This would need to be run per entity/renderable.
	void Scene::setColor(glm::mat4 mvp, glm::vec4 color)
	{
		GLuint colorUniformID = glGetUniformLocation(m_programId, "col");
		GLuint mvpID = glGetUniformLocation(m_programId, "mvp");

		//Sets color of shader
		glUniform4fv(colorUniformID, 1, glm::value_ptr(color));
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(mvp));
	}

	//Placeholder functio, can be replaced by serialized objects.
	VerticesComponent Scene::createSprite()
	{
		VerticesComponent vc;
		vc.vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		vc.vertexAttributes.push_back(VertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3));
		vc.stride = sizeof(float) * m_quadTexCoordinates;
		vc.numIndices = m_quadIndices;

		vc.vaoID = getVAO();
		vc.vboID = getVBO();
		vc.iboID = getIBO();

		setupVertexAttribPtr(vc);

		return vc;
	}

	//Placeholder function, can be replaced by serialized objects.
	VerticesComponent Scene::createRectangle()
	{
		VerticesComponent vc;
		vc.vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		vc.stride = sizeof(float) * m_quadTexCoordinates;
		vc.numIndices = m_quadIndices;

		vc.vaoID = getVAO();
		vc.vboID = getVBO(RT_Rectangle);
		vc.iboID = getIBO();

		setupVertexAttribPtr(vc);

		return vc;
	}

	//Define vertex attributes
	void Scene::setupVertexAttribPtr(VerticesComponent& vc)
	{
		for (int i = 0; i < vc.vertexAttributes.size(); i++)
		{
			const auto attribute = vc.vertexAttributes[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, vc.stride, (const void*)attribute.pointer);
		}

		glBindVertexArray(0);
	}

	#pragma endregion
	*/
}