#include "glad/glad.h"
#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>

#include "Log.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Entity.h"
#include "ShaderGenerator.h"
#include "SourceGatherer.h"
#include "Components.h"

namespace Engine
{
    Entity Scene::createEntity(std::string tag)
    {
        Entity entity(m_registry.create(), this);
        entity.addComponent<TagComponent>(tag);
        return entity;
    }

	// framebuffer size callback function
	void resize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Scene::onRuntimeStart()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(480, 480, "BlueDemise", nullptr, nullptr); //switch to unique ptr with deleter for RAII?
		if (m_window == nullptr)
		{
			GE_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GE_CORE_ERROR("Failed to initialize GLAD");
			glfwTerminate();
			return;
		}

		input::SourceGatherer sg(new std::string[2]{
			"..\\Engine\\src\\Engine\\Shaders\\Fill.vs",
			"..\\Engine\\src\\Engine\\Shaders\\Fill.fs"
			}, 2);

		input::ShaderGenerator shaderGenerator(sg.getSource().c_str(), sg.getSource(1).c_str());
		defaultShaders.fillShader = shaderGenerator.getProgramId();
		m_programId = defaultShaders.fillShader;
		glUseProgram(m_programId);

		glfwSwapInterval(1);
		glClearColor(0.1f, 0.1f, 0.1f, 1);

		// //enable transparency
		// glEnable(GL_DEPTH_TEST);
		// glEnable(GL_BLEND);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Camera
		Entity cameraEntity = createEntity("camera");
		cameraEntity.addComponent<CameraComponent>(
			90.f,
			glm::mat4(1.f),
			glm::vec2(4.8, 4.8),
			100.0f,
			0.1f
			);

		//
		auto triangle = createEntity("triangless");
		triangle.addComponent<TransformComponent>(
			glm::vec3(0,0,0),
			glm::vec3(1,1,1),
			glm::vec3(0, 0, 0)
			);
		triangle.addComponent<VerticesComponent>(createTriangle());
		triangle.addComponent<ColorComponent>(glm::vec4(0, 0, 1, 0.5f));

		while (!glfwWindowShouldClose(m_window))
		{
			onRuntimeUpdate();
		}

		onRuntimeStop();
	}

	VerticesComponent Scene::createTriangle()
	{
		//Future consideration: have one vao/ibo for a quad that can be used by all sprites in the engine
		float triangleVertices[9] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};
		//vertex order
		unsigned int indices[3] = { 0, 1, 2 };

		VerticesComponent vc;
		//Each vertex has one attribute, which is 2 floats to represent position
		vc.vertexAttributes.push_back(VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0));
		vc.stride = sizeof(float) * 3;
		vc.numIndices = 6;

		glGenVertexArrays(1, &vc.vaoID);
		glBindVertexArray(vc.vaoID);

		glGenBuffers(1, &vc.vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vc.vboID);

    	//Buffer data
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);

		//Define vertex attributes
		for (const auto attribute : vc.vertexAttributes) 
		{
			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, vc.stride, (const void*)attribute.pointer);
		}

		glGenBuffers(1, &vc.iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vc.iboID);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		return vc;
	}

	void Scene::onRuntimeStop()
	{
		glfwTerminate();

		auto view = getEntities<const VerticesComponent>();
		for (auto [entity, vertices] : view.each())
		{
			glDeleteVertexArrays(1, &vertices.vaoID);
			glDeleteBuffers(1, &vertices.vboID);
		}
		glDeleteProgram(defaultShaders.fillShader);
	}

	void Scene::onRuntimeUpdate()
	{
		renderScene();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Scene::renderScene()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// glEnable(GL_DEPTH_TEST);
		// glEnable(GL_BLEND);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto view = getEntities<const TransformComponent, const VerticesComponent, const ColorComponent>();
		auto cameraView = getEntities<const CameraComponent>();
		const auto camera = m_registry.get<CameraComponent>(cameraView.back());
		glm::mat4 pm = glm::ortho(-camera.viewport.x / 2, camera.viewport.x /2,
			-camera.viewport.y / 2, camera.viewport.y /2 , camera.nearZ, camera.farZ);
		glm::mat4 vm = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -10.f)); //position of camera in worldspace

		for (auto [entity, transform, vertices, color] : view.each())
		{
			//Setup mvp and mvm matrix
			glm::mat4 mvm = glm::mat4(1.f);
			mvm = glm::translate(mvm, transform.position);
			mvm = glm::rotate(mvm, transform.rotation.x, glm::vec3(1, 0, 0));
			mvm = glm::rotate(mvm, transform.rotation.y, glm::vec3(0, 1, 0));
			mvm = glm::rotate(mvm, transform.rotation.z, glm::vec3(0, 0, 1));
			mvm = glm::scale(mvm, transform.scale);
			mvm = vm * mvm;

			glm::mat4 mvp = pm * mvm;

			GLuint colorUniformID = glGetUniformLocation(m_programId, "col");
			GLuint mvpID = glGetUniformLocation(m_programId, "mvp");
			glUniform4fv(colorUniformID, 1, glm::value_ptr(color.color));
			glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(mvp));

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
		}
	}
}
