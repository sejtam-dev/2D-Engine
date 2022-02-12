#pragma once

#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Window.h"

#ifdef Engine_EXPORTS
#define Engine_API __declspec(dllexport)
#else
#define Engine_API __declspec(dllimport)
#endif

class Engine_API Engine
{
public:
	Window* window;

	Engine();
	~Engine();

private:
	static void initGLEW()
	{
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "GLEW initialization failed." << std::endl;

			return;
		}
	}
};