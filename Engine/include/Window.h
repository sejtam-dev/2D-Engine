#pragma once

#include <iostream>

#include <GLFW/glfw3.h>

class Window
{
public:
	int width;
	int height;
	const std::string title;

private:
	GLFWwindow* window;

public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	Window(int width, int height, std::string title);
	~Window();

	GLFWwindow* glfwWindow() const
	{
		return window;
	}
	bool shouldClose() const
	{
		return glfwWindowShouldClose(window);
	}

private:
	void createWindow();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

};