#include "Main.h"

int main()
{
	const std::unique_ptr<Game> game(new Game());
	game->run();

	return 0;
}

Game::Game()
{
	window->title = "TestGame";
}

void Game::init()
{
	window->changeSizeEvent(changeSize);
}

void Game::loadContent()
{

}

void Game::unloadContent()
{

}

void Game::update()
{
	//std::cout << deltaTime << std::endl;
}

void Game::draw()
{
	glBegin(GL_TRIANGLES);
	
	glColor3f(0, 0, 1);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0, 0);
	glVertex2f(-0.5, 0.5);

	glColor3f(1, 1, 1);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0, 0);
	glVertex2f(0.5, 0.5);

	glVertex2f(0.5, 0.5);
	glVertex2f(0, 0);
	glVertex2f(0.5, 0);

	glColor3f(1, 0, 0);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0, 0);
	glVertex2f(0.5, -0.5);

	glVertex2f(0.5, -0.5);
	glVertex2f(0, 0);
	glVertex2f(0.5, 0);

	glEnd();
}

void Game::changeSize(Window* window, int width, int height)
{
	std::cout << "Size changed: " << width << "x" << height << std::endl;
}
