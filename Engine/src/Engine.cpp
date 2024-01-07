#include "Engine.h"

Engine::Engine()
{
	window = new Window(800, 600, "Engine");
}

void Engine::Run()
{
	// Create window
	window->CreateWindow();

	// Init GLEW
	InitGLEW();

	Init();
	LoadContent();

	CreateShaders();
    Shader::LinkShaders(m_vertexShader.get(), m_fragmentShader.get());

#ifdef IMGUI_ENABLED
	InitImGUI();
#endif

	while (!window->ShouldClose())
	{
		GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		UpdateDeltaTime();
		CalculateFPS();

        if(m_DebugDelay == 200) {
            std::stringstream windowTitle;
            windowTitle << window->title << " [" << fps << " FPS]";
            GLCall(glfwSetWindowTitle(window->GLFWWindow(), windowTitle.str().c_str()));
        }

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

        if(m_DebugDelay >= 255) {
            m_DebugDelay = 0;
        } else m_DebugDelay++;
	}

	Shader::UnlinkShaders();
	Shader::DeleteProgram();

	UnloadContent();

#ifdef IMGUI_ENABLED
	ImGuiTerminate();
#endif

	// Delete window
	glfwTerminate();
	delete window;
}

void Engine::UpdateDeltaTime()
{
	m_currentTime = static_cast<float>(glfwGetTime());
	deltaTime = m_currentTime - m_lastTime;
	m_lastTime = m_currentTime;
}

void Engine::CalculateFPS()
{
	if (m_fpsLastTime == 0.0f)
		m_fpsLastTime = m_currentTime;

	m_frames++;

	const float delta = m_currentTime - m_fpsLastTime;
	if(delta >= 1.0f)
	{
		fps = static_cast<float>(m_frames) / delta;

		m_frames = 0;
		m_fpsLastTime = m_currentTime;
	}
}

void Engine::CreateShaders()
{
	m_vertexShader = std::make_unique<Shader>(Shader(ShaderType::VERTEX));
	m_vertexShader->LoadFromFile("Resources/shaders/DefaultVertexShader.shader");
	m_vertexShader->CreateShader();

	m_fragmentShader = std::make_unique<Shader>(Shader(ShaderType::FRAGMENT));
	m_fragmentShader->LoadFromFile("Resources/shaders/DefaultFragmentShader.shader");
	m_fragmentShader->CreateShader();
}


