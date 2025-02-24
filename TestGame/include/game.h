#pragma once

#include "engine.h"

class Game : public Engine {
public:
    Game();

protected:
    void Init() override;

    void LoadContent() override;

    void UnloadContent() override;

    void Update() override;

    void Draw() override;

private:
    static void ChangeSize(Window *window, int width, int height);
};
