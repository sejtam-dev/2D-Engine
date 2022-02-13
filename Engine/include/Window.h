#pragma once

#include "Static.h"

class ENGINE_API Window
{
public:
	typedef void(*EngineChangeSize)(Window* window, int width, int height);

	int width;
	int height;
	std::string title;

private:
	GLFWwindow* m_window;
	EngineChangeSize m_changeSizeEvent;

public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	Window(int width, int height, std::string title);
	~Window();

	GLFWwindow* glfwWindow() const
	{
		return m_window;
	}
	bool shouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

	void changeSizeEvent(EngineChangeSize event);
	void createWindow();

private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

};