#pragma once

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <memory>
#include <entt.h>

void RenderSystem(entt::registry r) {
	
}



//Temporary function to hold rendering loop, window setup and teardown should be separate from render loop in the future
void RenderLoop() {
	glfwInit();

	GLFWwindow* window{ glfwCreateWindow(640, 480, "BlueDemise", NULL, NULL) };
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}