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

	float position[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	GLuint bufferId;
	GLCall(glGenBuffers(1, &bufferId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));
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
	if (r > 1) {
		value = -0.001f;
	}
	else if(r < 0)
	{
		value = 0.001f;
	}
}

void Game::Draw()
{
	Shader::LinkShaders(m_vertexShader.get(), m_fragmentShader.get());
	Shader::SetUniform4f("u_Color", r, 0.4f, 0.4f, 1.0f);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	Shader::UnlinkShaders(); 
}

void Game::ChangeSize(Window* window, int width, int height)
{
	std::cout << "Size changed: " << width << "x" << height << std::endl;
}
