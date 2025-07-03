module;
#include "Utils.h"
#include "GLFW/glfw3.h"

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <sstream>

#ifdef IMGUI_ENABLED
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

module Engine:EngineImpl;
import :Engine;
import :Window;
import :Camera2D;

import Engine.Utils;

Engine::Engine() {
    window = std::make_unique<Window>(800, 600, "Engine");
    sceneManager = std::make_unique<SceneManager>();
}

void Engine::Run() {
    // Create window
    window->CreateWindow();

    // Init GLEW
    InitGLEW();

    // Create components
    // TODO: Rework -> Change camera to component
    CreateCamera();

    OnInit();
    OnLoadContent();

    CreateShaders();

#ifdef IMGUI_ENABLED
    InitImGUI();
#endif

    m_TargetFpsTime = static_cast<float>(glfwGetTime());

    m_Running = true;
    m_FixedUpdateThread = std::thread(&Engine::FixedUpdateLoop, this);

    UpdateLoop();

    m_Running = false;
    if (m_FixedUpdateThread.joinable()) {
        m_FixedUpdateThread.join();
    }

    Shader::ClearShaders();
    OnUnloadContent();

#ifdef IMGUI_ENABLED
    ImGuiTerminate();
#endif

    glfwTerminate();
}

void Engine::CreateShaders() {
    Shader::CreateShader(
        "default",
        "Resources/shaders/DefaultVertexShader.shader",
        "Resources/shaders/DefaultFragmentShader.shader"
    );

    Shader::LinkShader("default");
}

void Engine::CreateCamera() const {
    Engine::Camera = std::make_unique<Camera2D>(
        static_cast<float>(window->GetWidth()),
        static_cast<float>(window->GetHeight())
    );
}

void Engine::UpdateLoop() {
    while (!window->ShouldClose()) {
        std::lock_guard lock(m_SceneMutex);
        sceneManager->ProcessPendingChanges();

        GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        UpdateDeltaTime();
        CalculateFPS();

#ifdef DEBUG
        if (m_DebugDelay == 200) {
            std::stringstream windowTitle;
            windowTitle << window->title << " [" << fps << " FPS]";
            GLCall(glfwSetWindowTitle(window->GLFWWindow(), windowTitle.str().c_str()))
        }
#endif

        OnUpdate();
        sceneManager->HandleUpdate();

#ifdef IMGUI_ENABLED
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
#endif

        OnDraw();
        sceneManager->HandleDraw();

#ifdef IMGUI_ENABLED
        OnDrawImGUI();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

        GLCall(glfwSwapBuffers(window->GLFWWindow()));
        GLCall(glfwPollEvents());

        if (m_DebugDelay >= 255) {
            m_DebugDelay = 0;
        } else m_DebugDelay++;

        HoldTargetFPS();
    }
}

void Engine::FixedUpdateLoop() {
    constexpr float fixedDeltaTime = 0.02f;
    while (m_Running) {
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::lock_guard lock(m_SceneMutex);
            sceneManager->HandleFixedUpdate();
        }
        std::this_thread::sleep_until(start + std::chrono::duration<float>(fixedDeltaTime));
    }
}

void Engine::InitGLEW() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (glewInit() != GLEW_OK) {
        ERROR("{}", "GLEW initialization failed.");
    }

    LOG("OpenGL version supported by this platform: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

#ifdef IMGUI_ENABLED
    void Engine::InitImGUI() const {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        const ImGuiIO& io = ImGui::GetIO();
        (void) io;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window->GLFWWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void Engine::ImGuiTerminate() const {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext();
    }
#endif

void Engine::UpdateDeltaTime() {
    m_CurrentTime = static_cast<float>(glfwGetTime());
    deltaTime = m_CurrentTime - m_LastTime;
    m_LastTime = m_CurrentTime;
}

void Engine::CalculateFPS() {
    if (m_FpsLastTime == 0.0f)
        m_FpsLastTime = m_CurrentTime;

    m_Frames++;

    const float delta = m_CurrentTime - m_FpsLastTime;
    if (delta >= 1.0f) {
        fps = static_cast<float>(m_Frames) / delta;

        m_Frames = 0;
        m_FpsLastTime = m_CurrentTime;
    }
}

void Engine::HoldTargetFPS() {
    if (TARGET_FPS == 0.0f) {
        return;
    }

    while (glfwGetTime() < m_TargetFpsTime + 1.0f / TARGET_FPS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    m_TargetFpsTime += 1.0f / TARGET_FPS;
}

float Engine::deltaTime = 0.0f;
std::unique_ptr<Camera2D> Engine::Camera = nullptr;