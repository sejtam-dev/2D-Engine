#include "Main.h"

int main()
{
	const Engine engine;

	while (!engine.window->shouldClose())
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(engine.window->glfwWindow());
		glfwPollEvents();
	}

	return 0;
}