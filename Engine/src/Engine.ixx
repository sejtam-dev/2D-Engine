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

export module Engine:Engine;

export import :Window;
export import :Camera2D;

import Engine.Graphics;
import Engine.Utils;

export class Engine {
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
    void UpdateLoop();

    void FixedUpdateLoop();

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

    static float deltaTime;
    float fps = 0.0f;

private:
    float m_CurrentTime = 0.0f;
    float m_LastTime = 0.0f;
    float m_TargetFpsTime = 0.0f;

    float m_FpsLastTime = 0.0f;
    int m_Frames = 0;

    uint8_t m_DebugDelay = 0;

    std::thread m_FixedUpdateThread;
    std::atomic<bool> m_Running = false;
    std::mutex m_SceneMutex;
};
