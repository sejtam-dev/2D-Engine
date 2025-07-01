#include "Engine.h"

#include <thread>
#include <ranges>

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

    m_targetFpsTime = static_cast<float>(glfwGetTime());

    while (!window->ShouldClose()) {
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

#ifdef IMGUI_ENABLED
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
#endif

        OnDraw();

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

    m_Shaders.clear();
    OnUnloadContent();

#ifdef IMGUI_ENABLED
    ImGuiTerminate();
#endif

    glfwTerminate();
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
    auto defaultShader = std::make_unique<Shader>(
        "default",
        "Resources/shaders/DefaultVertexShader.shader",
        "Resources/shaders/DefaultFragmentShader.shader"
    );

    Shader::LinkShader(defaultShader.get());
    m_Shaders.emplace("default", std::move(defaultShader));
}

void Engine::CreateCamera() const {
    Engine::Camera = std::make_unique<Camera2D>(
        static_cast<float>(window->GetWidth()),
        static_cast<float>(window->GetHeight())
    );
}

void Engine::HoldTargetFPS() {
    while (glfwGetTime() < m_targetFpsTime + 1.0f / TARGET_FPS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    m_targetFpsTime += 1.0f / TARGET_FPS;
}

std::unique_ptr<Camera2D> Engine::Camera = nullptr;
