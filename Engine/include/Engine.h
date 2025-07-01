#pragma once
#include <memory>

#include "Shader.h"
#include "Utils.h"
#include "Window.h"
#include "Camera2D.h"
#include "Scene.fwd.h"
#include "Utils/SceneManager.h"

#ifdef IMGUI_ENABLED
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

class Engine {
public:
    Engine(const Engine&) = delete;

    Engine& operator=(const Engine&) = delete;

    Engine();

    virtual ~Engine() = default;

    void Run();

protected:
    virtual void OnInit() {
    }

    virtual void OnLoadContent() {
    }

    virtual void OnUnloadContent() {
    }

    virtual void OnUpdate() {
    }

    virtual void OnDraw() {
    }

    virtual void OnDrawImGUI() {
    }

    void CreateShaders();

    void CreateCamera() const;

private:
    void InitGLEW();

#ifdef IMGUI_ENABLED
    void InitImGUI() const;

    void ImGuiTerminate() const;
#endif

    void UpdateDeltaTime();

    void CalculateFPS();

    void HoldTargetFPS();

public:
    std::unique_ptr<Window> window;
    std::unique_ptr<SceneManager> sceneManager;

    static std::unique_ptr<Camera2D> Camera;

    const float TARGET_FPS = 60.0f;

    float deltaTime = 0.0f;
    float fps = 0.0f;

protected:
    std::map<std::string, std::unique_ptr<Shader>> m_Shaders;

private:
    float m_currentTime = 0.0f;
    float m_lastTime = 0.0f;
    float m_targetFpsTime = 0.0f;

    float m_fpsLastTime = 0.0f;
    int m_frames = 0;

    uint8_t m_DebugDelay = 0;
};
