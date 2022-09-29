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

	void Scene::onRuntimeStart()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
			return;
		}

		input::SourceGatherer sg(new std::string[2]{
			"..\\Engine\\src\\Engine\\Shaders\\Fill.vs",
			"..\\Engine\\src\\Engine\\Shaders\\Fill.fs"
			}, 2);

		/* --read shader source
		GE_CORE_INFO("Source(vs):\n");
		sg.print();
		GE_CORE_INFO("Source(fs):\n");
		sg.print(1);
		*/

		input::ShaderGenerator shaderGenerator(sg.getSource().c_str(), sg.getSource(1).c_str());
		m_programId = shaderGenerator.getProgramId();
		/* --Test program ID
		std::cout << "programID: " << shaderGenerator.getProgramId() << std::endl;
		*/

		glfwSwapInterval(1);
		glClearColor(0, 0, 0, 0); //set clear color to white
		glEnable(GL_DEPTH_TEST);
		//enable transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Camera
		Entity cameraEntity = createEntity("camera");
		cameraEntity.addComponent<CameraComponent>(
			90.f,
			glm::mat4(1.f),
			glm::vec2(480, 480),
			-1.f,
			1.f
			);

		Entity xdEntity = createEntity(";)");
		xdEntity.addComponent<TransformComponent>(
			glm::vec3(0,0,0),
			glm::vec3(1,1,1),
			glm::vec3(0, 0, 0)
			);
		auto vc = createTriangle();
		xdEntity.addComponent<VerticesComponent>(vc);
		xdEntity.addComponent<ColorComponent>(glm::vec4(0, 0, 1, 0.5f));

		while (!glfwWindowShouldClose(m_window))
		{
			onRuntimeUpdate();
		}

		onRuntimeStop();
	}

	VerticesComponent Scene::createTriangle()
	{
		//Future consideration: have one vao/ibo for a quad that can be used by all sprites in the engine
		static float triangleVertices[6] =
		{
			-0.5f, -0.5f,
			0.0f, 0.5f,
			0.5f, -0.5f
		};

		VerticesComponent vc;
		//Each vertex has one attribute, which is 2 floats to represent position
		vc.vertexAttributes.push_back(VertexAttribute(0, 1, GL_FLOAT, GL_FALSE, 0));
		vc.stride = sizeof(float) * 2;
		vc.numIndices = 6;


		glGenVertexArrays(1, &vc.vaoID);
		glBindVertexArray(vc.vaoID);

		glGenBuffers(1, &vc.vboID);
		glGenBuffers(1, &vc.iboID);
		

		glBindBuffer(GL_ARRAY_BUFFER, vc.vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vc.iboID);

		//Define vertex attributes
		glEnableVertexAttribArray(0);
		for (const auto attribute : vc.vertexAttributes) 
		{
			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, vc.stride, (const void*)attribute.pointer);
		}

		//Buffer data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), triangleVertices, GL_STATIC_DRAW);

		return vc;
	}

	void Scene::onRuntimeStop()
	{
		glfwTerminate();
	}

	void Scene::onRuntimeUpdate()
	{
		renderScene();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Scene::renderScene()
	{
		//Gets the last entity with these components, if there are multiple (TODO: Switch to wrapper)
		//const auto camera = m_registry.get<CameraComponent>(cameraView.back());

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 mvp;
		glm::mat4 mvm;
		glm::mat4 pm;

		auto view = getEntities<const TransformComponent, const VerticesComponent, const ColorComponent>();
		auto cameraView = getEntities<const CameraComponent>();
		const auto camera = m_registry.get<CameraComponent>(cameraView.back());//Gets the last entity with these components, if there are multiple (TODO: Switch to wrapper)
		pm = glm::ortho(0.0f, camera.viewport.x, 0.0f, camera.viewport.y, camera.nearZ, camera.farZ);
		for (auto [entity, transform, vertices, color] : view.each())
		{

			//Setup mvp and mvm matrix
			mvm = glm::mat4(1.f);
			mvm = glm::translate(mvm, transform.position);
			mvm = glm::rotate(mvm, transform.rotation.x, glm::vec3(1, 0, 0));
			mvm = glm::rotate(mvm, transform.rotation.y, glm::vec3(0, 1, 0));
			mvm = glm::rotate(mvm, transform.rotation.z, glm::vec3(0, 0, 1));
			mvm = glm::scale(mvm, transform.scale);

			mvp = pm * mvm;

			glUseProgram(m_programId);
			GLuint colorUniformID = glGetUniformLocation(m_programId, "col");
			GLuint mvpID = glGetUniformLocation(m_programId, "mvp");
			glUniform4fv(colorUniformID, 1, (const float *)glm::value_ptr(color.color)); //Consider changing the way we cast here, probably not best practices
			glUniformMatrix4fv(mvpID, 1, GL_FALSE, (const float*)glm::value_ptr(mvp));

			glBindVertexArray(vertices.vaoID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices.iboID);
			glDrawElements(GL_TRIANGLES, (GLsizei)vertices.numIndices, GL_UNSIGNED_INT, 0);
		}
		
	}
}
