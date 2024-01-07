#include "Main.h"

float r = 0;
float value = 0.001f;

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

	GLfloat vertices[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

    GLuint indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLuint bufferId;
	GLCall(glGenBuffers(1, &bufferId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, nullptr));

	GLuint ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indicies[0], GL_STATIC_DRAW));
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
	r += value;
	if (r > 100) {
		value = -10.0f * deltaTime;
	}
	else if(r < 0)
	{
		value = 10.0f * deltaTime;
	}
}

void Game::Draw()
{
	Shader::SetUniform4f("u_Color", glm::vec4(r / 100, 0.4f, 0.4f, 1.0f));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void Game::ChangeSize(Window* window, int width, int height)
{
	LOG("Size changed: " << width << "x" << height);
}
