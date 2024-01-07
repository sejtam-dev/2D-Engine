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

void Window::CreateWindow()
{
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if(m_window == nullptr)
	{
		ERROR("GLFW window creation failed!");
	}

	glfwMakeContextCurrent(m_window);

	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, FrameBufferResizeCallback);
}

void Window::FrameBufferResizeCallback(GLFWwindow* window, const int width, const int height) {
	const auto engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	engineWindow->width = width;
	engineWindow->height = height;

	if (engineWindow->m_changeSizeEvent)
		engineWindow->m_changeSizeEvent(engineWindow, width, height);
}

void Window::ChangeSizeEvent(EngineChangeSize event)
{
	m_changeSizeEvent = event;
}

void Window::ChangeSize(int width, int height)
{
	this->width = width;
	this->height = height;

	glfwSetWindowSize(m_window, width, height);
}
