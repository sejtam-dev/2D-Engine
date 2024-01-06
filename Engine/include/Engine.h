#pragma once

#include "Static.h"
#include "Window.h"
#include "Shader.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <thread>

class Engine
{
private:
	float m_currentTime = 0.0f;
	float m_lastTime = 0.0f;

	float m_fpsLastTime = 0.0f;
	int m_frames = 0;

protected:
	std::unique_ptr<Shader> m_vertexShader;
	std::unique_ptr<Shader> m_fragmentShader;

public:
	Window* window;

	float deltaTime = 0.0f;
	float fps = 0.0f;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	Engine();

	void Run();

protected:
	virtual void Init() {}

	virtual void LoadContent() {}
	virtual void UnloadContent() {}

	virtual void Update() {}

	virtual void Draw() {}
	virtual void DrawImGUI() {}

	void CreateShaders();

private:
	void InitGLEW()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (glewInit() != GLEW_OK)
		{
			std::cerr << "GLEW initialization failed." << std::endl;
		}

		printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
	}

	void InitImGUI() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window->GLFWWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGuiTerminate() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void UpdateDeltaTime();
	void CalculateFPS();
};