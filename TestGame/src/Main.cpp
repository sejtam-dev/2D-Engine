#include "Main.h"

int main()
{
	const std::unique_ptr<Game> game(new Game());
	game->Run();

	return 0;
}

Game::Game()
{
	window->title = "TestGame";
}

void Game::Init()
{
	window->ChangeSizeEvent(ChangeSize);
}

void Game::LoadContent()
{

}

void Game::UnloadContent()
{

}

void Game::Update()
{
	//std::cout << deltaTime << std::endl;
}

void Game::Draw()
{
	glBegin(GL_TRIANGLES);


	Shader::SetUniform4f("u_color", 0, 0, 1, 1);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0, 0);
	glVertex2f(-0.5, 0.5);

	Shader::SetUniform4f("u_color", 1, 1, 1, 1);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0, 0);
	glVertex2f(0.5, 0.5);

	glVertex2f(0.5, 0.5);
	glVertex2f(0, 0);
	glVertex2f(0.5, 0);

	Shader::SetUniform4f("u_color", 1, 0, 0, 1);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0, 0);
	glVertex2f(0.5, -0.5);

	glVertex2f(0.5, -0.5);
	glVertex2f(0, 0);
	glVertex2f(0.5, 0);

	glEnd();
}

void Game::ChangeSize(Window* window, int width, int height)
{
	std::cout << "Size changed: " << width << "x" << height << std::endl;
}
