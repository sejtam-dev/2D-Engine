#pragma once

#include "Engine.h"

class Game : public Engine {
public:
    Game();

protected:
    void OnInit() override;

    void OnLoadContent() override;

    void OnUnloadContent() override;

    void OnUpdate() override;

    void OnDraw() override;

private:
    static void ChangeSize(Window* window, int width, int height);
};
