#include "Window.h"

Window::Window(int const width, int const height, std::string title) : width(width), height(height), title(std::move(title))
{
	glfwInit();
}

Window::~Window()
{
	if(m_window != nullptr)
	{
		glfwDestroyWindow(m_window);
	}
}

void Window::createWindow()
{
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if(m_window == nullptr)
	{
		std::cerr << "GLFW window creation failed!" << std::endl;
	}

	glfwMakeContextCurrent(m_window);

	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto engineWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	engineWindow->width = width;
	engineWindow->height = height;

	if (engineWindow->m_changeSizeEvent)
		engineWindow->m_changeSizeEvent(engineWindow, width, height);
}

void Window::changeSizeEvent(EngineChangeSize event)
{
	m_changeSizeEvent = event;
}