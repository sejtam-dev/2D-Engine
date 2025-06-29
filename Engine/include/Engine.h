#pragma once

#include "Shader.h"
#include "static.h"
#include "Window.h"
#include "Camera2D.h"

#ifdef IMGUI_ENABLED
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#include <thread>

class Engine {
private:
    float m_currentTime = 0.0f;
    float m_lastTime = 0.0f;
    float m_targetFpsTime = 0.0f;

    float m_fpsLastTime = 0.0f;
    int m_frames = 0;

    uint8_t m_DebugDelay = 0;

protected:
    std::map<std::string, Shader *> m_Shaders;

public:
    Window *window;

    const float TARGET_FPS = 60.0f;

    float deltaTime = 0.0f;
    float fps = 0.0f;

    // Components
    static std::unique_ptr<Camera2D> Camera;

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    Engine();
    virtual ~Engine() = default;

    void Run();

protected:
    virtual void Init() {
    }

    virtual void LoadContent() {
    }

    virtual void UnloadContent() {
    }

    virtual void Update() {
    }

    virtual void Draw() {
    }

    virtual void DrawImGUI() {
    }

    void CreateShaders();
    void CreateCamera() const;

private:
    void InitGLEW() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (glewInit() != GLEW_OK) {
            ERROR("{}", "GLEW initialization failed.");
        }

        LOG("OpenGL version supported by this platform: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    }

#ifdef IMGUI_ENABLED
    void InitImGUI() const {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        const ImGuiIO &io = ImGui::GetIO();
        (void) io;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window->GLFWWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiTerminate() const {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext();
    }
#endif

    void UpdateDeltaTime();

    void CalculateFPS();

    void HoldTargetFPS();
};
