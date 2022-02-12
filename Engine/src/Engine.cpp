#include "Engine.h"

Engine::Engine()
{
	window = new Window(800, 600, "Test");
	initGLEW();
}

Engine::~Engine()
{
	delete window;

	glfwTerminate();
}
