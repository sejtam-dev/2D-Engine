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

	void CreateShaders();

private:
	static void InitGLEW()
	{
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "GLEW initialization failed." << std::endl;
		}
	}

	void UpdateDeltaTime();
	void CalculateFPS();
};