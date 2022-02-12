#include "Window.h"

Window::Window(int const width, int const height, std::string title) : width(width), height(height), title(std::move(title))
{
	createWindow();
}

Window::~Window()
{
	if(window != nullptr)
	{
		glfwDestroyWindow(window);
	}
}

void Window::createWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if(window == nullptr)
	{
		std::cerr << "GLFW window creation failed!" << std::endl;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::framebufferResizeCallback(GLFWwindow* window, int const width, int const height) {
	auto const engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	engineWindow->width = width;
	engineWindow->height = height;
}

