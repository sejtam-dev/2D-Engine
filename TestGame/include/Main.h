#pragma once

#include "Engine.h"

class Game : public Engine
{
public:
	Game();

protected:
	void init() override;

	void loadContent() override;
	void unloadContent() override;

	void update() override;
	void draw() override;

private:
	static void changeSize(Window* window, int width, int height);
};