#pragma once
#include "SourceGatherer.h"
#include "ShaderGenerator.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <memory>
#include <entt.h>
#include "Components.h"

void renderSystem(const entt::registry & reg, int programId) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 mvp;
	glm::mat4 mvm;
	glm::mat4 pm;

	auto view = reg.view<const TransformComponent, const VerticesComponent, const ColorComponent>();
	auto cameraView = reg.view<const TransformComponent, const CameraComponent>(); 
	const auto camera = reg.get<CameraComponent>(cameraView.back());//Gets the last entity with these components, if there are multiple (TODO: Switch to wrapper)
	pm = glm::ortho(0.0f, camera.viewport.x, 0.0f, camera.viewport.y, camera.nearZ, camera.farZ);
	for (auto [entity, transform, vertices, color] : view.each()) {

		//Setup mvp and mvm matrix
		mvm = glm::mat4(1.f);
		mvm = glm::translate(mvm, transform.position);
		mvm = glm::rotate(mvm, transform.rotation.x, glm::vec3(1, 0, 0));
		mvm = glm::rotate(mvm, transform.rotation.y, glm::vec3(0, 1, 0));
		mvm = glm::rotate(mvm, transform.rotation.z, glm::vec3(0, 0, 1));
		mvm = glm::scale(mvm, transform.scale);

		mvp = pm * mvm;

		glUseProgram(programId);
		GLuint colorUniformID = glGetUniformLocation(programId, "col");
		GLuint mvpID = glGetUniformLocation(programId, "mvp");
		glUniform4fv(colorUniformID, 1, (const float *)glm::value_ptr(color.color)); //Consider changing the way we cast here, probably not best practices
		glUniformMatrix4fv(mvpID, 1,GL_FALSE, (const float*)glm::value_ptr(mvp));

		glBindVertexArray(vertices.vaoID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices.iboID);
		glDrawElements(GL_TRIANGLES, vertices.numIndices, GL_UNSIGNED_INT, 0);
	}
}

//Temporary function to create a vertices component with the proper triangle data
VerticesComponent createTriangle() {
	//Future consideration: have one vao/ibo for a quad that can be used by all sprites in the engine
	static float triangleVertices[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	VerticesComponent vc;
	//Each vertex has one attribute, which is 2 floats to represent position
	vc.vertexAttributes.push_back(VertexAttribute(0, 1, GL_FLOAT, GL_FALSE, 0));
	vc.stride = sizeof(float) * 2;
	

	//Create VAO
	glGenVertexArrays(1, &vc.vaoID);
	//Create IBO
	glGenBuffers(1, &vc.iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vc.iboID);

	//Define vertex attributes
	glEnableVertexAttribArray(0);
	for (const auto attribute : vc.vertexAttributes) {
		glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, vc.stride, (const void *) attribute.pointer);
	}

	//Buffer data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), triangleVertices, GL_STATIC_DRAW);

	return vc;
}

//Temporary function to hold rendering loop, window setup and teardown should be separate from render loop in the future
void renderLoop() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window{ glfwCreateWindow(640, 480, "BlueDemise", NULL, NULL) }; //switch to unique ptr with deleter for RAII?
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	input::SourceGatherer sg(new std::string[2]{
		"..\\Engine\\src\\Engine\\Shaders\\Fill.vs",
		"..\\Engine\\src\\Engine\\Shaders\\Fill.fs"
		}, 2);

	/* -- Read source from cmd
	std::cout << "source(vs):" << std::endl << sg.getSource()
		<< "source(fs):" << std::endl << sg.getSource(1) << std::endl;
		*/

	input::ShaderGenerator shaderGenerator(sg.getSource().c_str(), sg.getSource(1).c_str());
	/* --Test program ID
	std::cout << "programID: " << shaderGenerator.getProgramId() << std::endl;
	*/

	glfwSwapInterval(1);
	glClearColor(0, 0, 0, 0); //set clear color to white
	glEnable(GL_DEPTH_TEST);
	//enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	entt::registry r;
	auto vc = createTriangle();

	TransformComponent t;
	t.position = glm::vec3(0.0f);
	t.scale = glm::vec3(1.0f);
	t.rotation = glm::vec3(0.0f);

	auto lonelyEntity = r.create();
	r.emplace<TransformComponent>(lonelyEntity, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	r.emplace<ColorComponent>(lonelyEntity, glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
	r.emplace<TransformComponent>(lonelyEntity, vc);

	while (!glfwWindowShouldClose(window))
	{
		renderSystem(r, shaderGenerator.getProgramId());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}