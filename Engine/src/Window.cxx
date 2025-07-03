module;
#include "Utils.h"
#include "GLFW/glfw3.h"
#include <string>

module Engine:WindowImpl;
import :Window;

Window::Window(const int width, const int height, std::string title)
    : title(std::move(title)), width(width), height(height), m_window(nullptr), m_changeSizeEvent(nullptr) {
    glfwInit();
}

Window::~Window() {
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
    }
}

void Window::CreateWindow() {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) {
        ERROR("{}", "GLFW window creation failed!");
        return;
    }

    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, FrameBufferResizeCallback);
}

void Window::FrameBufferResizeCallback(GLFWwindow* window, const int width, const int height) {
    const auto engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    engineWindow->width = width;
    engineWindow->height = height;

    GLCall(glViewport(0, 0, width, height));
    //Engine::Camera->SetSize(width, height);

    if (engineWindow->m_changeSizeEvent) {
        engineWindow->m_changeSizeEvent(engineWindow, width, height);
    }
}