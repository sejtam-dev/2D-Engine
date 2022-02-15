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

	while (!window->ShouldClose())
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		UpdateDeltaTime();
		CalculateFPS();

#if _DEBUG
		std::stringstream windowTitle;
		windowTitle << window->title << " [" << fps << " FPS]";
		glfwSetWindowTitle(window->GLFWWindow(), windowTitle.str().c_str());
#endif

		Update();
		Draw();

		glfwSwapBuffers(window->GLFWWindow());
		glfwPollEvents();
	}

	Shader::UnlinkShaders();
	Shader::DeleteProgram();

	UnloadContent();

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


