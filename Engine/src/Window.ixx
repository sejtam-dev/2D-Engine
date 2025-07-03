module;
#include "GLFW/glfw3.h"
#include <string>

export module Engine:Window;

export class Window {
public:
    typedef void (*EngineChangeSize)(const Window* window, const int width, const int height);

    Window(const Window&) = delete;

    Window& operator=(const Window&) = delete;

    Window(const int width, const int height, std::string title);

    ~Window();

    void CreateWindow();

    GLFWwindow* GLFWWindow() const {
        return m_window;
    }

    bool ShouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    void ChangeSizeEvent(const EngineChangeSize event) {
        m_changeSizeEvent = event;
    }

    void ChangeSize(const int width, const int height) {
        this->width = width;
        this->height = height;

        glfwSetWindowSize(m_window, width, height);
    }

    int GetWidth() const {
        return width;
    }

    int GetHeight() const {
        return width;
    }

    std::string title;

private:
    static void FrameBufferResizeCallback(GLFWwindow* window, const int width, const int height);

    int width;
    int height;

    GLFWwindow* m_window;
    EngineChangeSize m_changeSizeEvent;
};
