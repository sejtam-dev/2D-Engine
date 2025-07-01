#pragma once

#include "Utils.h"

class Window {
public:
    typedef void (*EngineChangeSize)(Window* window, int width, int height);

    std::string title;

private:
    int width;
    int height;

    GLFWwindow* m_window;
    EngineChangeSize m_changeSizeEvent;

public:
    Window(const Window&) = delete;

    Window& operator=(const Window&) = delete;

    Window(int width, int height, std::string title);

    ~Window();

    void CreateWindow();

    GLFWwindow* GLFWWindow() const {
        return m_window;
    }

    bool ShouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    void ChangeSizeEvent(EngineChangeSize event);

    void ChangeSize(int width, int height);

    int GetWidth() const {
        return width;
    }

    int GetHeight() const {
        return width;
    }

private:
    static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);
};
