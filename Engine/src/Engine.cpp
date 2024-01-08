#include "Engine.h"

#include <chrono>
#include <thread>

Engine::Engine() {
    window = new Window(800, 600, "Engine");
}

void Engine::Run() {
    // Create window
    window->CreateWindow();

    // Init GLEW
    InitGLEW();

    Init();
    LoadContent();

    CreateShaders();

#ifdef IMGUI_ENABLED
    InitImGUI();
#endif

    m_targetFpsTime = static_cast<float>(glfwGetTime());

    while (!window->ShouldClose()) {
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

        Update();

#ifdef IMGUI_ENABLED
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
#endif

        Draw();

#ifdef IMGUI_ENABLED
        DrawImGUI();

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

    for (auto const &pair: m_Shaders) {
        Shader *shader = pair.second;
        delete shader;
    }

    UnloadContent();

#ifdef IMGUI_ENABLED
    ImGuiTerminate();
#endif

    // Delete window
    glfwTerminate();
    delete window;
}

void Engine::UpdateDeltaTime() {
    m_currentTime = static_cast<float>(glfwGetTime());
    deltaTime = m_currentTime - m_lastTime;
    m_lastTime = m_currentTime;
}

void Engine::CalculateFPS() {
    if (m_fpsLastTime == 0.0f)
        m_fpsLastTime = m_currentTime;

    m_frames++;

    const float delta = m_currentTime - m_fpsLastTime;
    if (delta >= 1.0f) {
        fps = static_cast<float>(m_frames) / delta;

        m_frames = 0;
        m_fpsLastTime = m_currentTime;
    }
}

void Engine::CreateShaders() {
    auto *defaultShader = new Shader(
        "default",
        "Resources/shaders/DefaultVertexShader.shader",
        "Resources/shaders/DefaultFragmentShader.shader"
    );

    m_Shaders.emplace("default", defaultShader);
    Shader::LinkShader(defaultShader);
}

void Engine::HoldTargetFPS() {
    while (glfwGetTime() < m_targetFpsTime + 1.0f / TARGET_FPS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    m_targetFpsTime += 1.0f / TARGET_FPS;
}
