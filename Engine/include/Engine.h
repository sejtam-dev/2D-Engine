#pragma once

#include "Static.h"
#include "Window.h"
#include "Shader.h"

#include <thread>

class ENGINE_API Engine
{
private:
	float m_currentTime = 0.0f;
	float m_lastTime = 0.0f;

	float m_fpsLastTime = 0.0f;
	int m_frames = 0;

protected:
	Shader* vertexShader;
	Shader* fragmentShader;

public:
	Window* window;

	float deltaTime = 0.0f;
	float fps = 0.0f;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	Engine();

	void run();

protected:
	virtual void init() {}

	virtual void loadContent() {}
	virtual void unloadContent() {}

	virtual void update() {}
	virtual void draw() {}

	void createShaders();

private:
	static void initGLEW()
	{
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "GLEW initialization failed." << std::endl;
		}
	}

	void updateDeltaTime();
	void calculateFPS();
};