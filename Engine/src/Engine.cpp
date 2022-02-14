#include "Engine.h"

Engine::Engine()
{
	window = new Window(800, 600, "Engine");
}

void Engine::run()
{
	// Create window
	window->createWindow();

	// Init GLEW
	initGLEW();

	init();
	loadContent();

	Shader* vertexShader = new Shader(ShaderType::VERTEX);
	vertexShader->loadFromFile("Resources/shaders/DefaultVertexShader.shader");
	vertexShader->createShader();

	Shader* fragmentShader = new Shader(ShaderType::FRAGMENT);
	fragmentShader->loadFromFile("Resources/shaders/DefaultFragmentShader.shader");
	fragmentShader->createShader();

	Shader::linkShaders(vertexShader, fragmentShader);

	while (!window->shouldClose())
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		updateDeltaTime();
		calculateFPS();

#if _DEBUG
		std::stringstream windowTitle;
		windowTitle << window->title << " [" << fps << " FPS]";
		glfwSetWindowTitle(window->glfwWindow(), windowTitle.str().c_str());
#endif

		update();
		draw();

		glfwSwapBuffers(window->glfwWindow());
		glfwPollEvents();
	}

	unloadContent();

	// Delete window
	glfwTerminate();
	delete window;
}

void Engine::updateDeltaTime()
{
	m_currentTime = static_cast<float>(glfwGetTime());
	deltaTime = m_currentTime - m_lastTime;
	m_lastTime = m_currentTime;
}

void Engine::calculateFPS()
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


